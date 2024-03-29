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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Copyright SUSE Linux Products GmbH 2009
 *
 * Authors: Alexander Graf <agraf@suse.de>
 */

#define SHADOW_SLB_ESID(num)	(SLBSHADOW_SAVEAREA + (num * 0x10))
#define SHADOW_SLB_VSID(num)	(SLBSHADOW_SAVEAREA + (num * 0x10) + 0x8)
#define UNBOLT_SLB_ENTRY(num) \
	ld	r9, SHADOW_SLB_ESID(num)(r12); \
	/* Invalid? Skip. */; \
	rldicl. r0, r9, 37, 63; \
	beq	slb_entry_skip_ ## num; \
	xoris	r9, r9, SLB_ESID_V@h; \
	std	r9, SHADOW_SLB_ESID(num)(r12); \
  slb_entry_skip_ ## num:

#define REBOLT_SLB_ENTRY(num) \
	ld	r10, SHADOW_SLB_ESID(num)(r11); \
	cmpdi	r10, 0; \
	beq	slb_exit_skip_ ## num; \
	oris	r10, r10, SLB_ESID_V@h; \
	ld	r9, SHADOW_SLB_VSID(num)(r11); \
	slbmte	r9, r10; \
	std	r10, SHADOW_SLB_ESID(num)(r11); \
slb_exit_skip_ ## num:

/******************************************************************************
 *                                                                            *
 *                               Entry code                                   *
 *                                                                            *
 *****************************************************************************/

.macro LOAD_GUEST_SEGMENTS

	/* Required state:
	 *
	 * MSR = ~IR|DR
	 * R13 = PACA
	 * R1 = host R1
	 * R2 = host R2
	 * R3 = shadow vcpu
	 * all other volatile GPRS = free
	 * SVCPU[CR]  = guest CR
	 * SVCPU[XER] = guest XER
	 * SVCPU[CTR] = guest CTR
	 * SVCPU[LR]  = guest LR
	 */

	/* Remove LPAR shadow entries */

#if SLB_NUM_BOLTED == 3

	ld	r12, PACA_SLBSHADOWPTR(r13)

	/* Save off the first entry so we can slbie it later */
	ld	r10, SHADOW_SLB_ESID(0)(r12)
	ld	r11, SHADOW_SLB_VSID(0)(r12)

	/* Remove bolted entries */
	UNBOLT_SLB_ENTRY(0)
	UNBOLT_SLB_ENTRY(1)
	UNBOLT_SLB_ENTRY(2)
	
#else
#error unknown number of bolted entries
#endif

	/* Flush SLB */

	slbia

	/* r0 = esid & ESID_MASK */
	rldicr  r10, r10, 0, 35
	/* r0 |= CLASS_BIT(VSID) */
	rldic   r12, r11, 56 - 36, 36
	or      r10, r10, r12
	slbie	r10

	isync

	/* Fill SLB with our shadow */

	lbz	r12, SVCPU_SLB_MAX(r3)
	mulli	r12, r12, 16
	addi	r12, r12, SVCPU_SLB
	add	r12, r12, r3

	/* for (r11 = kvm_slb; r11 < kvm_slb + kvm_slb_size; r11+=slb_entry) */
	li	r11, SVCPU_SLB
	add	r11, r11, r3

slb_loop_enter:

	ld	r10, 0(r11)

	rldicl. r0, r10, 37, 63
	beq	slb_loop_enter_skip

	ld	r9, 8(r11)
	slbmte	r9, r10

slb_loop_enter_skip:
	addi	r11, r11, 16
	cmpd	cr0, r11, r12
	blt	slb_loop_enter

slb_do_enter:

.endm

/******************************************************************************
 *                                                                            *
 *                               Exit code                                    *
 *                                                                            *
 *****************************************************************************/

.macro LOAD_HOST_SEGMENTS

	/* Register usage at this point:
	 *
	 * R1         = host R1
	 * R2         = host R2
	 * R12        = exit handler id
	 * R13        = shadow vcpu - SHADOW_VCPU_OFF [=PACA on PPC64]
	 * SVCPU.*    = guest *
	 * SVCPU[CR]  = guest CR
	 * SVCPU[XER] = guest XER
	 * SVCPU[CTR] = guest CTR
	 * SVCPU[LR]  = guest LR
	 *
	 */

	/* Restore bolted entries from the shadow and fix it along the way */

	/* We don't store anything in entry 0, so we don't need to take care of it */
	slbia
	isync

#if SLB_NUM_BOLTED == 3

	ld	r11, PACA_SLBSHADOWPTR(r13)

	REBOLT_SLB_ENTRY(0)
	REBOLT_SLB_ENTRY(1)
	REBOLT_SLB_ENTRY(2)
	
#else
#error unknown number of bolted entries
#endif

slb_do_exit:

.endm
