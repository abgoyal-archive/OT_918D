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
 * Copyright 2007-2009 Analog Devices Inc.
 *               Philippe Gerum <rpm@xenomai.org>
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <asm/smp.h>
#include <asm/dma.h>
#include <asm/time.h>

static DEFINE_SPINLOCK(boot_lock);

/*
 * platform_init_cpus() - Tell the world about how many cores we
 * have. This is called while setting up the architecture support
 * (setup_arch()), so don't be too demanding here with respect to
 * available kernel services.
 */

void __init platform_init_cpus(void)
{
	cpu_set(0, cpu_possible_map); /* CoreA */
	cpu_set(1, cpu_possible_map); /* CoreB */
}

void __init platform_prepare_cpus(unsigned int max_cpus)
{
	int len;

	len = &coreb_trampoline_end - &coreb_trampoline_start + 1;
	BUG_ON(len > L1_CODE_LENGTH);

	dma_memcpy((void *)COREB_L1_CODE_START, &coreb_trampoline_start, len);

	/* Both cores ought to be present on a bf561! */
	cpu_set(0, cpu_present_map); /* CoreA */
	cpu_set(1, cpu_present_map); /* CoreB */

	printk(KERN_INFO "CoreB bootstrap code to SRAM %p via DMA.\n", (void *)COREB_L1_CODE_START);
}

int __init setup_profiling_timer(unsigned int multiplier) /* not supported */
{
	return -EINVAL;
}

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	/* Clone setup for peripheral interrupt sources from CoreA. */
	bfin_write_SICB_IMASK0(bfin_read_SICA_IMASK0());
	bfin_write_SICB_IMASK1(bfin_read_SICA_IMASK1());
	SSYNC();

	/* Clone setup for IARs from CoreA. */
	bfin_write_SICB_IAR0(bfin_read_SICA_IAR0());
	bfin_write_SICB_IAR1(bfin_read_SICA_IAR1());
	bfin_write_SICB_IAR2(bfin_read_SICA_IAR2());
	bfin_write_SICB_IAR3(bfin_read_SICA_IAR3());
	bfin_write_SICB_IAR4(bfin_read_SICA_IAR4());
	bfin_write_SICB_IAR5(bfin_read_SICA_IAR5());
	bfin_write_SICB_IAR6(bfin_read_SICA_IAR6());
	bfin_write_SICB_IAR7(bfin_read_SICA_IAR7());
	bfin_write_SICB_IWR0(IWR_DISABLE_ALL);
	bfin_write_SICB_IWR1(IWR_DISABLE_ALL);
	SSYNC();

	/* Store CPU-private information to the cpu_data array. */
	bfin_setup_cpudata(cpu);

	/* We are done with local CPU inits, unblock the boot CPU. */
	set_cpu_online(cpu, true);
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

int __cpuinit platform_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;

	printk(KERN_INFO "Booting Core B.\n");

	spin_lock(&boot_lock);

	if ((bfin_read_SICA_SYSCR() & COREB_SRAM_INIT) == 0) {
		/* CoreB already running, sending ipi to wakeup it */
		platform_send_ipi_cpu(cpu, IRQ_SUPPLE_0);
	} else {
		/* Kick CoreB, which should start execution from CORE_SRAM_BASE. */
		bfin_write_SICA_SYSCR(bfin_read_SICA_SYSCR() & ~COREB_SRAM_INIT);
		SSYNC();
	}

	timeout = jiffies + 1 * HZ;
	while (time_before(jiffies, timeout)) {
		if (cpu_online(cpu))
			break;
		udelay(100);
		barrier();
	}

	if (cpu_online(cpu)) {
		/* release the lock and let coreb run */
		spin_unlock(&boot_lock);
		return 0;
	} else
		panic("CPU%u: processor failed to boot\n", cpu);
}

void __init platform_request_ipi(irq_handler_t handler)
{
	int ret;

	ret = request_irq(IRQ_SUPPLE_0, handler, IRQF_DISABLED,
			  "Supplemental Interrupt0", handler);
	if (ret)
		panic("Cannot request supplemental interrupt 0 for IPI service");
}

void platform_send_ipi(cpumask_t callmap)
{
	unsigned int cpu;

	for_each_cpu_mask(cpu, callmap) {
		BUG_ON(cpu >= 2);
		SSYNC();
		bfin_write_SICB_SYSCR(bfin_read_SICB_SYSCR() | (1 << (6 + cpu)));
		SSYNC();
	}
}

void platform_send_ipi_cpu(unsigned int cpu)
{
	BUG_ON(cpu >= 2);
	SSYNC();
	bfin_write_SICB_SYSCR(bfin_read_SICB_SYSCR() | (1 << (6 + cpu)));
	SSYNC();
}

void platform_clear_ipi(unsigned int cpu)
{
	BUG_ON(cpu >= 2);
	SSYNC();
	bfin_write_SICB_SYSCR(bfin_read_SICB_SYSCR() | (1 << (10 + cpu)));
	SSYNC();
}

/*
 * Setup core B's local core timer.
 * In SMP, core timer is used for clock event device.
 */
void __cpuinit bfin_local_timer_setup(void)
{
#if defined(CONFIG_TICKSOURCE_CORETMR)
	bfin_coretmr_init();
	bfin_coretmr_clockevent_init();
	get_irq_chip(IRQ_CORETMR)->unmask(IRQ_CORETMR);
#else
	/* Power down the core timer, just to play safe. */
	bfin_write_TCNTL(0);
#endif

}
