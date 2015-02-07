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
 * TI Common Platform Interrupt Controller (cp_intc) driver
 *
 * Author: Steve Chen <schen@mvista.com>
 * Copyright (C) 2008-2009, MontaVista Software, Inc. <source@mvista.com>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/init.h>
#include <linux/irq.h>
#include <linux/io.h>

#include <mach/common.h>
#include <mach/cp_intc.h>

static inline unsigned int cp_intc_read(unsigned offset)
{
	return __raw_readl(davinci_intc_base + offset);
}

static inline void cp_intc_write(unsigned long value, unsigned offset)
{
	__raw_writel(value, davinci_intc_base + offset);
}

static void cp_intc_ack_irq(unsigned int irq)
{
	cp_intc_write(irq, CP_INTC_SYS_STAT_IDX_CLR);
}

/* Disable interrupt */
static void cp_intc_mask_irq(unsigned int irq)
{
	/* XXX don't know why we need to disable nIRQ here... */
	cp_intc_write(1, CP_INTC_HOST_ENABLE_IDX_CLR);
	cp_intc_write(irq, CP_INTC_SYS_ENABLE_IDX_CLR);
	cp_intc_write(1, CP_INTC_HOST_ENABLE_IDX_SET);
}

/* Enable interrupt */
static void cp_intc_unmask_irq(unsigned int irq)
{
	cp_intc_write(irq, CP_INTC_SYS_ENABLE_IDX_SET);
}

static int cp_intc_set_irq_type(unsigned int irq, unsigned int flow_type)
{
	unsigned reg		= BIT_WORD(irq);
	unsigned mask		= BIT_MASK(irq);
	unsigned polarity	= cp_intc_read(CP_INTC_SYS_POLARITY(reg));
	unsigned type		= cp_intc_read(CP_INTC_SYS_TYPE(reg));

	switch (flow_type) {
	case IRQ_TYPE_EDGE_RISING:
		polarity |= mask;
		type |= mask;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		polarity &= ~mask;
		type |= mask;
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		polarity |= mask;
		type &= ~mask;
		break;
	case IRQ_TYPE_LEVEL_LOW:
		polarity &= ~mask;
		type &= ~mask;
		break;
	default:
		return -EINVAL;
	}

	cp_intc_write(polarity, CP_INTC_SYS_POLARITY(reg));
	cp_intc_write(type, CP_INTC_SYS_TYPE(reg));

	return 0;
}

/*
 * Faking this allows us to to work with suspend functions of
 * generic drivers which call {enable|disable}_irq_wake for
 * wake up interrupt sources (eg RTC on DA850).
 */
static int cp_intc_set_wake(unsigned int irq, unsigned int on)
{
	return 0;
}

static struct irq_chip cp_intc_irq_chip = {
	.name		= "cp_intc",
	.ack		= cp_intc_ack_irq,
	.mask		= cp_intc_mask_irq,
	.unmask		= cp_intc_unmask_irq,
	.set_type	= cp_intc_set_irq_type,
	.set_wake	= cp_intc_set_wake,
};

void __init cp_intc_init(void)
{
	unsigned long num_irq	= davinci_soc_info.intc_irq_num;
	u8 *irq_prio		= davinci_soc_info.intc_irq_prios;
	u32 *host_map		= davinci_soc_info.intc_host_map;
	unsigned num_reg	= BITS_TO_LONGS(num_irq);
	int i;

	davinci_intc_type = DAVINCI_INTC_TYPE_CP_INTC;
	davinci_intc_base = ioremap(davinci_soc_info.intc_base, SZ_8K);
	if (WARN_ON(!davinci_intc_base))
		return;

	cp_intc_write(0, CP_INTC_GLOBAL_ENABLE);

	/* Disable all host interrupts */
	cp_intc_write(0, CP_INTC_HOST_ENABLE(0));

	/* Disable system interrupts */
	for (i = 0; i < num_reg; i++)
		cp_intc_write(~0, CP_INTC_SYS_ENABLE_CLR(i));

	/* Set to normal mode, no nesting, no priority hold */
	cp_intc_write(0, CP_INTC_CTRL);
	cp_intc_write(0, CP_INTC_HOST_CTRL);

	/* Clear system interrupt status */
	for (i = 0; i < num_reg; i++)
		cp_intc_write(~0, CP_INTC_SYS_STAT_CLR(i));

	/* Enable nIRQ (what about nFIQ?) */
	cp_intc_write(1, CP_INTC_HOST_ENABLE_IDX_SET);

	/*
	 * Priority is determined by host channel: lower channel number has
	 * higher priority i.e. channel 0 has highest priority and channel 31
	 * had the lowest priority.
	 */
	num_reg = (num_irq + 3) >> 2;	/* 4 channels per register */
	if (irq_prio) {
		unsigned j, k;
		u32 val;

		for (k = i = 0; i < num_reg; i++) {
			for (val = j = 0; j < 4; j++, k++) {
				val >>= 8;
				if (k < num_irq)
					val |= irq_prio[k] << 24;
			}

			cp_intc_write(val, CP_INTC_CHAN_MAP(i));
		}
	} else	{
		/*
		 * Default everything to channel 15 if priority not specified.
		 * Note that channel 0-1 are mapped to nFIQ and channels 2-31
		 * are mapped to nIRQ.
		 */
		for (i = 0; i < num_reg; i++)
			cp_intc_write(0x0f0f0f0f, CP_INTC_CHAN_MAP(i));
	}

	if (host_map)
		for (i = 0; host_map[i] != -1; i++)
			cp_intc_write(host_map[i], CP_INTC_HOST_MAP(i));

	/* Set up genirq dispatching for cp_intc */
	for (i = 0; i < num_irq; i++) {
		set_irq_chip(i, &cp_intc_irq_chip);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
		set_irq_handler(i, handle_edge_irq);
	}

	/* Enable global interrupt */
	cp_intc_write(1, CP_INTC_GLOBAL_ENABLE);
}