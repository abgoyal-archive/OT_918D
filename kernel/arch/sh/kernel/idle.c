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
 * The idle loop for all SuperH platforms.
 *
 *  Copyright (C) 2002 - 2009  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/pm.h>
#include <linux/tick.h>
#include <linux/preempt.h>
#include <linux/thread_info.h>
#include <linux/irqflags.h>
#include <linux/smp.h>
#include <asm/pgalloc.h>
#include <asm/system.h>
#include <asm/atomic.h>
#include <asm/smp.h>

void (*pm_idle)(void) = NULL;

static int hlt_counter;

static int __init nohlt_setup(char *__unused)
{
	hlt_counter = 1;
	return 1;
}
__setup("nohlt", nohlt_setup);

static int __init hlt_setup(char *__unused)
{
	hlt_counter = 0;
	return 1;
}
__setup("hlt", hlt_setup);

static inline int hlt_works(void)
{
	return !hlt_counter;
}

/*
 * On SMP it's slightly faster (but much more power-consuming!)
 * to poll the ->work.need_resched flag instead of waiting for the
 * cross-CPU IPI to arrive. Use this option with caution.
 */
static void poll_idle(void)
{
	local_irq_enable();
	while (!need_resched())
		cpu_relax();
}

void default_idle(void)
{
	if (hlt_works()) {
		clear_thread_flag(TIF_POLLING_NRFLAG);
		smp_mb__after_clear_bit();

		set_bl_bit();
		if (!need_resched()) {
			local_irq_enable();
			cpu_sleep();
		} else
			local_irq_enable();

		set_thread_flag(TIF_POLLING_NRFLAG);
		clear_bl_bit();
	} else
		poll_idle();
}

/*
 * The idle thread. There's no useful work to be done, so just try to conserve
 * power and have a low exit latency (ie sit in a loop waiting for somebody to
 * say that they'd like to reschedule)
 */
void cpu_idle(void)
{
	unsigned int cpu = smp_processor_id();

	set_thread_flag(TIF_POLLING_NRFLAG);

	/* endless idle loop with no priority at all */
	while (1) {
		tick_nohz_stop_sched_tick(1);

		while (!need_resched()) {
			check_pgt_cache();
			rmb();

			if (cpu_is_offline(cpu))
				play_dead();

			local_irq_disable();
			/* Don't trace irqs off for idle */
			stop_critical_timings();
			pm_idle();
			/*
			 * Sanity check to ensure that pm_idle() returns
			 * with IRQs enabled
			 */
			WARN_ON(irqs_disabled());
			start_critical_timings();
		}

		tick_nohz_restart_sched_tick();
		preempt_enable_no_resched();
		schedule();
		preempt_disable();
	}
}

void __init select_idle_routine(void)
{
	/*
	 * If a platform has set its own idle routine, leave it alone.
	 */
	if (pm_idle)
		return;

	if (hlt_works())
		pm_idle = default_idle;
	else
		pm_idle = poll_idle;
}

static void do_nothing(void *unused)
{
}

void stop_this_cpu(void *unused)
{
	local_irq_disable();
	set_cpu_online(smp_processor_id(), false);

	for (;;)
		cpu_sleep();
}

/*
 * cpu_idle_wait - Used to ensure that all the CPUs discard old value of
 * pm_idle and update to new pm_idle value. Required while changing pm_idle
 * handler on SMP systems.
 *
 * Caller must have changed pm_idle to the new value before the call. Old
 * pm_idle value will not be used by any CPU after the return of this function.
 */
void cpu_idle_wait(void)
{
	smp_mb();
	/* kick all the CPUs so that they exit out of pm_idle */
	smp_call_function(do_nothing, NULL, 1);
}
EXPORT_SYMBOL_GPL(cpu_idle_wait);
