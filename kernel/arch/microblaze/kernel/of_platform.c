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
 *    Copyright (C) 2006 Benjamin Herrenschmidt, IBM Corp.
 *			 <benh@kernel.crashing.org>
 *    and		 Arnd Bergmann, IBM Corp.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 */

#undef DEBUG

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/pci.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include <linux/errno.h>
#include <linux/topology.h>
#include <asm/atomic.h>

struct bus_type of_platform_bus_type = {
       .uevent	= of_device_uevent,
};
EXPORT_SYMBOL(of_platform_bus_type);

static int __init of_bus_driver_init(void)
{
	return of_bus_type_init(&of_platform_bus_type, "of_platform");
}
postcore_initcall(of_bus_driver_init);

struct of_device *of_platform_device_create(struct device_node *np,
					    const char *bus_id,
					    struct device *parent)
{
	struct of_device *dev;

	dev = of_device_alloc(np, bus_id, parent);
	if (!dev)
		return NULL;

	dev->archdata.dma_mask = 0xffffffffUL;
	dev->dev.bus = &of_platform_bus_type;

	/* We do not fill the DMA ops for platform devices by default.
	 * This is currently the responsibility of the platform code
	 * to do such, possibly using a device notifier
	 */

	if (of_device_register(dev) != 0) {
		of_device_free(dev);
		return NULL;
	}

	return dev;
}
EXPORT_SYMBOL(of_platform_device_create);

/**
 * of_platform_bus_create - Create an OF device for a bus node and all its
 * children. Optionally recursively instanciate matching busses.
 * @bus: device node of the bus to instanciate
 * @matches: match table, NULL to use the default, OF_NO_DEEP_PROBE to
 * disallow recursive creation of child busses
 */
static int of_platform_bus_create(const struct device_node *bus,
				  const struct of_device_id *matches,
				  struct device *parent)
{
	struct device_node *child;
	struct of_device *dev;
	int rc = 0;

	for_each_child_of_node(bus, child) {
		pr_debug("   create child: %s\n", child->full_name);
		dev = of_platform_device_create(child, NULL, parent);
		if (dev == NULL)
			rc = -ENOMEM;
		else if (!of_match_node(matches, child))
			continue;
		if (rc == 0) {
			pr_debug("   and sub busses\n");
			rc = of_platform_bus_create(child, matches, &dev->dev);
		}
		if (rc) {
			of_node_put(child);
			break;
		}
	}
	return rc;
}


/**
 * of_platform_bus_probe - Probe the device-tree for platform busses
 * @root: parent of the first level to probe or NULL for the root of the tree
 * @matches: match table, NULL to use the default
 * @parent: parent to hook devices from, NULL for toplevel
 *
 * Note that children of the provided root are not instanciated as devices
 * unless the specified root itself matches the bus list and is not NULL.
 */

int of_platform_bus_probe(struct device_node *root,
			  const struct of_device_id *matches,
			  struct device *parent)
{
	struct device_node *child;
	struct of_device *dev;
	int rc = 0;

	if (matches == NULL)
		matches = of_default_bus_ids;
	if (matches == OF_NO_DEEP_PROBE)
		return -EINVAL;
	if (root == NULL)
		root = of_find_node_by_path("/");
	else
		of_node_get(root);

	pr_debug("of_platform_bus_probe()\n");
	pr_debug(" starting at: %s\n", root->full_name);

	/* Do a self check of bus type, if there's a match, create
	 * children
	 */
	if (of_match_node(matches, root)) {
		pr_debug(" root match, create all sub devices\n");
		dev = of_platform_device_create(root, NULL, parent);
		if (dev == NULL) {
			rc = -ENOMEM;
			goto bail;
		}
		pr_debug(" create all sub busses\n");
		rc = of_platform_bus_create(root, matches, &dev->dev);
		goto bail;
	}
	for_each_child_of_node(root, child) {
		if (!of_match_node(matches, child))
			continue;

		pr_debug("  match: %s\n", child->full_name);
		dev = of_platform_device_create(child, NULL, parent);
		if (dev == NULL)
			rc = -ENOMEM;
		else
			rc = of_platform_bus_create(child, matches, &dev->dev);
		if (rc) {
			of_node_put(child);
			break;
		}
	}
 bail:
	of_node_put(root);
	return rc;
}
EXPORT_SYMBOL(of_platform_bus_probe);

static int of_dev_node_match(struct device *dev, void *data)
{
	return to_of_device(dev)->dev.of_node == data;
}

struct of_device *of_find_device_by_node(struct device_node *np)
{
	struct device *dev;

	dev = bus_find_device(&of_platform_bus_type,
			      NULL, np, of_dev_node_match);
	if (dev)
		return to_of_device(dev);
	return NULL;
}
EXPORT_SYMBOL(of_find_device_by_node);

static int of_dev_phandle_match(struct device *dev, void *data)
{
	phandle *ph = data;
	return to_of_device(dev)->dev.of_node->phandle == *ph;
}

struct of_device *of_find_device_by_phandle(phandle ph)
{
	struct device *dev;

	dev = bus_find_device(&of_platform_bus_type,
			      NULL, &ph, of_dev_phandle_match);
	if (dev)
		return to_of_device(dev);
	return NULL;
}
EXPORT_SYMBOL(of_find_device_by_phandle);
