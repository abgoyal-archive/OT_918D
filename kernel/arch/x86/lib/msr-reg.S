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

#include <linux/linkage.h>
#include <linux/errno.h>
#include <asm/dwarf2.h>
#include <asm/asm.h>
#include <asm/msr.h>

#ifdef CONFIG_X86_64
/*
 * int native_{rdmsr,wrmsr}_safe_regs(u32 gprs[8]);
 *
 * reg layout: u32 gprs[eax, ecx, edx, ebx, esp, ebp, esi, edi]
 *
 */
.macro op_safe_regs op
ENTRY(native_\op\()_safe_regs)
	CFI_STARTPROC
	pushq_cfi %rbx
	pushq_cfi %rbp
	movq	%rdi, %r10	/* Save pointer */
	xorl	%r11d, %r11d	/* Return value */
	movl    (%rdi), %eax
	movl    4(%rdi), %ecx
	movl    8(%rdi), %edx
	movl    12(%rdi), %ebx
	movl    20(%rdi), %ebp
	movl    24(%rdi), %esi
	movl    28(%rdi), %edi
	CFI_REMEMBER_STATE
1:	\op
2:	movl    %eax, (%r10)
	movl	%r11d, %eax	/* Return value */
	movl    %ecx, 4(%r10)
	movl    %edx, 8(%r10)
	movl    %ebx, 12(%r10)
	movl    %ebp, 20(%r10)
	movl    %esi, 24(%r10)
	movl    %edi, 28(%r10)
	popq_cfi %rbp
	popq_cfi %rbx
	ret
3:
	CFI_RESTORE_STATE
	movl    $-EIO, %r11d
	jmp     2b

	_ASM_EXTABLE(1b, 3b)
	CFI_ENDPROC
ENDPROC(native_\op\()_safe_regs)
.endm

#else /* X86_32 */

.macro op_safe_regs op
ENTRY(native_\op\()_safe_regs)
	CFI_STARTPROC
	pushl_cfi %ebx
	pushl_cfi %ebp
	pushl_cfi %esi
	pushl_cfi %edi
	pushl_cfi $0              /* Return value */
	pushl_cfi %eax
	movl    4(%eax), %ecx
	movl    8(%eax), %edx
	movl    12(%eax), %ebx
	movl    20(%eax), %ebp
	movl    24(%eax), %esi
	movl    28(%eax), %edi
	movl    (%eax), %eax
	CFI_REMEMBER_STATE
1:	\op
2:	pushl_cfi %eax
	movl    4(%esp), %eax
	popl_cfi (%eax)
	addl    $4, %esp
	CFI_ADJUST_CFA_OFFSET -4
	movl    %ecx, 4(%eax)
	movl    %edx, 8(%eax)
	movl    %ebx, 12(%eax)
	movl    %ebp, 20(%eax)
	movl    %esi, 24(%eax)
	movl    %edi, 28(%eax)
	popl_cfi %eax
	popl_cfi %edi
	popl_cfi %esi
	popl_cfi %ebp
	popl_cfi %ebx
	ret
3:
	CFI_RESTORE_STATE
	movl    $-EIO, 4(%esp)
	jmp     2b

	_ASM_EXTABLE(1b, 3b)
	CFI_ENDPROC
ENDPROC(native_\op\()_safe_regs)
.endm

#endif

op_safe_regs rdmsr
op_safe_regs wrmsr

