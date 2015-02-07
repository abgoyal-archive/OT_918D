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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 Maxime Bizon <mbizon@freebox.fr>
 */

#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <bcm63xx_cpu.h>
#include <bcm63xx_io.h>
#include <bcm63xx_regs.h>
#include <bcm63xx_clk.h>

static DEFINE_MUTEX(clocks_mutex);


static void clk_enable_unlocked(struct clk *clk)
{
	if (clk->set && (clk->usage++) == 0)
		clk->set(clk, 1);
}

static void clk_disable_unlocked(struct clk *clk)
{
	if (clk->set && (--clk->usage) == 0)
		clk->set(clk, 0);
}

static void bcm_hwclock_set(u32 mask, int enable)
{
	u32 reg;

	reg = bcm_perf_readl(PERF_CKCTL_REG);
	if (enable)
		reg |= mask;
	else
		reg &= ~mask;
	bcm_perf_writel(reg, PERF_CKCTL_REG);
}

/*
 * Ethernet MAC "misc" clock: dma clocks and main clock on 6348
 */
static void enet_misc_set(struct clk *clk, int enable)
{
	u32 mask;

	if (BCMCPU_IS_6338())
		mask = CKCTL_6338_ENET_EN;
	else if (BCMCPU_IS_6345())
		mask = CKCTL_6345_ENET_EN;
	else if (BCMCPU_IS_6348())
		mask = CKCTL_6348_ENET_EN;
	else
		/* BCMCPU_IS_6358 */
		mask = CKCTL_6358_EMUSB_EN;
	bcm_hwclock_set(mask, enable);
}

static struct clk clk_enet_misc = {
	.set	= enet_misc_set,
};

/*
 * Ethernet MAC clocks: only revelant on 6358, silently enable misc
 * clocks
 */
static void enetx_set(struct clk *clk, int enable)
{
	if (enable)
		clk_enable_unlocked(&clk_enet_misc);
	else
		clk_disable_unlocked(&clk_enet_misc);

	if (BCMCPU_IS_6358()) {
		u32 mask;

		if (clk->id == 0)
			mask = CKCTL_6358_ENET0_EN;
		else
			mask = CKCTL_6358_ENET1_EN;
		bcm_hwclock_set(mask, enable);
	}
}

static struct clk clk_enet0 = {
	.id	= 0,
	.set	= enetx_set,
};

static struct clk clk_enet1 = {
	.id	= 1,
	.set	= enetx_set,
};

/*
 * Ethernet PHY clock
 */
static void ephy_set(struct clk *clk, int enable)
{
	if (!BCMCPU_IS_6358())
		return;
	bcm_hwclock_set(CKCTL_6358_EPHY_EN, enable);
}


static struct clk clk_ephy = {
	.set	= ephy_set,
};

/*
 * PCM clock
 */
static void pcm_set(struct clk *clk, int enable)
{
	if (!BCMCPU_IS_6358())
		return;
	bcm_hwclock_set(CKCTL_6358_PCM_EN, enable);
}

static struct clk clk_pcm = {
	.set	= pcm_set,
};

/*
 * USB host clock
 */
static void usbh_set(struct clk *clk, int enable)
{
	if (!BCMCPU_IS_6348())
		return;
	bcm_hwclock_set(CKCTL_6348_USBH_EN, enable);
}

static struct clk clk_usbh = {
	.set	= usbh_set,
};

/*
 * SPI clock
 */
static void spi_set(struct clk *clk, int enable)
{
	u32 mask;

	if (BCMCPU_IS_6338())
		mask = CKCTL_6338_SPI_EN;
	else if (BCMCPU_IS_6348())
		mask = CKCTL_6348_SPI_EN;
	else
		/* BCMCPU_IS_6358 */
		mask = CKCTL_6358_SPI_EN;
	bcm_hwclock_set(mask, enable);
}

static struct clk clk_spi = {
	.set	= spi_set,
};

/*
 * Internal peripheral clock
 */
static struct clk clk_periph = {
	.rate	= (50 * 1000 * 1000),
};


/*
 * Linux clock API implementation
 */
int clk_enable(struct clk *clk)
{
	mutex_lock(&clocks_mutex);
	clk_enable_unlocked(clk);
	mutex_unlock(&clocks_mutex);
	return 0;
}

EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	mutex_lock(&clocks_mutex);
	clk_disable_unlocked(clk);
	mutex_unlock(&clocks_mutex);
}

EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	return clk->rate;
}

EXPORT_SYMBOL(clk_get_rate);

struct clk *clk_get(struct device *dev, const char *id)
{
	if (!strcmp(id, "enet0"))
		return &clk_enet0;
	if (!strcmp(id, "enet1"))
		return &clk_enet1;
	if (!strcmp(id, "ephy"))
		return &clk_ephy;
	if (!strcmp(id, "usbh"))
		return &clk_usbh;
	if (!strcmp(id, "spi"))
		return &clk_spi;
	if (!strcmp(id, "periph"))
		return &clk_periph;
	if (BCMCPU_IS_6358() && !strcmp(id, "pcm"))
		return &clk_pcm;
	return ERR_PTR(-ENOENT);
}

EXPORT_SYMBOL(clk_get);

void clk_put(struct clk *clk)
{
}

EXPORT_SYMBOL(clk_put);
