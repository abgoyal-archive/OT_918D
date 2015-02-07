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
 *
 * Backlight driver for HP Jornada 700 series (710/720/728)
 * Copyright (C) 2006-2009 Kristoffer Ericson <kristoffer.ericson@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 or any later version as published by the Free Software Foundation.
 *
 */

#include <linux/backlight.h>
#include <linux/device.h>
#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <mach/jornada720.h>
#include <mach/hardware.h>

#include <video/s1d13xxxfb.h>

#define BL_MAX_BRIGHT	255
#define BL_DEF_BRIGHT	25

static int jornada_bl_get_brightness(struct backlight_device *bd)
{
	int ret;

	/* check if backlight is on */
	if (!(PPSR & PPC_LDD1))
		return 0;

	jornada_ssp_start();

	/* cmd should return txdummy */
	ret = jornada_ssp_byte(GETBRIGHTNESS);

	if (jornada_ssp_byte(GETBRIGHTNESS) != TXDUMMY) {
		printk(KERN_ERR "bl : get brightness timeout\n");
		jornada_ssp_end();
		return -ETIMEDOUT;
	} else /* exchange txdummy for value */
		ret = jornada_ssp_byte(TXDUMMY);

	jornada_ssp_end();

	return (BL_MAX_BRIGHT - ret);
}

static int jornada_bl_update_status(struct backlight_device *bd)
{
	int ret = 0;

	jornada_ssp_start();

	/* If backlight is off then really turn it off */
	if ((bd->props.power != FB_BLANK_UNBLANK) || (bd->props.fb_blank != FB_BLANK_UNBLANK)) {
		ret = jornada_ssp_byte(BRIGHTNESSOFF);
		if (ret != TXDUMMY) {
			printk(KERN_INFO "bl : brightness off timeout\n");
			/* turn off backlight */
			PPSR &= ~PPC_LDD1;
			PPDR |= PPC_LDD1;
			ret = -ETIMEDOUT;
		}
	} else  /* turn on backlight */
		PPSR |= PPC_LDD1;

		/* send command to our mcu */
		if (jornada_ssp_byte(SETBRIGHTNESS) != TXDUMMY) {
			printk(KERN_INFO "bl : failed to set brightness\n");
			ret = -ETIMEDOUT;
			goto out;
		}

		/* at this point we expect that the mcu has accepted
		   our command and is waiting for our new value
		   please note that maximum brightness is 255,
		   but due to physical layout it is equal to 0, so we simply
		   invert the value (MAX VALUE - NEW VALUE). */
		if (jornada_ssp_byte(BL_MAX_BRIGHT - bd->props.brightness) != TXDUMMY) {
			printk(KERN_ERR "bl : set brightness failed\n");
			ret = -ETIMEDOUT;
		}

		/* If infact we get an TXDUMMY as output we are happy and dont
		   make any further comments about it */
out:
	jornada_ssp_end();

	return ret;
}

static const struct backlight_ops jornada_bl_ops = {
	.get_brightness = jornada_bl_get_brightness,
	.update_status = jornada_bl_update_status,
	.options = BL_CORE_SUSPENDRESUME,
};

static int jornada_bl_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	int ret;
	struct backlight_device *bd;

	memset(&props, 0, sizeof(struct backlight_properties));
	props.max_brightness = BL_MAX_BRIGHT;
	bd = backlight_device_register(S1D_DEVICENAME, &pdev->dev, NULL,
				       &jornada_bl_ops, &props);

	if (IS_ERR(bd)) {
		ret = PTR_ERR(bd);
		printk(KERN_ERR "bl : failed to register device, err=%x\n", ret);
		return ret;
	}

	bd->props.power = FB_BLANK_UNBLANK;
	bd->props.brightness = BL_DEF_BRIGHT;
	/* note. make sure max brightness is set otherwise
	   you will get seemingly non-related errors when
	   trying to change brightness */
	jornada_bl_update_status(bd);

	platform_set_drvdata(pdev, bd);
	printk(KERN_INFO "HP Jornada 700 series backlight driver\n");

	return 0;
}

static int jornada_bl_remove(struct platform_device *pdev)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);

	backlight_device_unregister(bd);

	return 0;
}

static struct platform_driver jornada_bl_driver = {
	.probe		= jornada_bl_probe,
	.remove		= jornada_bl_remove,
	.driver	= {
		.name	= "jornada_bl",
	},
};

int __init jornada_bl_init(void)
{
	return platform_driver_register(&jornada_bl_driver);
}

void __exit jornada_bl_exit(void)
{
	platform_driver_unregister(&jornada_bl_driver);
}

MODULE_AUTHOR("Kristoffer Ericson <kristoffer.ericson>");
MODULE_DESCRIPTION("HP Jornada 710/720/728 Backlight driver");
MODULE_LICENSE("GPL");

module_init(jornada_bl_init);
module_exit(jornada_bl_exit);
