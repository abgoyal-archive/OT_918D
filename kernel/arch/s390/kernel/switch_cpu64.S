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
 * 64-bit switch cpu code
 *
 * Copyright IBM Corp. 2009
 *
 */

#include <asm/asm-offsets.h>
#include <asm/ptrace.h>

# smp_switch_to_cpu switches to destination cpu and executes the passed function
# Parameter: %r2 - function to call
#	     %r3 - function parameter
#	     %r4 - stack poiner
#	     %r5 - current cpu
#	     %r6 - destination cpu

	.section .text
	.align 4
	.globl smp_switch_to_cpu
smp_switch_to_cpu:
	stmg	%r6,%r15,__SF_GPRS(%r15)
	lgr	%r1,%r15
	aghi	%r15,-STACK_FRAME_OVERHEAD
	stg	%r1,__SF_BACKCHAIN(%r15)
	larl	%r1,.gprregs
	stmg	%r0,%r15,0(%r1)
1:	sigp	%r0,%r6,__SIGP_RESTART	/* start destination CPU */
	brc	2,1b			/* busy, try again */
2:	sigp	%r0,%r5,__SIGP_STOP	/* stop current CPU */
	brc	2,2b			/* busy, try again */
3:	j	3b

	.globl	smp_restart_cpu
smp_restart_cpu:
	larl	%r1,.gprregs
	lmg	%r0,%r15,0(%r1)
1:	sigp	%r0,%r5,__SIGP_SENSE	/* Wait for calling CPU */
	brc	10,1b			/* busy, accepted (status 0), running */
	tmll	%r0,0x40		/* Test if calling CPU is stopped */
	jz	1b
	ltgr	%r4,%r4			/* New stack ? */
	jz	1f
	lgr	%r15,%r4
1:	basr	%r14,%r2

	.section .data,"aw",@progbits
.gprregs:
	.rept	16
	.quad	0
	.endr
