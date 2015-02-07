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
 * Loongson2 performance counter driver for oprofile
 *
 * Copyright (C) 2009 Lemote Inc.
 * Author: Yanhua <yanh@lemote.com>
 * Author: Wu Zhangjin <wuzhangjin@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/oprofile.h>
#include <linux/interrupt.h>

#include <loongson.h>			/* LOONGSON2_PERFCNT_IRQ */
#include "op_impl.h"

#define LOONGSON2_CPU_TYPE	"mips/loongson2"

#define LOONGSON2_PERFCNT_OVERFLOW		(1ULL   << 31)

#define LOONGSON2_PERFCTRL_EXL			(1UL	<<  0)
#define LOONGSON2_PERFCTRL_KERNEL		(1UL    <<  1)
#define LOONGSON2_PERFCTRL_SUPERVISOR		(1UL    <<  2)
#define LOONGSON2_PERFCTRL_USER			(1UL    <<  3)
#define LOONGSON2_PERFCTRL_ENABLE		(1UL    <<  4)
#define LOONGSON2_PERFCTRL_EVENT(idx, event) \
	(((event) & 0x0f) << ((idx) ? 9 : 5))

#define read_c0_perfctrl() __read_64bit_c0_register($24, 0)
#define write_c0_perfctrl(val) __write_64bit_c0_register($24, 0, val)
#define read_c0_perfcnt() __read_64bit_c0_register($25, 0)
#define write_c0_perfcnt(val) __write_64bit_c0_register($25, 0, val)

static struct loongson2_register_config {
	unsigned int ctrl;
	unsigned long long reset_counter1;
	unsigned long long reset_counter2;
	int cnt1_enabled, cnt2_enabled;
} reg;

static char *oprofid = "LoongsonPerf";
static irqreturn_t loongson2_perfcount_handler(int irq, void *dev_id);

static void reset_counters(void *arg)
{
	write_c0_perfctrl(0);
	write_c0_perfcnt(0);
}

static void loongson2_reg_setup(struct op_counter_config *cfg)
{
	unsigned int ctrl = 0;

	reg.reset_counter1 = 0;
	reg.reset_counter2 = 0;

	/*
	 * Compute the performance counter ctrl word.
	 * For now, count kernel and user mode.
	 */
	if (cfg[0].enabled) {
		ctrl |= LOONGSON2_PERFCTRL_EVENT(0, cfg[0].event);
		reg.reset_counter1 = 0x80000000ULL - cfg[0].count;
	}

	if (cfg[1].enabled) {
		ctrl |= LOONGSON2_PERFCTRL_EVENT(1, cfg[1].event);
		reg.reset_counter2 = 0x80000000ULL - cfg[1].count;
	}

	if (cfg[0].enabled || cfg[1].enabled) {
		ctrl |= LOONGSON2_PERFCTRL_EXL | LOONGSON2_PERFCTRL_ENABLE;
		if (cfg[0].kernel || cfg[1].kernel)
			ctrl |= LOONGSON2_PERFCTRL_KERNEL;
		if (cfg[0].user || cfg[1].user)
			ctrl |= LOONGSON2_PERFCTRL_USER;
	}

	reg.ctrl = ctrl;

	reg.cnt1_enabled = cfg[0].enabled;
	reg.cnt2_enabled = cfg[1].enabled;
}

static void loongson2_cpu_setup(void *args)
{
	write_c0_perfcnt((reg.reset_counter2 << 32) | reg.reset_counter1);
}

static void loongson2_cpu_start(void *args)
{
	/* Start all counters on current CPU */
	if (reg.cnt1_enabled || reg.cnt2_enabled)
		write_c0_perfctrl(reg.ctrl);
}

static void loongson2_cpu_stop(void *args)
{
	/* Stop all counters on current CPU */
	write_c0_perfctrl(0);
	memset(&reg, 0, sizeof(reg));
}

static irqreturn_t loongson2_perfcount_handler(int irq, void *dev_id)
{
	uint64_t counter, counter1, counter2;
	struct pt_regs *regs = get_irq_regs();
	int enabled;

	/* Check whether the irq belongs to me */
	enabled = read_c0_perfctrl() & LOONGSON2_PERFCTRL_ENABLE;
	if (!enabled)
		return IRQ_NONE;
	enabled = reg.cnt1_enabled | reg.cnt2_enabled;
	if (!enabled)
		return IRQ_NONE;

	counter = read_c0_perfcnt();
	counter1 = counter & 0xffffffff;
	counter2 = counter >> 32;

	if (counter1 & LOONGSON2_PERFCNT_OVERFLOW) {
		if (reg.cnt1_enabled)
			oprofile_add_sample(regs, 0);
		counter1 = reg.reset_counter1;
	}
	if (counter2 & LOONGSON2_PERFCNT_OVERFLOW) {
		if (reg.cnt2_enabled)
			oprofile_add_sample(regs, 1);
		counter2 = reg.reset_counter2;
	}

	write_c0_perfcnt((counter2 << 32) | counter1);

	return IRQ_HANDLED;
}

static int __init loongson2_init(void)
{
	return request_irq(LOONGSON2_PERFCNT_IRQ, loongson2_perfcount_handler,
			   IRQF_SHARED, "Perfcounter", oprofid);
}

static void loongson2_exit(void)
{
	reset_counters(NULL);
	free_irq(LOONGSON2_PERFCNT_IRQ, oprofid);
}

struct op_mips_model op_model_loongson2_ops = {
	.reg_setup = loongson2_reg_setup,
	.cpu_setup = loongson2_cpu_setup,
	.init = loongson2_init,
	.exit = loongson2_exit,
	.cpu_start = loongson2_cpu_start,
	.cpu_stop = loongson2_cpu_stop,
	.cpu_type = LOONGSON2_CPU_TYPE,
	.num_counters = 2
};
