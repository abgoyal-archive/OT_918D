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
 * System timer for Freescale STMP37XX/STMP378X
 *
 * Embedded Alley Solutions, Inc <source@embeddedalley.com>
 *
 * Copyright 2008 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

#include <asm/mach/time.h>
#include <mach/stmp3xxx.h>
#include <mach/platform.h>
#include <mach/regs-timrot.h>

static irqreturn_t
stmp3xxx_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *c = dev_id;

	/* timer 0 */
	if (__raw_readl(REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL0) &
			BM_TIMROT_TIMCTRLn_IRQ) {
		stmp3xxx_clearl(BM_TIMROT_TIMCTRLn_IRQ,
				REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL0);
		c->event_handler(c);
	}

	/* timer 1 */
	else if (__raw_readl(REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL1)
			& BM_TIMROT_TIMCTRLn_IRQ) {
		stmp3xxx_clearl(BM_TIMROT_TIMCTRLn_IRQ,
				REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL1);
		stmp3xxx_clearl(BM_TIMROT_TIMCTRLn_IRQ_EN,
				REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL1);
		__raw_writel(0xFFFF, REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT1);
	}

	return IRQ_HANDLED;
}

static cycle_t stmp3xxx_clock_read(struct clocksource *cs)
{
	return ~((__raw_readl(REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT1)
				& 0xFFFF0000) >> 16);
}

static int
stmp3xxx_timrot_set_next_event(unsigned long delta,
		struct clock_event_device *dev)
{
	/* reload the timer */
	__raw_writel(delta, REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT0);
	return 0;
}

static void
stmp3xxx_timrot_set_mode(enum clock_event_mode mode,
		struct clock_event_device *dev)
{
}

static struct clock_event_device ckevt_timrot = {
	.name		= "timrot",
	.features	= CLOCK_EVT_FEAT_ONESHOT,
	.shift		= 32,
	.set_next_event	= stmp3xxx_timrot_set_next_event,
	.set_mode	= stmp3xxx_timrot_set_mode,
};

static struct clocksource cksrc_stmp3xxx = {
	.name           = "cksrc_stmp3xxx",
	.rating         = 250,
	.read           = stmp3xxx_clock_read,
	.mask           = CLOCKSOURCE_MASK(16),
	.shift          = 10,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static struct irqaction stmp3xxx_timer_irq = {
	.name		= "stmp3xxx_timer",
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.handler	= stmp3xxx_timer_interrupt,
	.dev_id		= &ckevt_timrot,
};


/*
 * Set up timer interrupt, and return the current time in seconds.
 */
static void __init stmp3xxx_init_timer(void)
{
	cksrc_stmp3xxx.mult = clocksource_hz2mult(CLOCK_TICK_RATE,
				cksrc_stmp3xxx.shift);
	ckevt_timrot.mult = div_sc(CLOCK_TICK_RATE, NSEC_PER_SEC,
				ckevt_timrot.shift);
	ckevt_timrot.min_delta_ns = clockevent_delta2ns(2, &ckevt_timrot);
	ckevt_timrot.max_delta_ns = clockevent_delta2ns(0xFFF, &ckevt_timrot);
	ckevt_timrot.cpumask = cpumask_of(0);

	stmp3xxx_reset_block(REGS_TIMROT_BASE, false);

	/* clear two timers */
	__raw_writel(0, REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT0);
	__raw_writel(0, REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT1);

	/* configure them */
	__raw_writel(
		(8 << BP_TIMROT_TIMCTRLn_SELECT) |  /* 32 kHz */
		BM_TIMROT_TIMCTRLn_RELOAD |
		BM_TIMROT_TIMCTRLn_UPDATE |
		BM_TIMROT_TIMCTRLn_IRQ_EN,
			REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL0);
	__raw_writel(
		(8 << BP_TIMROT_TIMCTRLn_SELECT) |  /* 32 kHz */
		BM_TIMROT_TIMCTRLn_RELOAD |
		BM_TIMROT_TIMCTRLn_UPDATE |
		BM_TIMROT_TIMCTRLn_IRQ_EN,
			REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL1);

	__raw_writel(CLOCK_TICK_RATE / HZ - 1,
			REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT0);
	__raw_writel(0xFFFF, REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT1);

	setup_irq(IRQ_TIMER0, &stmp3xxx_timer_irq);

	clocksource_register(&cksrc_stmp3xxx);
	clockevents_register_device(&ckevt_timrot);
}

#ifdef CONFIG_PM

void stmp3xxx_suspend_timer(void)
{
	stmp3xxx_clearl(BM_TIMROT_TIMCTRLn_IRQ_EN | BM_TIMROT_TIMCTRLn_IRQ,
			REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL0);
	stmp3xxx_setl(BM_TIMROT_ROTCTRL_CLKGATE,
			REGS_TIMROT_BASE + HW_TIMROT_ROTCTRL);
}

void stmp3xxx_resume_timer(void)
{
	stmp3xxx_clearl(BM_TIMROT_ROTCTRL_SFTRST | BM_TIMROT_ROTCTRL_CLKGATE,
			REGS_TIMROT_BASE + HW_TIMROT_ROTCTRL);
	__raw_writel(
		8 << BP_TIMROT_TIMCTRLn_SELECT |  /* 32 kHz */
		BM_TIMROT_TIMCTRLn_RELOAD |
		BM_TIMROT_TIMCTRLn_UPDATE |
		BM_TIMROT_TIMCTRLn_IRQ_EN,
			REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL0);
	__raw_writel(
		8 << BP_TIMROT_TIMCTRLn_SELECT |  /* 32 kHz */
		BM_TIMROT_TIMCTRLn_RELOAD |
		BM_TIMROT_TIMCTRLn_UPDATE |
		BM_TIMROT_TIMCTRLn_IRQ_EN,
			REGS_TIMROT_BASE + HW_TIMROT_TIMCTRL1);
	__raw_writel(CLOCK_TICK_RATE / HZ - 1,
			REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT0);
	__raw_writel(0xFFFF, REGS_TIMROT_BASE + HW_TIMROT_TIMCOUNT1);
}

#else

#define stmp3xxx_suspend_timer	NULL
#define	stmp3xxx_resume_timer	NULL

#endif	/* CONFIG_PM */

struct sys_timer stmp3xxx_timer = {
	.init		= stmp3xxx_init_timer,
	.suspend	= stmp3xxx_suspend_timer,
	.resume		= stmp3xxx_resume_timer,
};
