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

#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <bcm63xx_cpu.h>
#include <bcm63xx_io.h>
#include <bcm63xx_timer.h>
#include <bcm63xx_regs.h>

static DEFINE_RAW_SPINLOCK(timer_reg_lock);
static DEFINE_RAW_SPINLOCK(timer_data_lock);
static struct clk *periph_clk;

static struct timer_data {
	void	(*cb)(void *);
	void	*data;
} timer_data[BCM63XX_TIMER_COUNT];

static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	u32 stat;
	int i;

	raw_spin_lock(&timer_reg_lock);
	stat = bcm_timer_readl(TIMER_IRQSTAT_REG);
	bcm_timer_writel(stat, TIMER_IRQSTAT_REG);
	raw_spin_unlock(&timer_reg_lock);

	for (i = 0; i < BCM63XX_TIMER_COUNT; i++) {
		if (!(stat & TIMER_IRQSTAT_TIMER_CAUSE(i)))
			continue;

		raw_spin_lock(&timer_data_lock);
		if (!timer_data[i].cb) {
			raw_spin_unlock(&timer_data_lock);
			continue;
		}

		timer_data[i].cb(timer_data[i].data);
		raw_spin_unlock(&timer_data_lock);
	}

	return IRQ_HANDLED;
}

int bcm63xx_timer_enable(int id)
{
	u32 reg;
	unsigned long flags;

	if (id >= BCM63XX_TIMER_COUNT)
		return -EINVAL;

	raw_spin_lock_irqsave(&timer_reg_lock, flags);

	reg = bcm_timer_readl(TIMER_CTLx_REG(id));
	reg |= TIMER_CTL_ENABLE_MASK;
	bcm_timer_writel(reg, TIMER_CTLx_REG(id));

	reg = bcm_timer_readl(TIMER_IRQSTAT_REG);
	reg |= TIMER_IRQSTAT_TIMER_IR_EN(id);
	bcm_timer_writel(reg, TIMER_IRQSTAT_REG);

	raw_spin_unlock_irqrestore(&timer_reg_lock, flags);
	return 0;
}

EXPORT_SYMBOL(bcm63xx_timer_enable);

int bcm63xx_timer_disable(int id)
{
	u32 reg;
	unsigned long flags;

	if (id >= BCM63XX_TIMER_COUNT)
		return -EINVAL;

	raw_spin_lock_irqsave(&timer_reg_lock, flags);

	reg = bcm_timer_readl(TIMER_CTLx_REG(id));
	reg &= ~TIMER_CTL_ENABLE_MASK;
	bcm_timer_writel(reg, TIMER_CTLx_REG(id));

	reg = bcm_timer_readl(TIMER_IRQSTAT_REG);
	reg &= ~TIMER_IRQSTAT_TIMER_IR_EN(id);
	bcm_timer_writel(reg, TIMER_IRQSTAT_REG);

	raw_spin_unlock_irqrestore(&timer_reg_lock, flags);
	return 0;
}

EXPORT_SYMBOL(bcm63xx_timer_disable);

int bcm63xx_timer_register(int id, void (*callback)(void *data), void *data)
{
	unsigned long flags;
	int ret;

	if (id >= BCM63XX_TIMER_COUNT || !callback)
		return -EINVAL;

	ret = 0;
	raw_spin_lock_irqsave(&timer_data_lock, flags);
	if (timer_data[id].cb) {
		ret = -EBUSY;
		goto out;
	}

	timer_data[id].cb = callback;
	timer_data[id].data = data;

out:
	raw_spin_unlock_irqrestore(&timer_data_lock, flags);
	return ret;
}

EXPORT_SYMBOL(bcm63xx_timer_register);

void bcm63xx_timer_unregister(int id)
{
	unsigned long flags;

	if (id >= BCM63XX_TIMER_COUNT)
		return;

	raw_spin_lock_irqsave(&timer_data_lock, flags);
	timer_data[id].cb = NULL;
	raw_spin_unlock_irqrestore(&timer_data_lock, flags);
}

EXPORT_SYMBOL(bcm63xx_timer_unregister);

unsigned int bcm63xx_timer_countdown(unsigned int countdown_us)
{
	return (clk_get_rate(periph_clk) / (1000 * 1000)) * countdown_us;
}

EXPORT_SYMBOL(bcm63xx_timer_countdown);

int bcm63xx_timer_set(int id, int monotonic, unsigned int countdown_us)
{
	u32 reg, countdown;
	unsigned long flags;

	if (id >= BCM63XX_TIMER_COUNT)
		return -EINVAL;

	countdown = bcm63xx_timer_countdown(countdown_us);
	if (countdown & ~TIMER_CTL_COUNTDOWN_MASK)
		return -EINVAL;

	raw_spin_lock_irqsave(&timer_reg_lock, flags);
	reg = bcm_timer_readl(TIMER_CTLx_REG(id));

	if (monotonic)
		reg &= ~TIMER_CTL_MONOTONIC_MASK;
	else
		reg |= TIMER_CTL_MONOTONIC_MASK;

	reg &= ~TIMER_CTL_COUNTDOWN_MASK;
	reg |= countdown;
	bcm_timer_writel(reg, TIMER_CTLx_REG(id));

	raw_spin_unlock_irqrestore(&timer_reg_lock, flags);
	return 0;
}

EXPORT_SYMBOL(bcm63xx_timer_set);

int bcm63xx_timer_init(void)
{
	int ret, irq;
	u32 reg;

	reg = bcm_timer_readl(TIMER_IRQSTAT_REG);
	reg &= ~TIMER_IRQSTAT_TIMER0_IR_EN;
	reg &= ~TIMER_IRQSTAT_TIMER1_IR_EN;
	reg &= ~TIMER_IRQSTAT_TIMER2_IR_EN;
	bcm_timer_writel(reg, TIMER_IRQSTAT_REG);

	periph_clk = clk_get(NULL, "periph");
	if (IS_ERR(periph_clk))
		return -ENODEV;

	irq = bcm63xx_get_irq_number(IRQ_TIMER);
	ret = request_irq(irq, timer_interrupt, 0, "bcm63xx_timer", NULL);
	if (ret) {
		printk(KERN_ERR "bcm63xx_timer: failed to register irq\n");
		return ret;
	}

	return 0;
}

arch_initcall(bcm63xx_timer_init);
