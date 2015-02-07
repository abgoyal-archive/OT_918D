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
 * atomic64_t for 386/486
 *
 * Copyright © 2010  Luca Barbieri
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/linkage.h>
#include <asm/alternative-asm.h>
#include <asm/dwarf2.h>

/* if you want SMP support, implement these with real spinlocks */
.macro LOCK reg
	pushfl
	CFI_ADJUST_CFA_OFFSET 4
	cli
.endm

.macro UNLOCK reg
	popfl
	CFI_ADJUST_CFA_OFFSET -4
.endm

#define BEGIN(op) \
.macro endp; \
	CFI_ENDPROC; \
ENDPROC(atomic64_##op##_386); \
.purgem endp; \
.endm; \
ENTRY(atomic64_##op##_386); \
	CFI_STARTPROC; \
	LOCK v;

#define ENDP endp

#define RET \
	UNLOCK v; \
	ret

#define RET_ENDP \
	RET; \
	ENDP

#define v %ecx
BEGIN(read)
	movl  (v), %eax
	movl 4(v), %edx
RET_ENDP
#undef v

#define v %esi
BEGIN(set)
	movl %ebx,  (v)
	movl %ecx, 4(v)
RET_ENDP
#undef v

#define v  %esi
BEGIN(xchg)
	movl  (v), %eax
	movl 4(v), %edx
	movl %ebx,  (v)
	movl %ecx, 4(v)
RET_ENDP
#undef v

#define v %ecx
BEGIN(add)
	addl %eax,  (v)
	adcl %edx, 4(v)
RET_ENDP
#undef v

#define v %ecx
BEGIN(add_return)
	addl  (v), %eax
	adcl 4(v), %edx
	movl %eax,  (v)
	movl %edx, 4(v)
RET_ENDP
#undef v

#define v %ecx
BEGIN(sub)
	subl %eax,  (v)
	sbbl %edx, 4(v)
RET_ENDP
#undef v

#define v %ecx
BEGIN(sub_return)
	negl %edx
	negl %eax
	sbbl $0, %edx
	addl  (v), %eax
	adcl 4(v), %edx
	movl %eax,  (v)
	movl %edx, 4(v)
RET_ENDP
#undef v

#define v %esi
BEGIN(inc)
	addl $1,  (v)
	adcl $0, 4(v)
RET_ENDP
#undef v

#define v %esi
BEGIN(inc_return)
	movl  (v), %eax
	movl 4(v), %edx
	addl $1, %eax
	adcl $0, %edx
	movl %eax,  (v)
	movl %edx, 4(v)
RET_ENDP
#undef v

#define v %esi
BEGIN(dec)
	subl $1,  (v)
	sbbl $0, 4(v)
RET_ENDP
#undef v

#define v %esi
BEGIN(dec_return)
	movl  (v), %eax
	movl 4(v), %edx
	subl $1, %eax
	sbbl $0, %edx
	movl %eax,  (v)
	movl %edx, 4(v)
RET_ENDP
#undef v

#define v %ecx
BEGIN(add_unless)
	addl %eax, %esi
	adcl %edx, %edi
	addl  (v), %eax
	adcl 4(v), %edx
	cmpl %eax, %esi
	je 3f
1:
	movl %eax,  (v)
	movl %edx, 4(v)
	movl $1, %eax
2:
	RET
3:
	cmpl %edx, %edi
	jne 1b
	xorl %eax, %eax
	jmp 2b
ENDP
#undef v

#define v %esi
BEGIN(inc_not_zero)
	movl  (v), %eax
	movl 4(v), %edx
	testl %eax, %eax
	je 3f
1:
	addl $1, %eax
	adcl $0, %edx
	movl %eax,  (v)
	movl %edx, 4(v)
	movl $1, %eax
2:
	RET
3:
	testl %edx, %edx
	jne 1b
	jmp 2b
ENDP
#undef v

#define v %esi
BEGIN(dec_if_positive)
	movl  (v), %eax
	movl 4(v), %edx
	subl $1, %eax
	sbbl $0, %edx
	js 1f
	movl %eax,  (v)
	movl %edx, 4(v)
1:
RET_ENDP
#undef v
