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

#include <linux/sched.h>
#include <linux/math64.h>
#include <linux/percpu.h>
#include <linux/irqflags.h>

#include <asm/cpufeature.h>
#include <asm/processor.h>

#ifdef CONFIG_SMP

static DEFINE_PER_CPU(struct aperfmperf, old_perf_sched);

static unsigned long scale_aperfmperf(void)
{
	struct aperfmperf val, *old = &__get_cpu_var(old_perf_sched);
	unsigned long ratio, flags;

	local_irq_save(flags);
	get_aperfmperf(&val);
	local_irq_restore(flags);

	ratio = calc_aperfmperf_ratio(old, &val);
	*old = val;

	return ratio;
}

unsigned long arch_scale_freq_power(struct sched_domain *sd, int cpu)
{
	/*
	 * do aperf/mperf on the cpu level because it includes things
	 * like turbo mode, which are relevant to full cores.
	 */
	if (boot_cpu_has(X86_FEATURE_APERFMPERF))
		return scale_aperfmperf();

	/*
	 * maybe have something cpufreq here
	 */

	return default_scale_freq_power(sd, cpu);
}

unsigned long arch_scale_smt_power(struct sched_domain *sd, int cpu)
{
	/*
	 * aperf/mperf already includes the smt gain
	 */
	if (boot_cpu_has(X86_FEATURE_APERFMPERF))
		return SCHED_LOAD_SCALE;

	return default_scale_smt_power(sd, cpu);
}

#endif
