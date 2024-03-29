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

/* linux/arch/arm/mach-s3c2410/bast-ide.c
 *
 * Copyright 2007 Simtec Electronics
 *	http://www.simtec.co.uk/products/EB2410ITX/
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include <linux/platform_device.h>
#include <linux/ata_platform.h>

#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/map.h>
#include <mach/bast-map.h>
#include <mach/bast-irq.h>

/* IDE ports */

static struct pata_platform_info bast_ide_platdata = {
	.ioport_shift	= 5,
};

#define IDE_CS	S3C2410_CS5

static struct resource bast_ide0_resource[] = {
	[0]	= {
		.start	= IDE_CS + BAST_PA_IDEPRI,
		.end	= IDE_CS + BAST_PA_IDEPRI + (8 * 0x20) - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1]	= {
		.start	= IDE_CS + BAST_PA_IDEPRIAUX + (6 * 0x20) ,
		.end	= IDE_CS + BAST_PA_IDEPRIAUX + (7 * 0x20) - 1,
		.flags	= IORESOURCE_MEM,
	},
	[2]	= {
		.start	= IRQ_IDE0,
		.end	= IRQ_IDE0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device bast_device_ide0 = {
	.name		= "pata_platform",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(bast_ide0_resource),
	.resource	= bast_ide0_resource,
	.dev		= {
		.platform_data = &bast_ide_platdata,
		.coherent_dma_mask = ~0,
	}

};

static struct resource bast_ide1_resource[] = {
	[0]	= {
		.start	= IDE_CS + BAST_PA_IDESEC,
		.end	= IDE_CS + BAST_PA_IDESEC + (8 * 0x20) - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1]	= {
		.start	= IDE_CS + BAST_PA_IDESECAUX + (6 * 0x20),
		.end	= IDE_CS + BAST_PA_IDESECAUX + (7 * 0x20) - 1,
		.flags	= IORESOURCE_MEM,
	},
	[2]	= {
		.start	= IRQ_IDE1,
		.end	= IRQ_IDE1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device bast_device_ide1 = {
	.name		= "pata_platform",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(bast_ide1_resource),
	.resource	= bast_ide1_resource,
	.dev		= {
		.platform_data = &bast_ide_platdata,
		.coherent_dma_mask = ~0,
	}
};

static struct platform_device *bast_ide_devices[] __initdata = {
	&bast_device_ide0,
	&bast_device_ide1,
};

static __init int bast_ide_init(void)
{
	if (machine_is_bast() || machine_is_vr1000())
		return platform_add_devices(bast_ide_devices,
					    ARRAY_SIZE(bast_ide_devices));

	return 0;
}

fs_initcall(bast_ide_init);
