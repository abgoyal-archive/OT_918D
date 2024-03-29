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
 * arch/arm/mach-netx/time.c
 *
 * Copyright (c) 2005 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/mach/time.h>
#include <mach/netx-regs.h>

#define TIMER_CLOCKEVENT 0
#define TIMER_CLOCKSOURCE 1

static void netx_set_mode(enum clock_event_mode mode,
		struct clock_event_device *clk)
{
	u32 tmode;

	/* disable timer */
	writel(0, NETX_GPIO_COUNTER_CTRL(TIMER_CLOCKEVENT));

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		writel(LATCH, NETX_GPIO_COUNTER_MAX(TIMER_CLOCKEVENT));
		tmode = NETX_GPIO_COUNTER_CTRL_RST_EN |
			NETX_GPIO_COUNTER_CTRL_IRQ_EN |
			NETX_GPIO_COUNTER_CTRL_RUN;
		break;

	case CLOCK_EVT_MODE_ONESHOT:
		writel(0, NETX_GPIO_COUNTER_MAX(TIMER_CLOCKEVENT));
		tmode = NETX_GPIO_COUNTER_CTRL_IRQ_EN |
			NETX_GPIO_COUNTER_CTRL_RUN;
		break;

	default:
		WARN(1, "%s: unhandled mode %d\n", __func__, mode);
		/* fall through */

	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_RESUME:
		tmode = 0;
		break;
	}

	writel(tmode, NETX_GPIO_COUNTER_CTRL(TIMER_CLOCKEVENT));
}

static int netx_set_next_event(unsigned long evt,
		struct clock_event_device *clk)
{
	writel(0 - evt, NETX_GPIO_COUNTER_CURRENT(TIMER_CLOCKEVENT));
	return 0;
}

static struct clock_event_device netx_clockevent = {
	.name = "netx-timer" __stringify(TIMER_CLOCKEVENT),
	.shift = 32,
	.features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_next_event = netx_set_next_event,
	.set_mode = netx_set_mode,
};

/*
 * IRQ handler for the timer
 */
static irqreturn_t
netx_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &netx_clockevent;

	/* acknowledge interrupt */
	writel(COUNTER_BIT(0), NETX_GPIO_IRQ);

	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct irqaction netx_timer_irq = {
	.name		= "NetX Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= netx_timer_interrupt,
};

cycle_t netx_get_cycles(struct clocksource *cs)
{
	return readl(NETX_GPIO_COUNTER_CURRENT(TIMER_CLOCKSOURCE));
}

static struct clocksource clocksource_netx = {
	.name		= "netx_timer",
	.rating		= 200,
	.read		= netx_get_cycles,
	.mask		= CLOCKSOURCE_MASK(32),
	.shift		= 20,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

/*
 * Set up timer interrupt
 */
static void __init netx_timer_init(void)
{
	/* disable timer initially */
	writel(0, NETX_GPIO_COUNTER_CTRL(0));

	/* Reset the timer value to zero */
	writel(0, NETX_GPIO_COUNTER_CURRENT(0));

	writel(LATCH, NETX_GPIO_COUNTER_MAX(0));

	/* acknowledge interrupt */
	writel(COUNTER_BIT(0), NETX_GPIO_IRQ);

	/* Enable the interrupt in the specific timer
	 * register and start timer
	 */
	writel(COUNTER_BIT(0), NETX_GPIO_IRQ_ENABLE);
	writel(NETX_GPIO_COUNTER_CTRL_IRQ_EN | NETX_GPIO_COUNTER_CTRL_RUN,
			NETX_GPIO_COUNTER_CTRL(0));

	setup_irq(NETX_IRQ_TIMER0, &netx_timer_irq);

	/* Setup timer one for clocksource */
	writel(0, NETX_GPIO_COUNTER_CTRL(TIMER_CLOCKSOURCE));
	writel(0, NETX_GPIO_COUNTER_CURRENT(TIMER_CLOCKSOURCE));
	writel(0xffffffff, NETX_GPIO_COUNTER_MAX(TIMER_CLOCKSOURCE));

	writel(NETX_GPIO_COUNTER_CTRL_RUN,
			NETX_GPIO_COUNTER_CTRL(TIMER_CLOCKSOURCE));

	clocksource_netx.mult =
		clocksource_hz2mult(CLOCK_TICK_RATE, clocksource_netx.shift);
	clocksource_register(&clocksource_netx);

	netx_clockevent.mult = div_sc(CLOCK_TICK_RATE, NSEC_PER_SEC,
			netx_clockevent.shift);
	netx_clockevent.max_delta_ns =
		clockevent_delta2ns(0xfffffffe, &netx_clockevent);
	/* with max_delta_ns >= delta2ns(0x800) the system currently runs fine.
	 * Adding some safety ... */
	netx_clockevent.min_delta_ns =
		clockevent_delta2ns(0xa00, &netx_clockevent);
	netx_clockevent.cpumask = cpumask_of(0);
	clockevents_register_device(&netx_clockevent);
}

struct sys_timer netx_timer = {
	.init		= netx_timer_init,
};
