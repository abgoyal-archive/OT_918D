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
 * linux/arch/arm/mach-h720x/h7202-eval.c
 *
 * Copyright (C) 2003 Thomas Gleixner <tglx@linutronix.de>
 *               2003 Robert Schwebel <r.schwebel@pengutronix.de>
 *		 2004 Sascha Hauer <s.hauer@pengutronix.de>
 *
 * Architecture specific stuff for Hynix HMS30C7202 development board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/platform_device.h>

#include <asm/setup.h>
#include <asm/types.h>
#include <asm/mach-types.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mach/arch.h>
#include <mach/irqs.h>
#include <mach/hardware.h>
#include "common.h"

static struct resource cirrus_resources[] = {
	[0] = {
		.start	= ETH0_PHYS + 0x300,
		.end	= ETH0_PHYS + 0x300 + 0x10,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_CHAINED_GPIOB(8),
		.end	= IRQ_CHAINED_GPIOB(8),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device cirrus_device = {
	.name		= "cirrus-cs89x0",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(cirrus_resources),
	.resource	= cirrus_resources,
};

static struct platform_device *devices[] __initdata = {
	&cirrus_device,
};

/*
 * Hardware init. This is called early in initcalls
 * Place pin inits here. So you avoid adding ugly
 * #ifdef stuff to common drivers.
 * Use this only, if your bootloader is not able
 * to initialize the pins proper.
 */
static void __init init_eval_h7202(void)
{
	init_hw_h7202();
	(void) platform_add_devices(devices, ARRAY_SIZE(devices));

	/* Enable interrupt on portb bit 8 (ethernet) */
	CPU_REG (GPIO_B_VIRT, GPIO_POL) &= ~(1 << 8);
	CPU_REG (GPIO_B_VIRT, GPIO_EN) |= (1 << 8);
}

MACHINE_START(H7202, "Hynix HMS30C7202")
	/* Maintainer: Robert Schwebel, Pengutronix */
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf0000000) >> 18) & 0xfffc,
	.boot_params	= 0x40000100,
	.map_io		= h720x_map_io,
	.init_irq	= h7202_init_irq,
	.timer		= &h7202_timer,
	.init_machine	= init_eval_h7202,
MACHINE_END
