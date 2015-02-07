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

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/errno.h>
#include <linux/irq.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include "of_device_common.h"

static int node_match(struct device *dev, void *data)
{
	struct of_device *op = to_of_device(dev);
	struct device_node *dp = data;

	return (op->dev.of_node == dp);
}

struct of_device *of_find_device_by_node(struct device_node *dp)
{
	struct device *dev = bus_find_device(&of_platform_bus_type, NULL,
					     dp, node_match);

	if (dev)
		return to_of_device(dev);

	return NULL;
}
EXPORT_SYMBOL(of_find_device_by_node);

unsigned int irq_of_parse_and_map(struct device_node *node, int index)
{
	struct of_device *op = of_find_device_by_node(node);

	if (!op || index >= op->num_irqs)
		return 0;

	return op->irqs[index];
}
EXPORT_SYMBOL(irq_of_parse_and_map);

/* Take the archdata values for IOMMU, STC, and HOSTDATA found in
 * BUS and propagate to all child of_device objects.
 */
void of_propagate_archdata(struct of_device *bus)
{
	struct dev_archdata *bus_sd = &bus->dev.archdata;
	struct device_node *bus_dp = bus->dev.of_node;
	struct device_node *dp;

	for (dp = bus_dp->child; dp; dp = dp->sibling) {
		struct of_device *op = of_find_device_by_node(dp);

		op->dev.archdata.iommu = bus_sd->iommu;
		op->dev.archdata.stc = bus_sd->stc;
		op->dev.archdata.host_controller = bus_sd->host_controller;
		op->dev.archdata.numa_node = bus_sd->numa_node;

		if (dp->child)
			of_propagate_archdata(op);
	}
}

struct bus_type of_platform_bus_type;
EXPORT_SYMBOL(of_platform_bus_type);

static void get_cells(struct device_node *dp, int *addrc, int *sizec)
{
	if (addrc)
		*addrc = of_n_addr_cells(dp);
	if (sizec)
		*sizec = of_n_size_cells(dp);
}

/*
 * Default translator (generic bus)
 */

void of_bus_default_count_cells(struct device_node *dev, int *addrc, int *sizec)
{
	get_cells(dev, addrc, sizec);
}

/* Make sure the least significant 64-bits are in-range.  Even
 * for 3 or 4 cell values it is a good enough approximation.
 */
int of_out_of_range(const u32 *addr, const u32 *base,
		    const u32 *size, int na, int ns)
{
	u64 a = of_read_addr(addr, na);
	u64 b = of_read_addr(base, na);

	if (a < b)
		return 1;

	b += of_read_addr(size, ns);
	if (a >= b)
		return 1;

	return 0;
}

int of_bus_default_map(u32 *addr, const u32 *range, int na, int ns, int pna)
{
	u32 result[OF_MAX_ADDR_CELLS];
	int i;

	if (ns > 2) {
		printk("of_device: Cannot handle size cells (%d) > 2.", ns);
		return -EINVAL;
	}

	if (of_out_of_range(addr, range, range + na + pna, na, ns))
		return -EINVAL;

	/* Start with the parent range base.  */
	memcpy(result, range + na, pna * 4);

	/* Add in the child address offset.  */
	for (i = 0; i < na; i++)
		result[pna - 1 - i] +=
			(addr[na - 1 - i] -
			 range[na - 1 - i]);

	memcpy(addr, result, pna * 4);

	return 0;
}

unsigned long of_bus_default_get_flags(const u32 *addr, unsigned long flags)
{
	if (flags)
		return flags;
	return IORESOURCE_MEM;
}

/*
 * SBUS bus specific translator
 */

int of_bus_sbus_match(struct device_node *np)
{
	struct device_node *dp = np;

	while (dp) {
		if (!strcmp(dp->name, "sbus") ||
		    !strcmp(dp->name, "sbi"))
			return 1;

		/* Have a look at use_1to1_mapping().  We're trying
		 * to match SBUS if that's the top-level bus and we
		 * don't have some intervening real bus that provides
		 * ranges based translations.
		 */
		if (of_find_property(dp, "ranges", NULL) != NULL)
			break;

		dp = dp->parent;
	}

	return 0;
}

void of_bus_sbus_count_cells(struct device_node *child, int *addrc, int *sizec)
{
	if (addrc)
		*addrc = 2;
	if (sizec)
		*sizec = 1;
}