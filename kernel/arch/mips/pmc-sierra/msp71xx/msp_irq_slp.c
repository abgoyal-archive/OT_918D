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
 * This file define the irq handler for MSP SLM subsystem interrupts.
 *
 * Copyright 2005-2006 PMC-Sierra, Inc, derived from irq_cpu.c
 * Author: Andrew Hughes, Andrew_Hughes@pmc-sierra.com
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/bitops.h>

#include <asm/mipsregs.h>
#include <asm/system.h>

#include <msp_slp_int.h>
#include <msp_regs.h>

static inline void unmask_msp_slp_irq(unsigned int irq)
{
	/* check for PER interrupt range */
	if (irq < MSP_PER_INTBASE)
		*SLP_INT_MSK_REG |= (1 << (irq - MSP_SLP_INTBASE));
	else
		*PER_INT_MSK_REG |= (1 << (irq - MSP_PER_INTBASE));
}

static inline void mask_msp_slp_irq(unsigned int irq)
{
	/* check for PER interrupt range */
	if (irq < MSP_PER_INTBASE)
		*SLP_INT_MSK_REG &= ~(1 << (irq - MSP_SLP_INTBASE));
	else
		*PER_INT_MSK_REG &= ~(1 << (irq - MSP_PER_INTBASE));
}

/*
 * While we ack the interrupt interrupts are disabled and thus we don't need
 * to deal with concurrency issues.  Same for msp_slp_irq_end.
 */
static inline void ack_msp_slp_irq(unsigned int irq)
{
	/* check for PER interrupt range */
	if (irq < MSP_PER_INTBASE)
		*SLP_INT_STS_REG = (1 << (irq - MSP_SLP_INTBASE));
	else
		*PER_INT_STS_REG = (1 << (irq - MSP_PER_INTBASE));
}

static struct irq_chip msp_slp_irq_controller = {
	.name = "MSP_SLP",
	.ack = ack_msp_slp_irq,
	.mask = mask_msp_slp_irq,
	.unmask = unmask_msp_slp_irq,
};

void __init msp_slp_irq_init(void)
{
	int i;

	/* Mask/clear interrupts. */
	*SLP_INT_MSK_REG = 0x00000000;
	*PER_INT_MSK_REG = 0x00000000;
	*SLP_INT_STS_REG = 0xFFFFFFFF;
	*PER_INT_STS_REG = 0xFFFFFFFF;

	/* initialize all the IRQ descriptors */
	for (i = MSP_SLP_INTBASE; i < MSP_PER_INTBASE + 32; i++)
		set_irq_chip_and_handler(i, &msp_slp_irq_controller,
					 handle_level_irq);
}

void msp_slp_irq_dispatch(void)
{
	u32 pending;
	int intbase;

	intbase = MSP_SLP_INTBASE;
	pending = *SLP_INT_STS_REG & *SLP_INT_MSK_REG;

	/* check for PER interrupt */
	if (pending == (1 << (MSP_INT_PER - MSP_SLP_INTBASE))) {
		intbase = MSP_PER_INTBASE;
		pending = *PER_INT_STS_REG & *PER_INT_MSK_REG;
	}

	/* check for spurious interrupt */
	if (pending == 0x00000000) {
		printk(KERN_ERR "Spurious %s interrupt?\n",
			(intbase == MSP_SLP_INTBASE) ? "SLP" : "PER");
		return;
	}

	/* dispatch the irq */
	do_IRQ(ffs(pending) + intbase - 1);
}
