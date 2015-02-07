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
 * linux/arch/arm/mach-w90x900/mfp.c
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

#define REG_MFSEL	(W90X900_VA_GCR + 0xC)

#define GPSELF		(0x01 << 1)

#define GPSELC		(0x03 << 2)
#define ENKPI		(0x02 << 2)
#define ENNAND		(0x01 << 2)

#define GPSELEI0	(0x01 << 26)
#define GPSELEI1	(0x01 << 27)

#define GPIOG0TO1	(0x03 << 14)
#define GPIOG2TO3	(0x03 << 16)
#define GPIOG22TO23	(0x03 << 22)

#define ENSPI		(0x0a << 14)
#define ENI2C0		(0x01 << 14)
#define ENI2C1		(0x01 << 16)
#define ENAC97		(0x02 << 22)

static DEFINE_MUTEX(mfp_mutex);

void mfp_set_groupf(struct device *dev)
{
	unsigned long mfpen;
	const char *dev_id;

	BUG_ON(!dev);

	mutex_lock(&mfp_mutex);

	dev_id = dev_name(dev);

	mfpen = __raw_readl(REG_MFSEL);

	if (strcmp(dev_id, "nuc900-emc") == 0)
		mfpen |= GPSELF;/*enable mac*/
	else
		mfpen &= ~GPSELF;/*GPIOF[9:0]*/

	__raw_writel(mfpen, REG_MFSEL);

	mutex_unlock(&mfp_mutex);
}
EXPORT_SYMBOL(mfp_set_groupf);

void mfp_set_groupc(struct device *dev)
{
	unsigned long mfpen;
	const char *dev_id;

	BUG_ON(!dev);

	mutex_lock(&mfp_mutex);

	dev_id = dev_name(dev);

	mfpen = __raw_readl(REG_MFSEL);

	if (strcmp(dev_id, "nuc900-lcd") == 0)
		mfpen |= GPSELC;/*enable lcd*/
	else if (strcmp(dev_id, "nuc900-kpi") == 0) {
		mfpen &= (~GPSELC);/*enable kpi*/
		mfpen |= ENKPI;
	} else if (strcmp(dev_id, "nuc900-nand") == 0) {
		mfpen &= (~GPSELC);/*enable nand*/
		mfpen |= ENNAND;
	} else
		mfpen &= (~GPSELC);/*GPIOC[14:0]*/

	__raw_writel(mfpen, REG_MFSEL);

	mutex_unlock(&mfp_mutex);
}
EXPORT_SYMBOL(mfp_set_groupc);

void mfp_set_groupi(struct device *dev)
{
	unsigned long mfpen;
	const char *dev_id;

	BUG_ON(!dev);

	mutex_lock(&mfp_mutex);

	dev_id = dev_name(dev);

	mfpen = __raw_readl(REG_MFSEL);

	mfpen &= ~GPSELEI1;/*default gpio16*/

	if (strcmp(dev_id, "nuc900-wdog") == 0)
		mfpen |= GPSELEI1;/*enable wdog*/
	else if (strcmp(dev_id, "nuc900-atapi") == 0)
		mfpen |= GPSELEI0;/*enable atapi*/
	else if (strcmp(dev_id, "nuc900-keypad") == 0)
		mfpen &= ~GPSELEI0;/*enable keypad*/

	__raw_writel(mfpen, REG_MFSEL);

	mutex_unlock(&mfp_mutex);
}
EXPORT_SYMBOL(mfp_set_groupi);

void mfp_set_groupg(struct device *dev)
{
	unsigned long mfpen;
	const char *dev_id;

	BUG_ON(!dev);

	mutex_lock(&mfp_mutex);

	dev_id = dev_name(dev);

	mfpen = __raw_readl(REG_MFSEL);

	if (strcmp(dev_id, "nuc900-spi") == 0) {
		mfpen &= ~(GPIOG0TO1 | GPIOG2TO3);
		mfpen |= ENSPI;/*enable spi*/
	} else if (strcmp(dev_id, "nuc900-i2c0") == 0) {
		mfpen &= ~(GPIOG0TO1);
		mfpen |= ENI2C0;/*enable i2c0*/
	} else if (strcmp(dev_id, "nuc900-i2c1") == 0) {
		mfpen &= ~(GPIOG2TO3);
		mfpen |= ENI2C1;/*enable i2c1*/
	} else if (strcmp(dev_id, "nuc900-audio") == 0) {
		mfpen &= ~(GPIOG22TO23);
		mfpen |= ENAC97;/*enable AC97*/
	} else {
		mfpen &= ~(GPIOG0TO1 | GPIOG2TO3);/*GPIOG[3:0]*/
	}

	__raw_writel(mfpen, REG_MFSEL);

	mutex_unlock(&mfp_mutex);
}
EXPORT_SYMBOL(mfp_set_groupg);

