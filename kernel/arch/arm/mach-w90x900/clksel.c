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
 * linux/arch/arm/mach-w90x900/clksel.c
 *
 * Copyright (c) 2008 Nuvoton technology corporation
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/mutex.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/regs-clock.h>

#define PLL0		0x00
#define PLL1		0x01
#define OTHER		0x02
#define EXT		0x03
#define MSOFFSET	0x0C
#define ATAOFFSET	0x0a
#define LCDOFFSET	0x06
#define AUDOFFSET	0x04
#define CPUOFFSET	0x00

static DEFINE_MUTEX(clksel_sem);

static void clock_source_select(const char *dev_id, unsigned int clkval)
{
	unsigned int clksel, offset;

	clksel = __raw_readl(REG_CLKSEL);

	if (strcmp(dev_id, "nuc900-ms") == 0)
		offset = MSOFFSET;
	else if (strcmp(dev_id, "nuc900-atapi") == 0)
		offset = ATAOFFSET;
	else if (strcmp(dev_id, "nuc900-lcd") == 0)
		offset = LCDOFFSET;
	else if (strcmp(dev_id, "nuc900-audio") == 0)
		offset = AUDOFFSET;
	else
		offset = CPUOFFSET;

	clksel &= ~(0x03 << offset);
	clksel |= (clkval << offset);

	__raw_writel(clksel, REG_CLKSEL);
}

void nuc900_clock_source(struct device *dev, unsigned char *src)
{
	unsigned int clkval;
	const char *dev_id;

	BUG_ON(!src);
	clkval = 0;

	mutex_lock(&clksel_sem);

	if (dev)
		dev_id = dev_name(dev);
	else
		dev_id = "cpufreq";

	if (strcmp(src, "pll0") == 0)
		clkval = PLL0;
	else if (strcmp(src, "pll1") == 0)
		clkval = PLL1;
	else if (strcmp(src, "ext") == 0)
		clkval = EXT;
	else if (strcmp(src, "oth") == 0)
		clkval = OTHER;

	clock_source_select(dev_id, clkval);

	mutex_unlock(&clksel_sem);
}
EXPORT_SYMBOL(nuc900_clock_source);

