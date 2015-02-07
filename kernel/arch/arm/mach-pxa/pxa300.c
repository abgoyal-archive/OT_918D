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
 * linux/arch/arm/mach-pxa/pxa300.c
 *
 * Code specific to PXA300/PXA310
 *
 * Copyright (C) 2007 Marvell Internation Ltd.
 *
 * 2007-08-21: eric miao <eric.miao@marvell.com>
 *             initial version
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

#include <mach/pxa300.h>

#include "generic.h"
#include "devices.h"
#include "clock.h"

static struct mfp_addr_map pxa300_mfp_addr_map[] __initdata = {

	MFP_ADDR_X(GPIO0,   GPIO2,   0x00b4),
	MFP_ADDR_X(GPIO3,   GPIO26,  0x027c),
	MFP_ADDR_X(GPIO27,  GPIO98,  0x0400),
	MFP_ADDR_X(GPIO99,  GPIO127, 0x0600),
	MFP_ADDR_X(GPIO0_2, GPIO1_2, 0x0674),
	MFP_ADDR_X(GPIO2_2, GPIO6_2, 0x02dc),

	MFP_ADDR(nBE0, 0x0204),
	MFP_ADDR(nBE1, 0x0208),

	MFP_ADDR(nLUA, 0x0244),
	MFP_ADDR(nLLA, 0x0254),

	MFP_ADDR(DF_CLE_nOE, 0x0240),
	MFP_ADDR(DF_nRE_nOE, 0x0200),
	MFP_ADDR(DF_ALE_nWE, 0x020C),
	MFP_ADDR(DF_INT_RnB, 0x00C8),
	MFP_ADDR(DF_nCS0, 0x0248),
	MFP_ADDR(DF_nCS1, 0x0278),
	MFP_ADDR(DF_nWE, 0x00CC),

	MFP_ADDR(DF_ADDR0, 0x0210),
	MFP_ADDR(DF_ADDR1, 0x0214),
	MFP_ADDR(DF_ADDR2, 0x0218),
	MFP_ADDR(DF_ADDR3, 0x021C),

	MFP_ADDR(DF_IO0, 0x0220),
	MFP_ADDR(DF_IO1, 0x0228),
	MFP_ADDR(DF_IO2, 0x0230),
	MFP_ADDR(DF_IO3, 0x0238),
	MFP_ADDR(DF_IO4, 0x0258),
	MFP_ADDR(DF_IO5, 0x0260),
	MFP_ADDR(DF_IO6, 0x0268),
	MFP_ADDR(DF_IO7, 0x0270),
	MFP_ADDR(DF_IO8, 0x0224),
	MFP_ADDR(DF_IO9, 0x022C),
	MFP_ADDR(DF_IO10, 0x0234),
	MFP_ADDR(DF_IO11, 0x023C),
	MFP_ADDR(DF_IO12, 0x025C),
	MFP_ADDR(DF_IO13, 0x0264),
	MFP_ADDR(DF_IO14, 0x026C),
	MFP_ADDR(DF_IO15, 0x0274),

	MFP_ADDR_END,
};

/* override pxa300 MFP register addresses */
static struct mfp_addr_map pxa310_mfp_addr_map[] __initdata = {
	MFP_ADDR_X(GPIO30,  GPIO98,   0x0418),
	MFP_ADDR_X(GPIO7_2, GPIO12_2, 0x052C),

	MFP_ADDR(ULPI_STP, 0x040C),
	MFP_ADDR(ULPI_NXT, 0x0410),
	MFP_ADDR(ULPI_DIR, 0x0414),

	MFP_ADDR_END,
};

static DEFINE_PXA3_CKEN(common_nand, NAND, 156000000, 0);
static DEFINE_PXA3_CKEN(gcu, PXA300_GCU, 0, 0);

static struct clk_lookup common_clkregs[] = {
	INIT_CLKREG(&clk_common_nand, "pxa3xx-nand", NULL),
	INIT_CLKREG(&clk_gcu, "pxa3xx-gcu", NULL),
};

static DEFINE_PXA3_CKEN(pxa310_mmc3, MMC3, 19500000, 0);

static struct clk_lookup pxa310_clkregs[] = {
	INIT_CLKREG(&clk_pxa310_mmc3, "pxa2xx-mci.2", NULL),
};

static int __init pxa300_init(void)
{
	if (cpu_is_pxa300() || cpu_is_pxa310()) {
		mfp_init_base(io_p2v(MFPR_BASE));
		mfp_init_addr(pxa300_mfp_addr_map);
		clkdev_add_table(ARRAY_AND_SIZE(common_clkregs));
	}

	if (cpu_is_pxa310()) {
		mfp_init_addr(pxa310_mfp_addr_map);
		clkdev_add_table(ARRAY_AND_SIZE(pxa310_clkregs));
	}

	return 0;
}

core_initcall(pxa300_init);
