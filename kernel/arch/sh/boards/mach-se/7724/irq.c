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
 * linux/arch/sh/boards/se/7724/irq.c
 *
 * Copyright (C) 2009 Renesas Solutions Corp.
 *
 * Kuninori Morimoto <morimoto.kuninori@renesas.com>
 *
 * Based on  linux/arch/sh/boards/se/7722/irq.c
 * Copyright (C) 2007  Nobuhiro Iwamatsu
 *
 * Hitachi UL SolutionEngine 7724 Support.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach-se/mach/se7724.h>

struct fpga_irq {
	unsigned long  sraddr;
	unsigned long  mraddr;
	unsigned short mask;
	unsigned int   base;
};

static unsigned int fpga2irq(unsigned int irq)
{
	if (irq >= IRQ0_BASE &&
	    irq <= IRQ0_END)
		return IRQ0_IRQ;
	else if (irq >= IRQ1_BASE &&
		 irq <= IRQ1_END)
		return IRQ1_IRQ;
	else
		return IRQ2_IRQ;
}

static struct fpga_irq get_fpga_irq(unsigned int irq)
{
	struct fpga_irq set;

	switch (irq) {
	case IRQ0_IRQ:
		set.sraddr = IRQ0_SR;
		set.mraddr = IRQ0_MR;
		set.mask   = IRQ0_MASK;
		set.base   = IRQ0_BASE;
		break;
	case IRQ1_IRQ:
		set.sraddr = IRQ1_SR;
		set.mraddr = IRQ1_MR;
		set.mask   = IRQ1_MASK;
		set.base   = IRQ1_BASE;
		break;
	default:
		set.sraddr = IRQ2_SR;
		set.mraddr = IRQ2_MR;
		set.mask   = IRQ2_MASK;
		set.base   = IRQ2_BASE;
		break;
	}

	return set;
}

static void disable_se7724_irq(unsigned int irq)
{
	struct fpga_irq set = get_fpga_irq(fpga2irq(irq));
	unsigned int bit = irq - set.base;
	__raw_writew(__raw_readw(set.mraddr) | 0x0001 << bit, set.mraddr);
}

static void enable_se7724_irq(unsigned int irq)
{
	struct fpga_irq set = get_fpga_irq(fpga2irq(irq));
	unsigned int bit = irq - set.base;
	__raw_writew(__raw_readw(set.mraddr) & ~(0x0001 << bit), set.mraddr);
}

static struct irq_chip se7724_irq_chip __read_mostly = {
	.name           = "SE7724-FPGA",
	.mask           = disable_se7724_irq,
	.unmask         = enable_se7724_irq,
	.mask_ack       = disable_se7724_irq,
};

static void se7724_irq_demux(unsigned int irq, struct irq_desc *desc)
{
	struct fpga_irq set = get_fpga_irq(irq);
	unsigned short intv = __raw_readw(set.sraddr);
	unsigned int ext_irq = set.base;

	intv &= set.mask;

	for (; intv; intv >>= 1, ext_irq++) {
		if (!(intv & 1))
			continue;

		generic_handle_irq(ext_irq);
	}
}

/*
 * Initialize IRQ setting
 */
void __init init_se7724_IRQ(void)
{
	int i, nid = cpu_to_node(boot_cpu_data);

	__raw_writew(0xffff, IRQ0_MR);  /* mask all */
	__raw_writew(0xffff, IRQ1_MR);  /* mask all */
	__raw_writew(0xffff, IRQ2_MR);  /* mask all */
	__raw_writew(0x0000, IRQ0_SR);  /* clear irq */
	__raw_writew(0x0000, IRQ1_SR);  /* clear irq */
	__raw_writew(0x0000, IRQ2_SR);  /* clear irq */
	__raw_writew(0x002a, IRQ_MODE); /* set irq type */

	for (i = 0; i < SE7724_FPGA_IRQ_NR; i++) {
		int irq, wanted;

		wanted = SE7724_FPGA_IRQ_BASE + i;

		irq = create_irq_nr(wanted, nid);
		if (unlikely(irq == 0)) {
			pr_err("%s: failed hooking irq %d for FPGA\n",
			       __func__, wanted);
			return;
		}

		if (unlikely(irq != wanted)) {
			pr_err("%s: got irq %d but wanted %d, bailing.\n",
			       __func__, irq, wanted);
			destroy_irq(irq);
			return;
		}

		set_irq_chip_and_handler_name(irq,
					      &se7724_irq_chip,
					      handle_level_irq, "level");
	}

	set_irq_chained_handler(IRQ0_IRQ, se7724_irq_demux);
	set_irq_type(IRQ0_IRQ, IRQ_TYPE_LEVEL_LOW);

	set_irq_chained_handler(IRQ1_IRQ, se7724_irq_demux);
	set_irq_type(IRQ1_IRQ, IRQ_TYPE_LEVEL_LOW);

	set_irq_chained_handler(IRQ2_IRQ, se7724_irq_demux);
	set_irq_type(IRQ2_IRQ, IRQ_TYPE_LEVEL_LOW);
}
