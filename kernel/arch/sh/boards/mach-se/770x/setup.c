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
 * linux/arch/sh/boards/se/770x/setup.c
 *
 * Copyright (C) 2000  Kazumoto Kojima
 *
 * Hitachi SolutionEngine Support.
 *
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <mach-se/mach/se.h>
#include <mach-se/mach/mrshpc.h>
#include <asm/machvec.h>
#include <asm/io.h>
#include <asm/smc37c93x.h>
#include <asm/heartbeat.h>

/*
 * Configure the Super I/O chip
 */
static void __init smsc_config(int index, int data)
{
	outb_p(index, INDEX_PORT);
	outb_p(data, DATA_PORT);
}

/* XXX: Another candidate for a more generic cchip machine vector */
static void __init smsc_setup(char **cmdline_p)
{
	outb_p(CONFIG_ENTER, CONFIG_PORT);
	outb_p(CONFIG_ENTER, CONFIG_PORT);

	/* FDC */
	smsc_config(CURRENT_LDN_INDEX, LDN_FDC);
	smsc_config(ACTIVATE_INDEX, 0x01);
	smsc_config(IRQ_SELECT_INDEX, 6); /* IRQ6 */

	/* AUXIO (GPIO): to use IDE1 */
	smsc_config(CURRENT_LDN_INDEX, LDN_AUXIO);
	smsc_config(GPIO46_INDEX, 0x00); /* nIOROP */
	smsc_config(GPIO47_INDEX, 0x00); /* nIOWOP */

	/* COM1 */
	smsc_config(CURRENT_LDN_INDEX, LDN_COM1);
	smsc_config(ACTIVATE_INDEX, 0x01);
	smsc_config(IO_BASE_HI_INDEX, 0x03);
	smsc_config(IO_BASE_LO_INDEX, 0xf8);
	smsc_config(IRQ_SELECT_INDEX, 4); /* IRQ4 */

	/* COM2 */
	smsc_config(CURRENT_LDN_INDEX, LDN_COM2);
	smsc_config(ACTIVATE_INDEX, 0x01);
	smsc_config(IO_BASE_HI_INDEX, 0x02);
	smsc_config(IO_BASE_LO_INDEX, 0xf8);
	smsc_config(IRQ_SELECT_INDEX, 3); /* IRQ3 */

	/* RTC */
	smsc_config(CURRENT_LDN_INDEX, LDN_RTC);
	smsc_config(ACTIVATE_INDEX, 0x01);
	smsc_config(IRQ_SELECT_INDEX, 8); /* IRQ8 */

	/* XXX: PARPORT, KBD, and MOUSE will come here... */
	outb_p(CONFIG_EXIT, CONFIG_PORT);
}


static struct resource cf_ide_resources[] = {
	[0] = {
		.start  = PA_MRSHPC_IO + 0x1f0,
		.end    = PA_MRSHPC_IO + 0x1f0 + 8,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = PA_MRSHPC_IO + 0x1f0 + 0x206,
		.end    = PA_MRSHPC_IO + 0x1f0 + 8 + 0x206 + 8,
		.flags  = IORESOURCE_MEM,
	},
	[2] = {
		.start  = IRQ_CFCARD,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device cf_ide_device  = {
	.name           = "pata_platform",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(cf_ide_resources),
	.resource       = cf_ide_resources,
};

static unsigned char heartbeat_bit_pos[] = { 8, 9, 10, 11, 12, 13, 14, 15 };

static struct heartbeat_data heartbeat_data = {
	.bit_pos	= heartbeat_bit_pos,
	.nr_bits	= ARRAY_SIZE(heartbeat_bit_pos),
};

static struct resource heartbeat_resource = {
	.start	= PA_LED,
	.end	= PA_LED,
	.flags	= IORESOURCE_MEM | IORESOURCE_MEM_16BIT,
};

static struct platform_device heartbeat_device = {
	.name		= "heartbeat",
	.id		= -1,
	.dev	= {
		.platform_data	= &heartbeat_data,
	},
	.num_resources	= 1,
	.resource	= &heartbeat_resource,
};

#if defined(CONFIG_CPU_SUBTYPE_SH7710) ||\
	defined(CONFIG_CPU_SUBTYPE_SH7712)
/* SH771X Ethernet driver */
static struct resource sh_eth0_resources[] = {
	[0] = {
		.start = SH_ETH0_BASE,
		.end = SH_ETH0_BASE + 0x1B8,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = SH_ETH0_IRQ,
		.end = SH_ETH0_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device sh_eth0_device = {
	.name = "sh-eth",
	.id	= 0,
	.dev = {
		.platform_data = PHY_ID,
	},
	.num_resources = ARRAY_SIZE(sh_eth0_resources),
	.resource = sh_eth0_resources,
};

static struct resource sh_eth1_resources[] = {
	[0] = {
		.start = SH_ETH1_BASE,
		.end = SH_ETH1_BASE + 0x1B8,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = SH_ETH1_IRQ,
		.end = SH_ETH1_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device sh_eth1_device = {
	.name = "sh-eth",
	.id	= 1,
	.dev = {
		.platform_data = PHY_ID,
	},
	.num_resources = ARRAY_SIZE(sh_eth1_resources),
	.resource = sh_eth1_resources,
};
#endif

static struct platform_device *se_devices[] __initdata = {
	&heartbeat_device,
	&cf_ide_device,
#if defined(CONFIG_CPU_SUBTYPE_SH7710) ||\
	defined(CONFIG_CPU_SUBTYPE_SH7712)
	&sh_eth0_device,
	&sh_eth1_device,
#endif
};

static int __init se_devices_setup(void)
{
	mrshpc_setup_windows();
	return platform_add_devices(se_devices, ARRAY_SIZE(se_devices));
}
device_initcall(se_devices_setup);

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_se __initmv = {
	.mv_name		= "SolutionEngine",
	.mv_setup		= smsc_setup,
#if defined(CONFIG_CPU_SH4)
	.mv_nr_irqs		= 48,
#elif defined(CONFIG_CPU_SUBTYPE_SH7708)
	.mv_nr_irqs		= 32,
#elif defined(CONFIG_CPU_SUBTYPE_SH7709)
	.mv_nr_irqs		= 61,
#elif defined(CONFIG_CPU_SUBTYPE_SH7705)
	.mv_nr_irqs		= 86,
#elif defined(CONFIG_CPU_SUBTYPE_SH7710) || defined(CONFIG_CPU_SUBTYPE_SH7712)
	.mv_nr_irqs             = 104,
#endif

	.mv_inb			= se_inb,
	.mv_inw			= se_inw,
	.mv_inl			= se_inl,
	.mv_outb		= se_outb,
	.mv_outw		= se_outw,
	.mv_outl		= se_outl,

	.mv_inb_p		= se_inb_p,
	.mv_inw_p		= se_inw,
	.mv_inl_p		= se_inl,
	.mv_outb_p		= se_outb_p,
	.mv_outw_p		= se_outw,
	.mv_outl_p		= se_outl,

	.mv_insb		= se_insb,
	.mv_insw		= se_insw,
	.mv_insl		= se_insl,
	.mv_outsb		= se_outsb,
	.mv_outsw		= se_outsw,
	.mv_outsl		= se_outsl,

	.mv_init_irq		= init_se_IRQ,
};
