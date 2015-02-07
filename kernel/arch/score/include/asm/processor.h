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

#ifndef _ASM_SCORE_PROCESSOR_H
#define _ASM_SCORE_PROCESSOR_H

#include <linux/cpumask.h>
#include <linux/threads.h>

#include <asm/segment.h>

struct task_struct;

/*
 * System setup and hardware flags..
 */
extern void (*cpu_wait)(void);

extern long kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);
extern unsigned long thread_saved_pc(struct task_struct *tsk);
extern void start_thread(struct pt_regs *regs,
			unsigned long pc, unsigned long sp);
extern unsigned long get_wchan(struct task_struct *p);

/*
 * Return current * instruction pointer ("program counter").
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l; })

#define cpu_relax()		barrier()
#define release_thread(thread)	do {} while (0)
#define prepare_to_copy(tsk)	do {} while (0)

/*
 * User space process size: 2GB. This is hardcoded into a few places,
 * so don't change it unless you know what you are doing.
 */
#define TASK_SIZE	0x7fff8000UL

/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define TASK_UNMAPPED_BASE	((TASK_SIZE / 3) & ~(PAGE_SIZE))

#ifdef __KERNEL__
#define STACK_TOP	TASK_SIZE
#define STACK_TOP_MAX	TASK_SIZE
#endif

/*
 * If you change thread_struct remember to change the #defines below too!
 */
struct thread_struct {
	unsigned long reg0, reg2, reg3;
	unsigned long reg12, reg13, reg14, reg15, reg16;
	unsigned long reg17, reg18, reg19, reg20, reg21;

	unsigned long cp0_psr;
	unsigned long cp0_ema;		/* Last user fault */
	unsigned long cp0_badvaddr;	/* Last user fault */
	unsigned long cp0_baduaddr;	/* Last kernel fault accessing USEG */
	unsigned long error_code;
	unsigned long trap_no;

	unsigned long mflags;
	unsigned long reg29;

	unsigned long single_step;
	unsigned long ss_nextcnt;

	unsigned long insn1_type;
	unsigned long addr1;
	unsigned long insn1;

	unsigned long insn2_type;
	unsigned long addr2;
	unsigned long insn2;

	mm_segment_t current_ds;
};

#define INIT_THREAD {						\
	.reg0			= 0,				\
	.reg2			= 0,				\
	.reg3			= 0,				\
	.reg12			= 0,				\
	.reg13			= 0,				\
	.reg14			= 0,				\
	.reg15			= 0,				\
	.reg16			= 0,				\
	.reg17			= 0,				\
	.reg18			= 0,				\
	.reg19			= 0,				\
	.reg20			= 0,				\
	.reg21			= 0,				\
	.cp0_psr		= 0,				\
	.error_code		= 0,				\
	.trap_no		= 0,				\
}

#define kstk_tos(tsk)		\
	((unsigned long)task_stack_page(tsk) + THREAD_SIZE - 32)
#define task_pt_regs(tsk)	((struct pt_regs *)kstk_tos(tsk) - 1)

#define KSTK_EIP(tsk)		(task_pt_regs(tsk)->cp0_epc)
#define KSTK_ESP(tsk)		(task_pt_regs(tsk)->regs[29])

#endif /* _ASM_SCORE_PROCESSOR_H */
