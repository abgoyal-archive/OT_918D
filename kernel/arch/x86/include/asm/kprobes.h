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

#ifndef _ASM_X86_KPROBES_H
#define _ASM_X86_KPROBES_H
/*
 *  Kernel Probes (KProbes)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) IBM Corporation, 2002, 2004
 *
 * See arch/x86/kernel/kprobes.c for x86 kprobes history.
 */
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/percpu.h>
#include <asm/insn.h>

#define  __ARCH_WANT_KPROBES_INSN_SLOT

struct pt_regs;
struct kprobe;

typedef u8 kprobe_opcode_t;
#define BREAKPOINT_INSTRUCTION	0xcc
#define RELATIVEJUMP_OPCODE 0xe9
#define RELATIVEJUMP_SIZE 5
#define RELATIVECALL_OPCODE 0xe8
#define RELATIVE_ADDR_SIZE 4
#define MAX_STACK_SIZE 64
#define MIN_STACK_SIZE(ADDR)					       \
	(((MAX_STACK_SIZE) < (((unsigned long)current_thread_info()) + \
			      THREAD_SIZE - (unsigned long)(ADDR)))    \
	 ? (MAX_STACK_SIZE)					       \
	 : (((unsigned long)current_thread_info()) +		       \
	    THREAD_SIZE - (unsigned long)(ADDR)))

#define flush_insn_slot(p)	do { } while (0)

/* optinsn template addresses */
extern kprobe_opcode_t optprobe_template_entry;
extern kprobe_opcode_t optprobe_template_val;
extern kprobe_opcode_t optprobe_template_call;
extern kprobe_opcode_t optprobe_template_end;
#define MAX_OPTIMIZED_LENGTH (MAX_INSN_SIZE + RELATIVE_ADDR_SIZE)
#define MAX_OPTINSN_SIZE 				\
	(((unsigned long)&optprobe_template_end -	\
	  (unsigned long)&optprobe_template_entry) +	\
	 MAX_OPTIMIZED_LENGTH + RELATIVEJUMP_SIZE)

extern const int kretprobe_blacklist_size;

void arch_remove_kprobe(struct kprobe *p);
void kretprobe_trampoline(void);

/* Architecture specific copy of original instruction*/
struct arch_specific_insn {
	/* copy of the original instruction */
	kprobe_opcode_t *insn;
	/*
	 * boostable = -1: This instruction type is not boostable.
	 * boostable = 0: This instruction type is boostable.
	 * boostable = 1: This instruction has been boosted: we have
	 * added a relative jump after the instruction copy in insn,
	 * so no single-step and fixup are needed (unless there's
	 * a post_handler or break_handler).
	 */
	int boostable;
};

struct arch_optimized_insn {
	/* copy of the original instructions */
	kprobe_opcode_t copied_insn[RELATIVE_ADDR_SIZE];
	/* detour code buffer */
	kprobe_opcode_t *insn;
	/* the size of instructions copied to detour code buffer */
	size_t size;
};

/* Return true (!0) if optinsn is prepared for optimization. */
static inline int arch_prepared_optinsn(struct arch_optimized_insn *optinsn)
{
	return optinsn->size;
}

struct prev_kprobe {
	struct kprobe *kp;
	unsigned long status;
	unsigned long old_flags;
	unsigned long saved_flags;
};

/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned long kprobe_status;
	unsigned long kprobe_old_flags;
	unsigned long kprobe_saved_flags;
	unsigned long *jprobe_saved_sp;
	struct pt_regs jprobe_saved_regs;
	kprobe_opcode_t jprobes_stack[MAX_STACK_SIZE];
	struct prev_kprobe prev_kprobe;
};

extern int kprobe_fault_handler(struct pt_regs *regs, int trapnr);
extern int kprobe_exceptions_notify(struct notifier_block *self,
				    unsigned long val, void *data);
#endif /* _ASM_X86_KPROBES_H */
