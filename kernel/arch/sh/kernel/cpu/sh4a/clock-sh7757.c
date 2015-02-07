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
 * arch/sh/kernel/cpu/sh4/clock-sh7757.c
 *
 * SH7757 support for the clock framework
 *
 *  Copyright (C) 2009  Renesas Solutions Corp.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <asm/clkdev.h>
#include <asm/clock.h>
#include <asm/freq.h>

static int ifc_divisors[] = { 2, 1, 4, 1, 1, 8, 1, 1,
			      16, 1, 1, 32, 1, 1, 1, 1 };
static int sfc_divisors[] = { 2, 1, 4, 1, 1, 8, 1, 1,
			      16, 1, 1, 32, 1, 1, 1, 1 };
static int bfc_divisors[] = { 2, 1, 4, 1, 1, 8, 1, 1,
			      16, 1, 1, 32, 1, 1, 1, 1 };
static int p1fc_divisors[] = { 2, 1, 4, 1, 1, 8, 1, 1,
			       16, 1, 1, 32, 1, 1, 1, 1 };

static void master_clk_init(struct clk *clk)
{
	clk->rate = CONFIG_SH_PCLK_FREQ * 16;
}

static struct clk_ops sh7757_master_clk_ops = {
	.init		= master_clk_init,
};

static void module_clk_recalc(struct clk *clk)
{
	int idx = __raw_readl(FRQCR) & 0x0000000f;
	clk->rate = clk->parent->rate / p1fc_divisors[idx];
}

static struct clk_ops sh7757_module_clk_ops = {
	.recalc		= module_clk_recalc,
};

static void bus_clk_recalc(struct clk *clk)
{
	int idx = (__raw_readl(FRQCR) >> 8) & 0x0000000f;
	clk->rate = clk->parent->rate / bfc_divisors[idx];
}

static struct clk_ops sh7757_bus_clk_ops = {
	.recalc		= bus_clk_recalc,
};

static void cpu_clk_recalc(struct clk *clk)
{
	int idx = (__raw_readl(FRQCR) >> 20) & 0x0000000f;
	clk->rate = clk->parent->rate / ifc_divisors[idx];
}

static struct clk_ops sh7757_cpu_clk_ops = {
	.recalc		= cpu_clk_recalc,
};

static struct clk_ops *sh7757_clk_ops[] = {
	&sh7757_master_clk_ops,
	&sh7757_module_clk_ops,
	&sh7757_bus_clk_ops,
	&sh7757_cpu_clk_ops,
};

void __init arch_init_clk_ops(struct clk_ops **ops, int idx)
{
	if (idx < ARRAY_SIZE(sh7757_clk_ops))
		*ops = sh7757_clk_ops[idx];
}

static void shyway_clk_recalc(struct clk *clk)
{
	int idx = (__raw_readl(FRQCR) >> 12) & 0x0000000f;
	clk->rate = clk->parent->rate / sfc_divisors[idx];
}

static struct clk_ops sh7757_shyway_clk_ops = {
	.recalc		= shyway_clk_recalc,
};

static struct clk sh7757_shyway_clk = {
	.flags		= CLK_ENABLE_ON_INIT,
	.ops		= &sh7757_shyway_clk_ops,
};

/*
 * Additional sh7757-specific on-chip clocks that aren't already part of the
 * clock framework
 */
static struct clk *sh7757_onchip_clocks[] = {
	&sh7757_shyway_clk,
};

#define CLKDEV_CON_ID(_id, _clk) { .con_id = _id, .clk = _clk }

static struct clk_lookup lookups[] = {
	/* main clocks */
	CLKDEV_CON_ID("shyway_clk", &sh7757_shyway_clk),
};

static int __init sh7757_clk_init(void)
{
	struct clk *clk = clk_get(NULL, "master_clk");
	int i;

	for (i = 0; i < ARRAY_SIZE(sh7757_onchip_clocks); i++) {
		struct clk *clkp = sh7757_onchip_clocks[i];

		clkp->parent = clk;
		clk_register(clkp);
		clk_enable(clkp);
	}

	/*
	 * Now that we have the rest of the clocks registered, we need to
	 * force the parent clock to propagate so that these clocks will
	 * automatically figure out their rate. We cheat by handing the
	 * parent clock its current rate and forcing child propagation.
	 */
	clk_set_rate(clk, clk_get_rate(clk));

	clk_put(clk);

	clkdev_add_table(lookups, ARRAY_SIZE(lookups));

	return 0;
}

arch_initcall(sh7757_clk_init);

