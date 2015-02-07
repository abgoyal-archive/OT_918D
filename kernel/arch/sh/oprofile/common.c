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
 * arch/sh/oprofile/init.c
 *
 * Copyright (C) 2003 - 2008  Paul Mundt
 *
 * Based on arch/mips/oprofile/common.c:
 *
 *	Copyright (C) 2004, 2005 Ralf Baechle
 *	Copyright (C) 2005 MIPS Technologies, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/kernel.h>
#include <linux/oprofile.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/smp.h>
#include <asm/processor.h>
#include "op_impl.h"

static struct op_sh_model *model;

static struct op_counter_config ctr[20];

extern void sh_backtrace(struct pt_regs * const regs, unsigned int depth);

static int op_sh_setup(void)
{
	/* Pre-compute the values to stuff in the hardware registers.  */
	model->reg_setup(ctr);

	/* Configure the registers on all cpus.  */
	on_each_cpu(model->cpu_setup, NULL, 1);

        return 0;
}

static int op_sh_create_files(struct super_block *sb, struct dentry *root)
{
	int i, ret = 0;

	for (i = 0; i < model->num_counters; i++) {
		struct dentry *dir;
		char buf[4];

		snprintf(buf, sizeof(buf), "%d", i);
		dir = oprofilefs_mkdir(sb, root, buf);

		ret |= oprofilefs_create_ulong(sb, dir, "enabled", &ctr[i].enabled);
		ret |= oprofilefs_create_ulong(sb, dir, "event", &ctr[i].event);
		ret |= oprofilefs_create_ulong(sb, dir, "kernel", &ctr[i].kernel);
		ret |= oprofilefs_create_ulong(sb, dir, "user", &ctr[i].user);

		if (model->create_files)
			ret |= model->create_files(sb, dir);
		else
			ret |= oprofilefs_create_ulong(sb, dir, "count", &ctr[i].count);

		/* Dummy entries */
		ret |= oprofilefs_create_ulong(sb, dir, "unit_mask", &ctr[i].unit_mask);
	}

	return ret;
}

static int op_sh_start(void)
{
	/* Enable performance monitoring for all counters.  */
	on_each_cpu(model->cpu_start, NULL, 1);

	return 0;
}

static void op_sh_stop(void)
{
	/* Disable performance monitoring for all counters.  */
	on_each_cpu(model->cpu_stop, NULL, 1);
}

int __init oprofile_arch_init(struct oprofile_operations *ops)
{
	struct op_sh_model *lmodel = NULL;
	int ret;

	/*
	 * Always assign the backtrace op. If the counter initialization
	 * fails, we fall back to the timer which will still make use of
	 * this.
	 */
	ops->backtrace = sh_backtrace;

	/*
	 * XXX
	 *
	 * All of the SH7750/SH-4A counters have been converted to perf,
	 * this infrastructure hook is left for other users until they've
	 * had a chance to convert over, at which point all of this
	 * will be deleted.
	 */

	if (!lmodel)
		return -ENODEV;
	if (!(current_cpu_data.flags & CPU_HAS_PERF_COUNTER))
		return -ENODEV;

	ret = lmodel->init();
	if (unlikely(ret != 0))
		return ret;

	model = lmodel;

	ops->setup		= op_sh_setup;
	ops->create_files	= op_sh_create_files;
	ops->start		= op_sh_start;
	ops->stop		= op_sh_stop;
	ops->cpu_type		= lmodel->cpu_type;

	printk(KERN_INFO "oprofile: using %s performance monitoring.\n",
	       lmodel->cpu_type);

	return 0;
}

void oprofile_arch_exit(void)
{
	if (model && model->exit)
		model->exit();
}
