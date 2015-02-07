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
 * linux/arch/sh/boards/se/7343/irq.c
 *
 * Copyright (C) 2008  Yoshihiro Shimoda
 *
 * Based on linux/arch/sh/boards/se/7722/irq.c
 * Copyright (C) 2007  Nobuhiro Iwamatsu
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <mach-se/mach/se7343.h>

unsigned int se7343_fpga_irq[SE7343_FPGA_IRQ_NR] = { 0, };

static void disable_se7343_irq(unsigned int irq)
{
	unsigned int bit = (unsigned int)get_irq_chip_data(irq);
	__raw_writew(__raw_readw(PA_CPLD_IMSK) | 1 << bit, PA_CPLD_IMSK);
}

static void enable_se7343_irq(unsigned int irq)
{
	unsigned int bit = (unsigned int)get_irq_chip_data(irq);
	__raw_writew(__raw_readw(PA_CPLD_IMSK) & ~(1 << bit), PA_CPLD_IMSK);
}

static struct irq_chip se7343_irq_chip __read_mostly = {
	.name           = "SE7343-FPGA",
	.mask           = disable_se7343_irq,
	.unmask         = enable_se7343_irq,
	.mask_ack       = disable_se7343_irq,
};

static void se7343_irq_demux(unsigned int irq, struct irq_desc *desc)
{
	unsigned short intv = __raw_readw(PA_CPLD_ST);
	unsigned int ext_irq = 0;

	intv &= (1 << SE7343_FPGA_IRQ_NR) - 1;

	for (; intv; intv >>= 1, ext_irq++) {
		if (!(intv & 1))
			continue;

		generic_handle_irq(se7343_fpga_irq[ext_irq]);
	}
}

/*
 * Initialize IRQ setting
 */
void __init init_7343se_IRQ(void)
{
	int i, irq;

	__raw_writew(0, PA_CPLD_IMSK);	/* disable all irqs */
	__raw_writew(0x2000, 0xb03fffec);	/* mrshpc irq enable */

	for (i = 0; i < SE7343_FPGA_IRQ_NR; i++) {
		irq = create_irq();
		if (irq < 0)
			return;
		se7343_fpga_irq[i] = irq;

		set_irq_chip_and_handler_name(se7343_fpga_irq[i],
					      &se7343_irq_chip,
					      handle_level_irq, "level");

		set_irq_chip_data(se7343_fpga_irq[i], (void *)i);
	}

	set_irq_chained_handler(IRQ0_IRQ, se7343_irq_demux);
	set_irq_type(IRQ0_IRQ, IRQ_TYPE_LEVEL_LOW);
	set_irq_chained_handler(IRQ1_IRQ, se7343_irq_demux);
	set_irq_type(IRQ1_IRQ, IRQ_TYPE_LEVEL_LOW);
	set_irq_chained_handler(IRQ4_IRQ, se7343_irq_demux);
	set_irq_type(IRQ4_IRQ, IRQ_TYPE_LEVEL_LOW);
	set_irq_chained_handler(IRQ5_IRQ, se7343_irq_demux);
	set_irq_type(IRQ5_IRQ, IRQ_TYPE_LEVEL_LOW);
}
