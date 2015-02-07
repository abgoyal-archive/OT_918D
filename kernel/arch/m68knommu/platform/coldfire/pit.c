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

/***************************************************************************/

/*
 *	pit.c -- Freescale ColdFire PIT timer. Currently this type of
 *	         hardware timer only exists in the Freescale ColdFire
 *		 5270/5271, 5282 and 5208 CPUs. No doubt newer ColdFire
 *		 family members will probably use it too.
 *
 *	Copyright (C) 1999-2008, Greg Ungerer (gerg@snapgear.com)
 *	Copyright (C) 2001-2004, SnapGear Inc. (www.snapgear.com)
 */

/***************************************************************************/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/param.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/clockchips.h>
#include <asm/machdep.h>
#include <asm/io.h>
#include <asm/coldfire.h>
#include <asm/mcfpit.h>
#include <asm/mcfsim.h>

/***************************************************************************/

/*
 *	By default use timer1 as the system clock timer.
 */
#define	FREQ	((MCF_CLK / 2) / 64)
#define	TA(a)	(MCF_IPSBAR + MCFPIT_BASE1 + (a))
#define PIT_CYCLES_PER_JIFFY (FREQ / HZ)

static u32 pit_cnt;

/*
 * Initialize the PIT timer.
 *
 * This is also called after resume to bring the PIT into operation again.
 */

static void init_cf_pit_timer(enum clock_event_mode mode,
                             struct clock_event_device *evt)
{
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:

		__raw_writew(MCFPIT_PCSR_DISABLE, TA(MCFPIT_PCSR));
		__raw_writew(PIT_CYCLES_PER_JIFFY, TA(MCFPIT_PMR));
		__raw_writew(MCFPIT_PCSR_EN | MCFPIT_PCSR_PIE | \
				MCFPIT_PCSR_OVW | MCFPIT_PCSR_RLD | \
				MCFPIT_PCSR_CLK64, TA(MCFPIT_PCSR));
		break;

	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:

		__raw_writew(MCFPIT_PCSR_DISABLE, TA(MCFPIT_PCSR));
		break;

	case CLOCK_EVT_MODE_ONESHOT:

		__raw_writew(MCFPIT_PCSR_DISABLE, TA(MCFPIT_PCSR));
		__raw_writew(MCFPIT_PCSR_EN | MCFPIT_PCSR_PIE | \
				MCFPIT_PCSR_OVW | MCFPIT_PCSR_CLK64, \
				TA(MCFPIT_PCSR));
		break;

	case CLOCK_EVT_MODE_RESUME:
		/* Nothing to do here */
		break;
	}
}

/*
 * Program the next event in oneshot mode
 *
 * Delta is given in PIT ticks
 */
static int cf_pit_next_event(unsigned long delta,
		struct clock_event_device *evt)
{
	__raw_writew(delta, TA(MCFPIT_PMR));
	return 0;
}

struct clock_event_device cf_pit_clockevent = {
	.name		= "pit",
	.features	= CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_mode	= init_cf_pit_timer,
	.set_next_event	= cf_pit_next_event,
	.shift		= 32,
	.irq		= MCFINT_VECBASE + MCFINT_PIT1,
};



/***************************************************************************/

static irqreturn_t pit_tick(int irq, void *dummy)
{
	struct clock_event_device *evt = &cf_pit_clockevent;
	u16 pcsr;

	/* Reset the ColdFire timer */
	pcsr = __raw_readw(TA(MCFPIT_PCSR));
	__raw_writew(pcsr | MCFPIT_PCSR_PIF, TA(MCFPIT_PCSR));

	pit_cnt += PIT_CYCLES_PER_JIFFY;
	evt->event_handler(evt);
	return IRQ_HANDLED;
}

/***************************************************************************/

static struct irqaction pit_irq = {
	.name	 = "timer",
	.flags	 = IRQF_DISABLED | IRQF_TIMER,
	.handler = pit_tick,
};

/***************************************************************************/

static cycle_t pit_read_clk(struct clocksource *cs)
{
	unsigned long flags;
	u32 cycles;
	u16 pcntr;

	local_irq_save(flags);
	pcntr = __raw_readw(TA(MCFPIT_PCNTR));
	cycles = pit_cnt;
	local_irq_restore(flags);

	return cycles + PIT_CYCLES_PER_JIFFY - pcntr;
}

/***************************************************************************/

static struct clocksource pit_clk = {
	.name	= "pit",
	.rating	= 100,
	.read	= pit_read_clk,
	.shift	= 20,
	.mask	= CLOCKSOURCE_MASK(32),
};

/***************************************************************************/

void hw_timer_init(void)
{
	cf_pit_clockevent.cpumask = cpumask_of(smp_processor_id());
	cf_pit_clockevent.mult = div_sc(FREQ, NSEC_PER_SEC, 32);
	cf_pit_clockevent.max_delta_ns =
		clockevent_delta2ns(0xFFFF, &cf_pit_clockevent);
	cf_pit_clockevent.min_delta_ns =
		clockevent_delta2ns(0x3f, &cf_pit_clockevent);
	clockevents_register_device(&cf_pit_clockevent);

	setup_irq(MCFINT_VECBASE + MCFINT_PIT1, &pit_irq);

	pit_clk.mult = clocksource_hz2mult(FREQ, pit_clk.shift);
	clocksource_register(&pit_clk);
}

/***************************************************************************/