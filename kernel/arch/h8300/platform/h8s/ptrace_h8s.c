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
 *  linux/arch/h8300/platform/h8s/ptrace_h8s.c
 *    ptrace cpu depend helper functions
 *
 *  Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file COPYING in the main directory of
 * this archive for more details.
 */

#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/ptrace.h>

#define CCR_MASK  0x6f
#define EXR_TRACE 0x80

/* Mapping from PT_xxx to the stack offset at which the register is
   saved.  Notice that usp has no stack-slot and needs to be treated
   specially (see get_reg/put_reg below). */
static const int h8300_register_offset[] = {
	PT_REG(er1), PT_REG(er2), PT_REG(er3), PT_REG(er4),
	PT_REG(er5), PT_REG(er6), PT_REG(er0), PT_REG(orig_er0),
	PT_REG(ccr), PT_REG(pc),  0,           PT_REG(exr)
};

/* read register */
long h8300_get_reg(struct task_struct *task, int regno)
{
	switch (regno) {
	case PT_USP:
		return task->thread.usp + sizeof(long)*2 + 2;
	case PT_CCR:
	case PT_EXR:
	    return *(unsigned short *)(task->thread.esp0 + h8300_register_offset[regno]);
	default:
	    return *(unsigned long *)(task->thread.esp0 + h8300_register_offset[regno]);
	}
}

/* write register */
int h8300_put_reg(struct task_struct *task, int regno, unsigned long data)
{
	unsigned short oldccr;
	switch (regno) {
	case PT_USP:
		task->thread.usp = data - sizeof(long)*2 - 2;
	case PT_CCR:
		oldccr = *(unsigned short *)(task->thread.esp0 + h8300_register_offset[regno]);
		oldccr &= ~CCR_MASK;
		data &= CCR_MASK;
		data |= oldccr;
		*(unsigned short *)(task->thread.esp0 + h8300_register_offset[regno]) = data;
		break;
	case PT_EXR:
		/* exr modify not support */
		return -EIO;
	default:
		*(unsigned long *)(task->thread.esp0 + h8300_register_offset[regno]) = data;
		break;
	}
	return 0;
}

/* disable singlestep */
void user_disable_single_step(struct task_struct *child)
{
	*(unsigned short *)(child->thread.esp0 + h8300_register_offset[PT_EXR]) &= ~EXR_TRACE;
}

/* enable singlestep */
void user_enable_single_step(struct task_struct *child)
{
	*(unsigned short *)(child->thread.esp0 + h8300_register_offset[PT_EXR]) |= EXR_TRACE;
}

asmlinkage void trace_trap(unsigned long bp)
{
	(void)bp;
	force_sig(SIGTRAP,current);
}

