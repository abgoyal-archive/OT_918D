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

/*  linux/arch/arm/mach-pxa/xcep.c
 *
 *  Support for the Iskratel Electronics XCEP platform as used in
 *  the Libera instruments from Instrumentation Technologies.
 *
 *  Author:     Ales Bardorfer <ales@i-tech.si>
 *  Contributions by: Abbott, MG (Michael) <michael.abbott@diamond.ac.uk>
 *  Contributions by: Matej Kenda <matej.kenda@i-tech.si>
 *  Created:    June 2006
 *  Copyright:  (C) 2006-2009 Instrumentation Technologies
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/smc91x.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>

#include <plat/i2c.h>

#include <mach/hardware.h>
#include <mach/pxa2xx-regs.h>
#include <mach/mfp-pxa25x.h>

#include "generic.h"

#define XCEP_ETH_PHYS		(PXA_CS3_PHYS + 0x00000300)
#define XCEP_ETH_PHYS_END	(PXA_CS3_PHYS + 0x000fffff)
#define XCEP_ETH_ATTR		(PXA_CS3_PHYS + 0x02000000)
#define XCEP_ETH_ATTR_END	(PXA_CS3_PHYS + 0x020fffff)
#define XCEP_ETH_IRQ		IRQ_GPIO0

/*  XCEP CPLD base */
#define XCEP_CPLD_BASE		0xf0000000


/* Flash partitions. */

static struct mtd_partition xcep_partitions[] = {
	{
		.name =		"Bootloader",
		.size =		0x00040000,
		.offset =	0,
		.mask_flags =	MTD_WRITEABLE
	}, {
		.name =		"Bootloader ENV",
		.size =		0x00040000,
		.offset =	0x00040000,
		.mask_flags =	MTD_WRITEABLE
	}, {
		.name =		"Kernel",
		.size =		0x00100000,
		.offset =	0x00080000,
	}, {
		.name =		"Rescue fs",
		.size =		0x00280000,
		.offset =	0x00180000,
	}, {
		.name =		"Filesystem",
		.size =		MTDPART_SIZ_FULL,
		.offset =	0x00400000
	}
};

static struct physmap_flash_data xcep_flash_data[] = {
	{
		.width		= 4,		/* bankwidth in bytes */
		.parts		= xcep_partitions,
		.nr_parts	= ARRAY_SIZE(xcep_partitions)
	}
};

static struct resource flash_resource = {
	.start	= PXA_CS0_PHYS,
	.end	= PXA_CS0_PHYS + SZ_32M - 1,
	.flags	= IORESOURCE_MEM,
};

static struct platform_device flash_device = {
	.name	= "physmap-flash",
	.id	= 0,
	.dev 	= {
		.platform_data = xcep_flash_data,
	},
	.resource = &flash_resource,
	.num_resources = 1,
};



/* SMC LAN91C111 network controller. */

static struct resource smc91x_resources[] = {
	[0] = {
		.name	= "smc91x-regs",
		.start	= XCEP_ETH_PHYS,
		.end	= XCEP_ETH_PHYS_END,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= XCEP_ETH_IRQ,
		.end	= XCEP_ETH_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.name	= "smc91x-attrib",
		.start	= XCEP_ETH_ATTR,
		.end	= XCEP_ETH_ATTR_END,
		.flags	= IORESOURCE_MEM,
	},
};

static struct smc91x_platdata xcep_smc91x_info = {
	.flags	= SMC91X_USE_32BIT | SMC91X_NOWAIT | SMC91X_USE_DMA,
};

static struct platform_device smc91x_device = {
	.name		= "smc91x",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(smc91x_resources),
	.resource	= smc91x_resources,
	.dev		= {
		.platform_data = &xcep_smc91x_info,
	},
};


static struct platform_device *devices[] __initdata = {
	&flash_device,
	&smc91x_device,
};


/* We have to state that there are HWMON devices on the I2C bus on XCEP.
 * Drivers for HWMON verify capabilities of the adapter when loading and
 * refuse to attach if the adapter doesn't support HWMON class of devices.
 * See also Documentation/i2c/porting-clients. */
static struct i2c_pxa_platform_data xcep_i2c_platform_data  = {
	.class = I2C_CLASS_HWMON
};


static mfp_cfg_t xcep_pin_config[] __initdata = {
	GPIO79_nCS_3,	/* SMC 91C111 chip select. */
	GPIO80_nCS_4,	/* CPLD chip select. */
	/* SSP communication to MSP430 */
	GPIO23_SSP1_SCLK,
	GPIO24_SSP1_SFRM,
	GPIO25_SSP1_TXD,
	GPIO26_SSP1_RXD,
	GPIO27_SSP1_EXTCLK
};

static void __init xcep_init(void)
{
	pxa2xx_mfp_config(ARRAY_AND_SIZE(xcep_pin_config));

	pxa_set_ffuart_info(NULL);
	pxa_set_btuart_info(NULL);
	pxa_set_stuart_info(NULL);
	pxa_set_hwuart_info(NULL);

	/* See Intel XScale Developer's Guide for details */
	/* Set RDF and RDN to appropriate values (chip select 3 (smc91x)) */
	MSC1 = (MSC1 & 0xffff) | 0xD5540000;
	/* Set RDF and RDN to appropriate values (chip select 5 (fpga)) */
	MSC2 = (MSC2 & 0xffff) | 0x72A00000;

	platform_add_devices(ARRAY_AND_SIZE(devices));
	pxa_set_i2c_info(&xcep_i2c_platform_data);
}

MACHINE_START(XCEP, "Iskratel XCEP")
	.phys_io	= 0x40000000,
	.io_pg_offst	= (io_p2v(0x40000000) >> 18) & 0xfffc,
	.boot_params	= 0xa0000100,
	.init_machine	= xcep_init,
	.map_io		= pxa_map_io,
	.init_irq	= pxa25x_init_irq,
	.timer		= &pxa_timer,
MACHINE_END

