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

/* Flash mappings for the MB93090-MB00 motherboard
 *
 * Copyright (C) 2009 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>

#define MB93090_BOOTROM_ADDR	0xFF000000	/* Boot ROM */
#define MB93090_BOOTROM_SIZE	(2 * 1024 * 1024)
#define MB93090_USERROM_ADDR	0xFF200000	/* User ROM */
#define MB93090_USERROM_SIZE	(2 * 1024 * 1024)

/*
 * default MTD partition table for both main flash devices, expected to be
 * overridden by RedBoot
 */
static struct mtd_partition mb93090_partitions[] = {
	{
		.name		= "Filesystem",
		.size		= MTDPART_SIZ_FULL,
		.offset		= 0,
	}
};

/*
 * Definition of the MB93090 Boot ROM (on the CPU card)
 */
static struct physmap_flash_data mb93090_bootrom_data = {
	.width		= 2,
	.nr_parts	= 2,
	.parts		= mb93090_partitions,
};

static struct resource mb93090_bootrom_resource = {
	.start		= MB93090_BOOTROM_ADDR,
	.end		= MB93090_BOOTROM_ADDR + MB93090_BOOTROM_SIZE - 1,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device mb93090_bootrom = {
	.name		= "physmap-flash",
	.id		= 0,
	.dev.platform_data = &mb93090_bootrom_data,
	.num_resources	= 1,
	.resource	= &mb93090_bootrom_resource,
};

/*
 * Definition of the MB93090 User ROM definition (on the motherboard)
 */
static struct physmap_flash_data mb93090_userrom_data = {
	.width		= 2,
	.nr_parts	= 2,
	.parts		= mb93090_partitions,
};

static struct resource mb93090_userrom_resource = {
	.start		= MB93090_USERROM_ADDR,
	.end		= MB93090_USERROM_ADDR + MB93090_USERROM_SIZE - 1,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device mb93090_userrom = {
	.name		= "physmap-flash",
	.id		= 1,
	.dev.platform_data = &mb93090_userrom_data,
	.num_resources	= 1,
	.resource	= &mb93090_userrom_resource,
};

/*
 * register the MB93090 flashes
 */
static int __init mb93090_mtd_init(void)
{
	platform_device_register(&mb93090_bootrom);
	platform_device_register(&mb93090_userrom);
	return 0;
}

module_init(mb93090_mtd_init);
