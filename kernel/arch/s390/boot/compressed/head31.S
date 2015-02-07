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
 * Startup glue code to uncompress the kernel
 *
 * Copyright IBM Corp. 2010
 *
 *   Author(s):	Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#include <linux/init.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
#include <asm/page.h>
#include "sizes.h"

__HEAD
	.globl	startup_continue
startup_continue:
	basr	%r13,0			# get base
.LPG1:
	# setup stack
	l	%r15,.Lstack-.LPG1(%r13)
	ahi	%r15,-96
	l	%r1,.Ldecompress-.LPG1(%r13)
	basr	%r14,%r1
	# setup registers for memory mover & branch to target
	lr	%r4,%r2
	l	%r2,.Loffset-.LPG1(%r13)
	la	%r4,0(%r2,%r4)
	l	%r3,.Lmvsize-.LPG1(%r13)
	lr	%r5,%r3
	# move the memory mover someplace safe
	la	%r1,0x200
	mvc	0(mover_end-mover,%r1),mover-.LPG1(%r13)
	# decompress image is started at 0x11000
	lr	%r6,%r2
	br	%r1
mover:
	mvcle	%r2,%r4,0
	jo	mover
	br	%r6
mover_end:

	.align	8
.Lstack:
	.long	0x8000 + (1<<(PAGE_SHIFT+THREAD_ORDER))
.Ldecompress:
	.long	decompress_kernel
.Loffset:
	.long	0x11000
.Lmvsize:
	.long	SZ__bss_start
