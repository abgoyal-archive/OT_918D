/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 * linux/drivers/video/omap2/omapfb-sysfs.c
 *
 * Copyright (C) 2008 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@nokia.com>
 *
 * Some code and ideas taken from drivers/video/omap/ driver
 * by Imre Deak.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/fb.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/omapfb.h>

#include <plat/display.h>
#include <plat/vrfb.h>

#include "omapfb.h"

static ssize_t show_rotate_type(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);

	return snprintf(buf, PAGE_SIZE, "%d\n", ofbi->rotation_type);
}

static ssize_t store_rotate_type(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);
	enum omap_dss_rotation_type rot_type;
	int r;

	rot_type = simple_strtoul(buf, NULL, 0);

	if (rot_type != OMAP_DSS_ROT_DMA && rot_type != OMAP_DSS_ROT_VRFB)
		return -EINVAL;

	if (!lock_fb_info(fbi))
		return -ENODEV;

	r = 0;
	if (rot_type == ofbi->rotation_type)
		goto out;

	if (ofbi->region.size) {
		r = -EBUSY;
		goto out;
	}

	ofbi->rotation_type = rot_type;

	/*
	 * Since the VRAM for this FB is not allocated at the moment we don't
	 * need to do any further parameter checking at this point.
	 */
out:
	unlock_fb_info(fbi);

	return r ? r : count;
}


static ssize_t show_mirror(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);

	return snprintf(buf, PAGE_SIZE, "%d\n", ofbi->mirror);
}

static ssize_t store_mirror(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);
	bool mirror;
	int r;
	struct fb_var_screeninfo new_var;

	mirror = simple_strtoul(buf, NULL, 0);

	if (mirror != 0 && mirror != 1)
		return -EINVAL;

	if (!lock_fb_info(fbi))
		return -ENODEV;

	ofbi->mirror = mirror;

	memcpy(&new_var, &fbi->var, sizeof(new_var));
	r = check_fb_var(fbi, &new_var);
	if (r)
		goto out;
	memcpy(&fbi->var, &new_var, sizeof(fbi->var));

	set_fb_fix(fbi);

	r = omapfb_apply_changes(fbi, 0);
	if (r)
		goto out;

	r = count;
out:
	unlock_fb_info(fbi);

	return r;
}

static ssize_t show_overlays(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);
	struct omapfb2_device *fbdev = ofbi->fbdev;
	ssize_t l = 0;
	int t;

	if (!lock_fb_info(fbi))
		return -ENODEV;
	omapfb_lock(fbdev);

	for (t = 0; t < ofbi->num_overlays; t++) {
		struct omap_overlay *ovl = ofbi->overlays[t];
		int ovlnum;

		for (ovlnum = 0; ovlnum < fbdev->num_overlays; ++ovlnum)
			if (ovl == fbdev->overlays[ovlnum])
				break;

		l += snprintf(buf + l, PAGE_SIZE - l, "%s%d",
				t == 0 ? "" : ",", ovlnum);
	}

	l += snprintf(buf + l, PAGE_SIZE - l, "\n");

	omapfb_unlock(fbdev);
	unlock_fb_info(fbi);

	return l;
}

static struct omapfb_info *get_overlay_fb(struct omapfb2_device *fbdev,
		struct omap_overlay *ovl)
{
	int i, t;

	for (i = 0; i < fbdev->num_fbs; i++) {
		struct omapfb_info *ofbi = FB2OFB(fbdev->fbs[i]);

		for (t = 0; t < ofbi->num_overlays; t++) {
			if (ofbi->overlays[t] == ovl)
				return ofbi;
		}
	}

	return NULL;
}

static ssize_t store_overlays(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);
	struct omapfb2_device *fbdev = ofbi->fbdev;
	struct omap_overlay *ovls[OMAPFB_MAX_OVL_PER_FB];
	struct omap_overlay *ovl;
	int num_ovls, r, i;
	int len;
	bool added = false;

	num_ovls = 0;

	len = strlen(buf);
	if (buf[len - 1] == '\n')
		len = len - 1;

	if (!lock_fb_info(fbi))
		return -ENODEV;
	omapfb_lock(fbdev);

	if (len > 0) {
		char *p = (char *)buf;
		int ovlnum;

		while (p < buf + len) {
			int found;
			if (num_ovls == OMAPFB_MAX_OVL_PER_FB) {
				r = -EINVAL;
				goto out;
			}

			ovlnum = simple_strtoul(p, &p, 0);
			if (ovlnum > fbdev->num_overlays) {
				r = -EINVAL;
				goto out;
			}

			found = 0;
			for (i = 0; i < num_ovls; ++i) {
				if (ovls[i] == fbdev->overlays[ovlnum]) {
					found = 1;
					break;
				}
			}

			if (!found)
				ovls[num_ovls++] = fbdev->overlays[ovlnum];

			p++;
		}
	}

	for (i = 0; i < num_ovls; ++i) {
		struct omapfb_info *ofbi2 = get_overlay_fb(fbdev, ovls[i]);
		if (ofbi2 && ofbi2 != ofbi) {
			dev_err(fbdev->dev, "overlay already in use\n");
			r = -EINVAL;
			goto out;
		}
	}

	/* detach unused overlays */
	for (i = 0; i < ofbi->num_overlays; ++i) {
		int t, found;

		ovl = ofbi->overlays[i];

		found = 0;

		for (t = 0; t < num_ovls; ++t) {
			if (ovl == ovls[t]) {
				found = 1;
				break;
			}
		}

		if (found)
			continue;

		DBG("detaching %d\n", ofbi->overlays[i]->id);

		omapfb_overlay_enable(ovl, 0);

		if (ovl->manager)
			ovl->manager->apply(ovl->manager);

		for (t = i + 1; t < ofbi->num_overlays; t++) {
			ofbi->rotation[t-1] = ofbi->rotation[t];
			ofbi->overlays[t-1] = ofbi->overlays[t];
		}

		ofbi->num_overlays--;
		i--;
	}

	for (i = 0; i < num_ovls; ++i) {
		int t, found;

		ovl = ovls[i];

		found = 0;

		for (t = 0; t < ofbi->num_overlays; ++t) {
			if (ovl == ofbi->overlays[t]) {
				found = 1;
				break;
			}
		}

		if (found)
			continue;
		ofbi->rotation[ofbi->num_overlays] = 0;
		ofbi->overlays[ofbi->num_overlays++] = ovl;

		added = true;
	}

	if (added) {
		r = omapfb_apply_changes(fbi, 0);
		if (r)
			goto out;
	}

	r = count;
out:
	omapfb_unlock(fbdev);
	unlock_fb_info(fbi);

	return r;
}

static ssize_t show_overlays_rotate(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);
	ssize_t l = 0;
	int t;

	if (!lock_fb_info(fbi))
		return -ENODEV;

	for (t = 0; t < ofbi->num_overlays; t++) {
		l += snprintf(buf + l, PAGE_SIZE - l, "%s%d",
				t == 0 ? "" : ",", ofbi->rotation[t]);
	}

	l += snprintf(buf + l, PAGE_SIZE - l, "\n");

	unlock_fb_info(fbi);

	return l;
}

static ssize_t store_overlays_rotate(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);
	int num_ovls = 0, r, i;
	int len;
	bool changed = false;
	u8 rotation[OMAPFB_MAX_OVL_PER_FB];

	len = strlen(buf);
	if (buf[len - 1] == '\n')
		len = len - 1;

	if (!lock_fb_info(fbi))
		return -ENODEV;

	if (len > 0) {
		char *p = (char *)buf;

		while (p < buf + len) {
			int rot;

			if (num_ovls == ofbi->num_overlays) {
				r = -EINVAL;
				goto out;
			}

			rot = simple_strtoul(p, &p, 0);
			if (rot < 0 || rot > 3) {
				r = -EINVAL;
				goto out;
			}

			if (ofbi->rotation[num_ovls] != rot)
				changed = true;

			rotation[num_ovls++] = rot;

			p++;
		}
	}

	if (num_ovls != ofbi->num_overlays) {
		r = -EINVAL;
		goto out;
	}

	if (changed) {
		for (i = 0; i < num_ovls; ++i)
			ofbi->rotation[i] = rotation[i];

		r = omapfb_apply_changes(fbi, 0);
		if (r)
			goto out;

		/* FIXME error handling? */
	}

	r = count;
out:
	unlock_fb_info(fbi);

	return r;
}

static ssize_t show_size(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);

	return snprintf(buf, PAGE_SIZE, "%lu\n", ofbi->region.size);
}

static ssize_t store_size(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);
	unsigned long size;
	int r;
	int i;

	size = PAGE_ALIGN(simple_strtoul(buf, NULL, 0));

	if (!lock_fb_info(fbi))
		return -ENODEV;

	for (i = 0; i < ofbi->num_overlays; i++) {
		if (ofbi->overlays[i]->info.enabled) {
			r = -EBUSY;
			goto out;
		}
	}

	if (size != ofbi->region.size) {
		r = omapfb_realloc_fbmem(fbi, size, ofbi->region.type);
		if (r) {
			dev_err(dev, "realloc fbmem failed\n");
			goto out;
		}
	}

	r = count;
out:
	unlock_fb_info(fbi);

	return r;
}

static ssize_t show_phys(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);

	return snprintf(buf, PAGE_SIZE, "%0x\n", ofbi->region.paddr);
}

static ssize_t show_virt(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fb_info *fbi = dev_get_drvdata(dev);
	struct omapfb_info *ofbi = FB2OFB(fbi);

	return snprintf(buf, PAGE_SIZE, "%p\n", ofbi->region.vaddr);
}

static struct device_attribute omapfb_attrs[] = {
	__ATTR(rotate_type, S_IRUGO | S_IWUSR, show_rotate_type,
			store_rotate_type),
	__ATTR(mirror, S_IRUGO | S_IWUSR, show_mirror, store_mirror),
	__ATTR(size, S_IRUGO | S_IWUSR, show_size, store_size),
	__ATTR(overlays, S_IRUGO | S_IWUSR, show_overlays, store_overlays),
	__ATTR(overlays_rotate, S_IRUGO | S_IWUSR, show_overlays_rotate,
			store_overlays_rotate),
	__ATTR(phys_addr, S_IRUGO, show_phys, NULL),
	__ATTR(virt_addr, S_IRUGO, show_virt, NULL),
};

int omapfb_create_sysfs(struct omapfb2_device *fbdev)
{
	int i;
	int r;

	DBG("create sysfs for fbs\n");
	for (i = 0; i < fbdev->num_fbs; i++) {
		int t;
		for (t = 0; t < ARRAY_SIZE(omapfb_attrs); t++) {
			r = device_create_file(fbdev->fbs[i]->dev,
					&omapfb_attrs[t]);

			if (r) {
				dev_err(fbdev->dev, "failed to create sysfs "
						"file\n");
				return r;
			}
		}
	}

	return 0;
}

void omapfb_remove_sysfs(struct omapfb2_device *fbdev)
{
	int i, t;

	DBG("remove sysfs for fbs\n");
	for (i = 0; i < fbdev->num_fbs; i++) {
		for (t = 0; t < ARRAY_SIZE(omapfb_attrs); t++)
			device_remove_file(fbdev->fbs[i]->dev,
					&omapfb_attrs[t]);
	}
}

