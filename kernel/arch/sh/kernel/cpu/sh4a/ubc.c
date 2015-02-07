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
 * arch/sh/kernel/cpu/sh4a/ubc.c
 *
 * On-chip UBC support for SH-4A CPUs.
 *
 * Copyright (C) 2009 - 2010  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <asm/hw_breakpoint.h>

#define UBC_CBR(idx)	(0xff200000 + (0x20 * idx))
#define UBC_CRR(idx)	(0xff200004 + (0x20 * idx))
#define UBC_CAR(idx)	(0xff200008 + (0x20 * idx))
#define UBC_CAMR(idx)	(0xff20000c + (0x20 * idx))

#define UBC_CCMFR	0xff200600
#define UBC_CBCR	0xff200620

/* CRR */
#define UBC_CRR_PCB	(1 << 1)
#define UBC_CRR_BIE	(1 << 0)

/* CBR */
#define UBC_CBR_CE	(1 << 0)

static struct sh_ubc sh4a_ubc;

static void sh4a_ubc_enable(struct arch_hw_breakpoint *info, int idx)
{
	__raw_writel(UBC_CBR_CE | info->len | info->type, UBC_CBR(idx));
	__raw_writel(info->address, UBC_CAR(idx));
}

static void sh4a_ubc_disable(struct arch_hw_breakpoint *info, int idx)
{
	__raw_writel(0, UBC_CBR(idx));
	__raw_writel(0, UBC_CAR(idx));
}

static void sh4a_ubc_enable_all(unsigned long mask)
{
	int i;

	for (i = 0; i < sh4a_ubc.num_events; i++)
		if (mask & (1 << i))
			__raw_writel(__raw_readl(UBC_CBR(i)) | UBC_CBR_CE,
				     UBC_CBR(i));
}

static void sh4a_ubc_disable_all(void)
{
	int i;

	for (i = 0; i < sh4a_ubc.num_events; i++)
		__raw_writel(__raw_readl(UBC_CBR(i)) & ~UBC_CBR_CE,
			     UBC_CBR(i));
}

static unsigned long sh4a_ubc_active_mask(void)
{
	unsigned long active = 0;
	int i;

	for (i = 0; i < sh4a_ubc.num_events; i++)
		if (__raw_readl(UBC_CBR(i)) & UBC_CBR_CE)
			active |= (1 << i);

	return active;
}

static unsigned long sh4a_ubc_triggered_mask(void)
{
	return __raw_readl(UBC_CCMFR);
}

static void sh4a_ubc_clear_triggered_mask(unsigned long mask)
{
	__raw_writel(__raw_readl(UBC_CCMFR) & ~mask, UBC_CCMFR);
}

static struct sh_ubc sh4a_ubc = {
	.name			= "SH-4A",
	.num_events		= 2,
	.trap_nr		= 0x1e0,
	.enable			= sh4a_ubc_enable,
	.disable		= sh4a_ubc_disable,
	.enable_all		= sh4a_ubc_enable_all,
	.disable_all		= sh4a_ubc_disable_all,
	.active_mask		= sh4a_ubc_active_mask,
	.triggered_mask		= sh4a_ubc_triggered_mask,
	.clear_triggered_mask	= sh4a_ubc_clear_triggered_mask,
};

static int __init sh4a_ubc_init(void)
{
	struct clk *ubc_iclk = clk_get(NULL, "ubc0");
	int i;

	/*
	 * The UBC MSTP bit is optional, as not all platforms will have
	 * it. Just ignore it if we can't find it.
	 */
	if (IS_ERR(ubc_iclk))
		ubc_iclk = NULL;

	clk_enable(ubc_iclk);

	__raw_writel(0, UBC_CBCR);

	for (i = 0; i < sh4a_ubc.num_events; i++) {
		__raw_writel(0, UBC_CAMR(i));
		__raw_writel(0, UBC_CBR(i));

		__raw_writel(UBC_CRR_BIE | UBC_CRR_PCB, UBC_CRR(i));

		/* dummy read for write posting */
		(void)__raw_readl(UBC_CRR(i));
	}

	clk_disable(ubc_iclk);

	sh4a_ubc.clk = ubc_iclk;

	return register_sh_ubc(&sh4a_ubc);
}
arch_initcall(sh4a_ubc_init);
