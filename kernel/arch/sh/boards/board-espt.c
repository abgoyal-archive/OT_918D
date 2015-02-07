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
 * Data Technology Inc. ESPT-GIGA board suport
 *
 * Copyright (C) 2008, 2009 Renesas Solutions Corp.
 * Copyright (C) 2008, 2009 Nobuhiro Iwamatsu <iwamatsu.nobuhiro@renesas.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/mtd/physmap.h>
#include <linux/io.h>
#include <asm/machvec.h>
#include <asm/sizes.h>
#include <asm/sh_eth.h>

/* NOR Flash */
static struct mtd_partition espt_nor_flash_partitions[] = {
	{
		.name = "U-Boot",
		.offset = 0,
		.size = (2 * SZ_128K),
		.mask_flags = MTD_WRITEABLE,	/* Read-only */
	}, {
		.name = "Linux-Kernel",
		.offset = MTDPART_OFS_APPEND,
		.size = (20 * SZ_128K),
	}, {
		.name = "Root Filesystem",
		.offset = MTDPART_OFS_APPEND,
		.size = MTDPART_SIZ_FULL,
	},
};

static struct physmap_flash_data espt_nor_flash_data = {
	.width = 2,
	.parts = espt_nor_flash_partitions,
	.nr_parts = ARRAY_SIZE(espt_nor_flash_partitions),
};

static struct resource espt_nor_flash_resources[] = {
	[0] = {
		.name = "NOR Flash",
		.start = 0,
		.end = SZ_8M - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device espt_nor_flash_device = {
	.name = "physmap-flash",
	.resource = espt_nor_flash_resources,
	.num_resources = ARRAY_SIZE(espt_nor_flash_resources),
	.dev = {
		.platform_data = &espt_nor_flash_data,
	},
};

/* SH-Ether */
static struct resource sh_eth_resources[] = {
	{
		.start  = 0xFEE00800,   /* use eth1 */
		.end    = 0xFEE00F7C - 1,
		.flags  = IORESOURCE_MEM,
	}, {
		.start  = 57,   /* irq number */
		.flags  = IORESOURCE_IRQ,
	},
};

static struct sh_eth_plat_data sh7763_eth_pdata = {
	.phy = 0,
	.edmac_endian = EDMAC_LITTLE_ENDIAN,
};

static struct platform_device espt_eth_device = {
	.name       = "sh-eth",
	.resource   = sh_eth_resources,
	.num_resources  = ARRAY_SIZE(sh_eth_resources),
	.dev        = {
		.platform_data = &sh7763_eth_pdata,
	},
};

static struct platform_device *espt_devices[] __initdata = {
	&espt_nor_flash_device,
	&espt_eth_device,
};

static int __init espt_devices_setup(void)
{
	return platform_add_devices(espt_devices,
				    ARRAY_SIZE(espt_devices));
}
device_initcall(espt_devices_setup);

static struct sh_machine_vector mv_espt __initmv = {
	.mv_name = "ESPT-GIGA",
};