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

/* kernel atomic64 operations
 *
 * For an explanation of how atomic ops work in this arch, see:
 *   Documentation/frv/atomic-ops.txt
 *
 * Copyright (C) 2009 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/spr-regs.h>

	.text
	.balign 4


###############################################################################
#
# long long atomic64_inc_return(atomic64_t *v)
#
###############################################################################
	.globl		atomic64_inc_return
        .type		atomic64_inc_return,@function
atomic64_inc_return:
	or.p		gr8,gr8,gr10
0:
	orcc		gr0,gr0,gr0,icc3		/* set ICC3.Z */
	ckeq		icc3,cc7
	ldd.p		@(gr10,gr0),gr8			/* LDD.P/ORCR must be atomic */
	orcr		cc7,cc7,cc3			/* set CC3 to true */
	addicc		gr9,#1,gr9,icc0
	addxi		gr8,#0,gr8,icc0
	cstd.p		gr8,@(gr10,gr0)		,cc3,#1
	corcc		gr29,gr29,gr0		,cc3,#1	/* clear ICC3.Z if store happens */
	beq		icc3,#0,0b
	bralr

	.size		atomic64_inc_return, .-atomic64_inc_return

###############################################################################
#
# long long atomic64_dec_return(atomic64_t *v)
#
###############################################################################
	.globl		atomic64_dec_return
        .type		atomic64_dec_return,@function
atomic64_dec_return:
	or.p		gr8,gr8,gr10
0:
	orcc		gr0,gr0,gr0,icc3		/* set ICC3.Z */
	ckeq		icc3,cc7
	ldd.p		@(gr10,gr0),gr8			/* LDD.P/ORCR must be atomic */
	orcr		cc7,cc7,cc3			/* set CC3 to true */
	subicc		gr9,#1,gr9,icc0
	subxi		gr8,#0,gr8,icc0
	cstd.p		gr8,@(gr10,gr0)		,cc3,#1
	corcc		gr29,gr29,gr0		,cc3,#1	/* clear ICC3.Z if store happens */
	beq		icc3,#0,0b
	bralr

	.size		atomic64_dec_return, .-atomic64_dec_return

###############################################################################
#
# long long atomic64_add_return(long long i, atomic64_t *v)
#
###############################################################################
	.globl		atomic64_add_return
        .type		atomic64_add_return,@function
atomic64_add_return:
	or.p		gr8,gr8,gr4
	or		gr9,gr9,gr5
0:
	orcc		gr0,gr0,gr0,icc3		/* set ICC3.Z */
	ckeq		icc3,cc7
	ldd.p		@(gr10,gr0),gr8			/* LDD.P/ORCR must be atomic */
	orcr		cc7,cc7,cc3			/* set CC3 to true */
	addcc		gr9,gr5,gr9,icc0
	addx		gr8,gr4,gr8,icc0
	cstd.p		gr8,@(gr10,gr0)		,cc3,#1
	corcc		gr29,gr29,gr0		,cc3,#1	/* clear ICC3.Z if store happens */
	beq		icc3,#0,0b
	bralr

	.size		atomic64_add_return, .-atomic64_add_return

###############################################################################
#
# long long atomic64_sub_return(long long i, atomic64_t *v)
#
###############################################################################
	.globl		atomic64_sub_return
        .type		atomic64_sub_return,@function
atomic64_sub_return:
	or.p		gr8,gr8,gr4
	or		gr9,gr9,gr5
0:
	orcc		gr0,gr0,gr0,icc3		/* set ICC3.Z */
	ckeq		icc3,cc7
	ldd.p		@(gr10,gr0),gr8			/* LDD.P/ORCR must be atomic */
	orcr		cc7,cc7,cc3			/* set CC3 to true */
	subcc		gr9,gr5,gr9,icc0
	subx		gr8,gr4,gr8,icc0
	cstd.p		gr8,@(gr10,gr0)		,cc3,#1
	corcc		gr29,gr29,gr0		,cc3,#1	/* clear ICC3.Z if store happens */
	beq		icc3,#0,0b
	bralr

	.size		atomic64_sub_return, .-atomic64_sub_return

###############################################################################
#
# uint64_t __xchg_64(uint64_t i, uint64_t *v)
#
###############################################################################
	.globl		__xchg_64
        .type		__xchg_64,@function
__xchg_64:
	or.p		gr8,gr8,gr4
	or		gr9,gr9,gr5
0:
	orcc		gr0,gr0,gr0,icc3		/* set ICC3.Z */
	ckeq		icc3,cc7
	ldd.p		@(gr10,gr0),gr8			/* LDD.P/ORCR must be atomic */
	orcr		cc7,cc7,cc3			/* set CC3 to true */
	cstd.p		gr4,@(gr10,gr0)		,cc3,#1
	corcc		gr29,gr29,gr0		,cc3,#1	/* clear ICC3.Z if store happens */
	beq		icc3,#0,0b
	bralr

	.size		__xchg_64, .-__xchg_64

###############################################################################
#
# uint64_t __cmpxchg_64(uint64_t test, uint64_t new, uint64_t *v)
#
###############################################################################
	.globl		__cmpxchg_64
        .type		__cmpxchg_64,@function
__cmpxchg_64:
	or.p		gr8,gr8,gr4
	or		gr9,gr9,gr5
0:
	orcc		gr0,gr0,gr0,icc3		/* set ICC3.Z */
	ckeq		icc3,cc7
	ldd.p		@(gr12,gr0),gr8			/* LDD.P/ORCR must be atomic */
	orcr		cc7,cc7,cc3
	subcc		gr8,gr4,gr0,icc0
	subcc.p		gr9,gr5,gr0,icc1
	bnelr		icc0,#0
	bnelr		icc1,#0
	cstd.p		gr10,@(gr12,gr0)	,cc3,#1
	corcc		gr29,gr29,gr0		,cc3,#1	/* clear ICC3.Z if store happens */
	beq		icc3,#0,0b
	bralr

	.size		__cmpxchg_64, .-__cmpxchg_64

