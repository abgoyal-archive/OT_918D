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

/*****************************************************************************
* Copyright 2001 - 2009 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <mach/csp/hw_cfg.h>
#include <mach/csp/chipcHw_def.h>
#include <mach/csp/chipcHw_reg.h>
#include <mach/csp/chipcHw_inline.h>

#include <asm/clkdev.h>

#include "clock.h"

#define clk_is_primary(x)       ((x)->type & CLK_TYPE_PRIMARY)
#define clk_is_pll1(x)          ((x)->type & CLK_TYPE_PLL1)
#define clk_is_pll2(x)          ((x)->type & CLK_TYPE_PLL2)
#define clk_is_programmable(x)  ((x)->type & CLK_TYPE_PROGRAMMABLE)
#define clk_is_bypassable(x)    ((x)->type & CLK_TYPE_BYPASSABLE)

#define clk_is_using_xtal(x)    ((x)->mode & CLK_MODE_XTAL)

static DEFINE_SPINLOCK(clk_lock);

static void __clk_enable(struct clk *clk)
{
	if (!clk)
		return;

	/* enable parent clock first */
	if (clk->parent)
		__clk_enable(clk->parent);

	if (clk->use_cnt++ == 0) {
		if (clk_is_pll1(clk)) {	/* PLL1 */
			chipcHw_pll1Enable(clk->rate_hz, 0);
		} else if (clk_is_pll2(clk)) {	/* PLL2 */
			chipcHw_pll2Enable(clk->rate_hz);
		} else if (clk_is_using_xtal(clk)) {	/* source is crystal */
			if (!clk_is_primary(clk))
				chipcHw_bypassClockEnable(clk->csp_id);
		} else {	/* source is PLL */
			chipcHw_setClockEnable(clk->csp_id);
		}
	}
}

int clk_enable(struct clk *clk)
{
	unsigned long flags;

	if (!clk)
		return -EINVAL;

	spin_lock_irqsave(&clk_lock, flags);
	__clk_enable(clk);
	spin_unlock_irqrestore(&clk_lock, flags);

	return 0;
}
EXPORT_SYMBOL(clk_enable);

static void __clk_disable(struct clk *clk)
{
	if (!clk)
		return;

	BUG_ON(clk->use_cnt == 0);

	if (--clk->use_cnt == 0) {
		if (clk_is_pll1(clk)) {	/* PLL1 */
			chipcHw_pll1Disable();
		} else if (clk_is_pll2(clk)) {	/* PLL2 */
			chipcHw_pll2Disable();
		} else if (clk_is_using_xtal(clk)) {	/* source is crystal */
			if (!clk_is_primary(clk))
				chipcHw_bypassClockDisable(clk->csp_id);
		} else {	/* source is PLL */
			chipcHw_setClockDisable(clk->csp_id);
		}
	}

	if (clk->parent)
		__clk_disable(clk->parent);
}

void clk_disable(struct clk *clk)
{
	unsigned long flags;

	if (!clk)
		return;

	spin_lock_irqsave(&clk_lock, flags);
	__clk_disable(clk);
	spin_unlock_irqrestore(&clk_lock, flags);
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	if (!clk)
		return 0;

	return clk->rate_hz;
}
EXPORT_SYMBOL(clk_get_rate);

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	unsigned long flags;
	unsigned long actual;
	unsigned long rate_hz;

	if (!clk)
		return -EINVAL;

	if (!clk_is_programmable(clk))
		return -EINVAL;

	if (clk->use_cnt)
		return -EBUSY;

	spin_lock_irqsave(&clk_lock, flags);
	actual = clk->parent->rate_hz;
	rate_hz = min(actual, rate);
	spin_unlock_irqrestore(&clk_lock, flags);

	return rate_hz;
}
EXPORT_SYMBOL(clk_round_rate);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	unsigned long flags;
	unsigned long actual;
	unsigned long rate_hz;

	if (!clk)
		return -EINVAL;

	if (!clk_is_programmable(clk))
		return -EINVAL;

	if (clk->use_cnt)
		return -EBUSY;

	spin_lock_irqsave(&clk_lock, flags);
	actual = clk->parent->rate_hz;
	rate_hz = min(actual, rate);
	rate_hz = chipcHw_setClockFrequency(clk->csp_id, rate_hz);
	clk->rate_hz = rate_hz;
	spin_unlock_irqrestore(&clk_lock, flags);

	return 0;
}
EXPORT_SYMBOL(clk_set_rate);

struct clk *clk_get_parent(struct clk *clk)
{
	if (!clk)
		return NULL;

	return clk->parent;
}
EXPORT_SYMBOL(clk_get_parent);

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	unsigned long flags;
	struct clk *old_parent;

	if (!clk || !parent)
		return -EINVAL;

	if (!clk_is_primary(parent) || !clk_is_bypassable(clk))
		return -EINVAL;

	/* if more than one user, parent is not allowed */
	if (clk->use_cnt > 1)
		return -EBUSY;

	if (clk->parent == parent)
		return 0;

	spin_lock_irqsave(&clk_lock, flags);
	old_parent = clk->parent;
	clk->parent = parent;
	if (clk_is_using_xtal(parent))
		clk->mode |= CLK_MODE_XTAL;
	else
		clk->mode &= (~CLK_MODE_XTAL);

	/* if clock is active */
	if (clk->use_cnt != 0) {
		clk->use_cnt--;
		/* enable clock with the new parent */
		__clk_enable(clk);
		/* disable the old parent */
		__clk_disable(old_parent);
	}
	spin_unlock_irqrestore(&clk_lock, flags);

	return 0;
}
EXPORT_SYMBOL(clk_set_parent);
