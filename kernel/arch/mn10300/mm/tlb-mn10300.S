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

###############################################################################
#
# TLB loading functions
#
# Copyright (C) 2007 Matsushita Electric Industrial Co., Ltd.
# Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
# Modified by David Howells (dhowells@redhat.com)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public Licence
# as published by the Free Software Foundation; either version
# 2 of the Licence, or (at your option) any later version.
#
###############################################################################
#include <linux/sys.h>
#include <linux/linkage.h>
#include <asm/smp.h>
#include <asm/intctl-regs.h>
#include <asm/frame.inc>
#include <asm/page.h>
#include <asm/pgtable.h>

###############################################################################
#
# Instruction TLB Miss handler entry point
#
###############################################################################
	.type	itlb_miss,@function
ENTRY(itlb_miss)
	and	~EPSW_NMID,epsw
#ifdef CONFIG_GDBSTUB
	movm	[d2,d3,a2],(sp)
#else
	or	EPSW_nAR,epsw		# switch D0-D3 & A0-A3 to the alternate
					# register bank
	nop
	nop
	nop
#endif

	mov	(IPTEU),d3
	mov	(PTBR),a2
	mov	d3,d2
	and	0xffc00000,d2
	lsr	20,d2
	mov	(a2,d2),a2		# PTD *ptd = PGD[addr 31..22]
	btst	_PAGE_VALID,a2
	beq	itlb_miss_fault		# jump if doesn't point anywhere

	and	~(PAGE_SIZE-1),a2
	mov	d3,d2
	and	0x003ff000,d2
	lsr	10,d2
	add	d2,a2
	mov	(a2),d2			# get pte from PTD[addr 21..12]
	btst	_PAGE_VALID,d2
	beq	itlb_miss_fault		# jump if doesn't point to a page
					# (might be a swap id)
	bset	_PAGE_ACCESSED,(0,a2)
	and	~(xPTEL_UNUSED1|xPTEL_UNUSED2),d2
itlb_miss_set:
	mov	d2,(IPTEL)		# change the TLB
#ifdef CONFIG_GDBSTUB
	movm	(sp),[d2,d3,a2]
#endif
	rti

itlb_miss_fault:
	mov	_PAGE_VALID,d2		# force address error handler to be
					# invoked
	bra	itlb_miss_set

	.size	itlb_miss, . - itlb_miss

###############################################################################
#
# Data TLB Miss handler entry point
#
###############################################################################
	.type	dtlb_miss,@function
ENTRY(dtlb_miss)
	and	~EPSW_NMID,epsw
#ifdef CONFIG_GDBSTUB
	movm	[d2,d3,a2],(sp)
#else
	or	EPSW_nAR,epsw		# switch D0-D3 & A0-A3 to the alternate
					# register bank
	nop
	nop
	nop
#endif

	mov	(DPTEU),d3
	mov	(PTBR),a2
	mov	d3,d2
	and	0xffc00000,d2
	lsr	20,d2
	mov	(a2,d2),a2		# PTD *ptd = PGD[addr 31..22]
	btst	_PAGE_VALID,a2
	beq	dtlb_miss_fault		# jump if doesn't point anywhere

	and	~(PAGE_SIZE-1),a2
	mov	d3,d2
	and	0x003ff000,d2
	lsr	10,d2
	add	d2,a2
	mov	(a2),d2			# get pte from PTD[addr 21..12]
	btst	_PAGE_VALID,d2
	beq	dtlb_miss_fault		# jump if doesn't point to a page
					# (might be a swap id)
	bset	_PAGE_ACCESSED,(0,a2)
	and	~(xPTEL_UNUSED1|xPTEL_UNUSED2),d2
dtlb_miss_set:
	mov	d2,(DPTEL)		# change the TLB
#ifdef CONFIG_GDBSTUB
	movm	(sp),[d2,d3,a2]
#endif
	rti

dtlb_miss_fault:
	mov	_PAGE_VALID,d2		# force address error handler to be
					# invoked
	bra	dtlb_miss_set
	.size	dtlb_miss, . - dtlb_miss

###############################################################################
#
# Instruction TLB Address Error handler entry point
#
###############################################################################
	.type	itlb_aerror,@function
ENTRY(itlb_aerror)
	and	~EPSW_NMID,epsw
	add	-4,sp
	SAVE_ALL
	add	-4,sp				# need to pass three params

	# calculate the fault code
	movhu	(MMUFCR_IFC),d1
	or	0x00010000,d1			# it's an instruction fetch

	# determine the page address
	mov	(IPTEU),a2
	mov	a2,d0
	and	PAGE_MASK,d0
	mov	d0,(12,sp)

	clr	d0
	mov	d0,(IPTEL)

	and	~EPSW_NMID,epsw
	or	EPSW_IE,epsw
	mov	fp,d0
	call	do_page_fault[],0		# do_page_fault(regs,code,addr

	jmp	ret_from_exception
	.size	itlb_aerror, . - itlb_aerror

###############################################################################
#
# Data TLB Address Error handler entry point
#
###############################################################################
	.type	dtlb_aerror,@function
ENTRY(dtlb_aerror)
	and	~EPSW_NMID,epsw
	add	-4,sp
	SAVE_ALL
	add	-4,sp				# need to pass three params

	# calculate the fault code
	movhu	(MMUFCR_DFC),d1

	# determine the page address
	mov	(DPTEU),a2
	mov	a2,d0
	and	PAGE_MASK,d0
	mov	d0,(12,sp)

	clr	d0
	mov	d0,(DPTEL)

	and	~EPSW_NMID,epsw
	or	EPSW_IE,epsw
	mov	fp,d0
	call	do_page_fault[],0		# do_page_fault(regs,code,addr

	jmp	ret_from_exception
	.size	dtlb_aerror, . - dtlb_aerror
