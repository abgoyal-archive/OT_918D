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
 * x86-64 rwsem wrappers
 *
 * This interfaces the inline asm code to the slow-path
 * C routines. We need to save the call-clobbered regs
 * that the asm does not mark as clobbered, and move the
 * argument from %rax to %rdi.
 *
 * NOTE! We don't need to save %rax, because the functions
 * will always return the semaphore pointer in %rax (which
 * is also the input argument to these helpers)
 *
 * The following can clobber %rdx because the asm clobbers it:
 *   call_rwsem_down_write_failed
 *   call_rwsem_wake
 * but %rdi, %rsi, %rcx, %r8-r11 always need saving.
 */

#include <linux/linkage.h>
#include <asm/rwlock.h>
#include <asm/alternative-asm.h>
#include <asm/frame.h>
#include <asm/dwarf2.h>

#define save_common_regs \
	pushq %rdi; \
	pushq %rsi; \
	pushq %rcx; \
	pushq %r8; \
	pushq %r9; \
	pushq %r10; \
	pushq %r11

#define restore_common_regs \
	popq %r11; \
	popq %r10; \
	popq %r9; \
	popq %r8; \
	popq %rcx; \
	popq %rsi; \
	popq %rdi

/* Fix up special calling conventions */
ENTRY(call_rwsem_down_read_failed)
	save_common_regs
	pushq %rdx
	movq %rax,%rdi
	call rwsem_down_read_failed
	popq %rdx
	restore_common_regs
	ret
	ENDPROC(call_rwsem_down_read_failed)

ENTRY(call_rwsem_down_write_failed)
	save_common_regs
	movq %rax,%rdi
	call rwsem_down_write_failed
	restore_common_regs
	ret
	ENDPROC(call_rwsem_down_write_failed)

ENTRY(call_rwsem_wake)
	decl %edx	/* do nothing if still outstanding active readers */
	jnz 1f
	save_common_regs
	movq %rax,%rdi
	call rwsem_wake
	restore_common_regs
1:	ret
	ENDPROC(call_rwsem_wake)

/* Fix up special calling conventions */
ENTRY(call_rwsem_downgrade_wake)
	save_common_regs
	pushq %rdx
	movq %rax,%rdi
	call rwsem_downgrade_wake
	popq %rdx
	restore_common_regs
	ret
	ENDPROC(call_rwsem_downgrade_wake)
