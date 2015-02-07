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
 * arch/powerpc/platforms/embedded6xx/hlwd-pic.c
 *
 * Nintendo Wii "Hollywood" interrupt controller support.
 * Copyright (C) 2009 The GameCube Linux Team
 * Copyright (C) 2009 Albert Herranz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 */
#define DRV_MODULE_NAME "hlwd-pic"
#define pr_fmt(fmt) DRV_MODULE_NAME ": " fmt

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/of.h>
#include <asm/io.h>

#include "hlwd-pic.h"

#define HLWD_NR_IRQS	32

/*
 * Each interrupt has a corresponding bit in both
 * the Interrupt Cause (ICR) and Interrupt Mask (IMR) registers.
 *
 * Enabling/disabling an interrupt line involves asserting/clearing
 * the corresponding bit in IMR. ACK'ing a request simply involves
 * asserting the corresponding bit in ICR.
 */
#define HW_BROADWAY_ICR		0x00
#define HW_BROADWAY_IMR		0x04


/*
 * IRQ chip hooks.
 *
 */

static void hlwd_pic_mask_and_ack(unsigned int virq)
{
	int irq = virq_to_hw(virq);
	void __iomem *io_base = get_irq_chip_data(virq);
	u32 mask = 1 << irq;

	clrbits32(io_base + HW_BROADWAY_IMR, mask);
	out_be32(io_base + HW_BROADWAY_ICR, mask);
}

static void hlwd_pic_ack(unsigned int virq)
{
	int irq = virq_to_hw(virq);
	void __iomem *io_base = get_irq_chip_data(virq);

	out_be32(io_base + HW_BROADWAY_ICR, 1 << irq);
}

static void hlwd_pic_mask(unsigned int virq)
{
	int irq = virq_to_hw(virq);
	void __iomem *io_base = get_irq_chip_data(virq);

	clrbits32(io_base + HW_BROADWAY_IMR, 1 << irq);
}

static void hlwd_pic_unmask(unsigned int virq)
{
	int irq = virq_to_hw(virq);
	void __iomem *io_base = get_irq_chip_data(virq);

	setbits32(io_base + HW_BROADWAY_IMR, 1 << irq);
}


static struct irq_chip hlwd_pic = {
	.name		= "hlwd-pic",
	.ack		= hlwd_pic_ack,
	.mask_ack	= hlwd_pic_mask_and_ack,
	.mask		= hlwd_pic_mask,
	.unmask		= hlwd_pic_unmask,
};

/*
 * IRQ host hooks.
 *
 */

static struct irq_host *hlwd_irq_host;

static int hlwd_pic_map(struct irq_host *h, unsigned int virq,
			   irq_hw_number_t hwirq)
{
	set_irq_chip_data(virq, h->host_data);
	irq_to_desc(virq)->status |= IRQ_LEVEL;
	set_irq_chip_and_handler(virq, &hlwd_pic, handle_level_irq);
	return 0;
}

static void hlwd_pic_unmap(struct irq_host *h, unsigned int irq)
{
	set_irq_chip_data(irq, NULL);
	set_irq_chip(irq, NULL);
}

static struct irq_host_ops hlwd_irq_host_ops = {
	.map = hlwd_pic_map,
	.unmap = hlwd_pic_unmap,
};

static unsigned int __hlwd_pic_get_irq(struct irq_host *h)
{
	void __iomem *io_base = h->host_data;
	int irq;
	u32 irq_status;

	irq_status = in_be32(io_base + HW_BROADWAY_ICR) &
		     in_be32(io_base + HW_BROADWAY_IMR);
	if (irq_status == 0)
		return NO_IRQ;	/* no more IRQs pending */

	irq = __ffs(irq_status);
	return irq_linear_revmap(h, irq);
}

static void hlwd_pic_irq_cascade(unsigned int cascade_virq,
				      struct irq_desc *desc)
{
	struct irq_host *irq_host = get_irq_data(cascade_virq);
	unsigned int virq;

	raw_spin_lock(&desc->lock);
	desc->chip->mask(cascade_virq); /* IRQ_LEVEL */
	raw_spin_unlock(&desc->lock);

	virq = __hlwd_pic_get_irq(irq_host);
	if (virq != NO_IRQ)
		generic_handle_irq(virq);
	else
		pr_err("spurious interrupt!\n");

	raw_spin_lock(&desc->lock);
	desc->chip->ack(cascade_virq); /* IRQ_LEVEL */
	if (!(desc->status & IRQ_DISABLED) && desc->chip->unmask)
		desc->chip->unmask(cascade_virq);
	raw_spin_unlock(&desc->lock);
}

/*
 * Platform hooks.
 *
 */

static void __hlwd_quiesce(void __iomem *io_base)
{
	/* mask and ack all IRQs */
	out_be32(io_base + HW_BROADWAY_IMR, 0);
	out_be32(io_base + HW_BROADWAY_ICR, 0xffffffff);
}

struct irq_host *hlwd_pic_init(struct device_node *np)
{
	struct irq_host *irq_host;
	struct resource res;
	void __iomem *io_base;
	int retval;

	retval = of_address_to_resource(np, 0, &res);
	if (retval) {
		pr_err("no io memory range found\n");
		return NULL;
	}
	io_base = ioremap(res.start, resource_size(&res));
	if (!io_base) {
		pr_err("ioremap failed\n");
		return NULL;
	}

	pr_info("controller at 0x%08x mapped to 0x%p\n", res.start, io_base);

	__hlwd_quiesce(io_base);

	irq_host = irq_alloc_host(np, IRQ_HOST_MAP_LINEAR, HLWD_NR_IRQS,
				  &hlwd_irq_host_ops, -1);
	if (!irq_host) {
		pr_err("failed to allocate irq_host\n");
		return NULL;
	}
	irq_host->host_data = io_base;

	return irq_host;
}

unsigned int hlwd_pic_get_irq(void)
{
	return __hlwd_pic_get_irq(hlwd_irq_host);
}

/*
 * Probe function.
 *
 */

void hlwd_pic_probe(void)
{
	struct irq_host *host;
	struct device_node *np;
	const u32 *interrupts;
	int cascade_virq;

	for_each_compatible_node(np, NULL, "nintendo,hollywood-pic") {
		interrupts = of_get_property(np, "interrupts", NULL);
		if (interrupts) {
			host = hlwd_pic_init(np);
			BUG_ON(!host);
			cascade_virq = irq_of_parse_and_map(np, 0);
			set_irq_data(cascade_virq, host);
			set_irq_chained_handler(cascade_virq,
						hlwd_pic_irq_cascade);
			hlwd_irq_host = host;
			break;
		}
	}
}

/**
 * hlwd_quiesce() - quiesce hollywood irq controller
 *
 * Mask and ack all interrupt sources.
 *
 */
void hlwd_quiesce(void)
{
	void __iomem *io_base = hlwd_irq_host->host_data;

	__hlwd_quiesce(io_base);
}

