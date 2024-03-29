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
 * Copyright SUSE Linux Products GmbH 2010
 *
 * Authors: Alexander Graf <agraf@suse.de>
 */

/* Real mode helpers */

#if defined(CONFIG_PPC_BOOK3S_64)

#define GET_SHADOW_VCPU(reg)    \
	addi    reg, r13, PACA_KVM_SVCPU

#elif defined(CONFIG_PPC_BOOK3S_32)

#define GET_SHADOW_VCPU(reg)    			\
	tophys(reg, r2);       			\
	lwz     reg, (THREAD + THREAD_KVM_SVCPU)(reg);	\
	tophys(reg, reg)

#endif

/* Disable for nested KVM */
#define USE_QUICK_LAST_INST


/* Get helper functions for subarch specific functionality */

#if defined(CONFIG_PPC_BOOK3S_64)
#include "book3s_64_slb.S"
#elif defined(CONFIG_PPC_BOOK3S_32)
#include "book3s_32_sr.S"
#endif

/******************************************************************************
 *                                                                            *
 *                               Entry code                                   *
 *                                                                            *
 *****************************************************************************/

.global kvmppc_handler_trampoline_enter
kvmppc_handler_trampoline_enter:

	/* Required state:
	 *
	 * MSR = ~IR|DR
	 * R13 = PACA
	 * R1 = host R1
	 * R2 = host R2
	 * R10 = guest MSR
	 * all other volatile GPRS = free
	 * SVCPU[CR] = guest CR
	 * SVCPU[XER] = guest XER
	 * SVCPU[CTR] = guest CTR
	 * SVCPU[LR] = guest LR
	 */

	/* r3 = shadow vcpu */
	GET_SHADOW_VCPU(r3)

	/* Move SRR0 and SRR1 into the respective regs */
	PPC_LL  r9, SVCPU_PC(r3)
	mtsrr0	r9
	mtsrr1	r10

	/* Activate guest mode, so faults get handled by KVM */
	li	r11, KVM_GUEST_MODE_GUEST
	stb	r11, SVCPU_IN_GUEST(r3)

	/* Switch to guest segment. This is subarch specific. */
	LOAD_GUEST_SEGMENTS

	/* Enter guest */

	PPC_LL	r4, (SVCPU_CTR)(r3)
	PPC_LL	r5, (SVCPU_LR)(r3)
	lwz	r6, (SVCPU_CR)(r3)
	lwz	r7, (SVCPU_XER)(r3)

	mtctr	r4
	mtlr	r5
	mtcr	r6
	mtxer	r7

	PPC_LL	r0, (SVCPU_R0)(r3)
	PPC_LL	r1, (SVCPU_R1)(r3)
	PPC_LL	r2, (SVCPU_R2)(r3)
	PPC_LL	r4, (SVCPU_R4)(r3)
	PPC_LL	r5, (SVCPU_R5)(r3)
	PPC_LL	r6, (SVCPU_R6)(r3)
	PPC_LL	r7, (SVCPU_R7)(r3)
	PPC_LL	r8, (SVCPU_R8)(r3)
	PPC_LL	r9, (SVCPU_R9)(r3)
	PPC_LL	r10, (SVCPU_R10)(r3)
	PPC_LL	r11, (SVCPU_R11)(r3)
	PPC_LL	r12, (SVCPU_R12)(r3)
	PPC_LL	r13, (SVCPU_R13)(r3)

	PPC_LL	r3, (SVCPU_R3)(r3)

	RFI
kvmppc_handler_trampoline_enter_end:



/******************************************************************************
 *                                                                            *
 *                               Exit code                                    *
 *                                                                            *
 *****************************************************************************/

.global kvmppc_handler_trampoline_exit
kvmppc_handler_trampoline_exit:

	/* Register usage at this point:
	 *
	 * SPRG_SCRATCH0  = guest R13
	 * R12            = exit handler id
	 * R13            = shadow vcpu - SHADOW_VCPU_OFF [=PACA on PPC64]
	 * SVCPU.SCRATCH0 = guest R12
	 * SVCPU.SCRATCH1 = guest CR
	 *
	 */

	/* Save registers */

	PPC_STL	r0, (SHADOW_VCPU_OFF + SVCPU_R0)(r13)
	PPC_STL	r1, (SHADOW_VCPU_OFF + SVCPU_R1)(r13)
	PPC_STL	r2, (SHADOW_VCPU_OFF + SVCPU_R2)(r13)
	PPC_STL	r3, (SHADOW_VCPU_OFF + SVCPU_R3)(r13)
	PPC_STL	r4, (SHADOW_VCPU_OFF + SVCPU_R4)(r13)
	PPC_STL	r5, (SHADOW_VCPU_OFF + SVCPU_R5)(r13)
	PPC_STL	r6, (SHADOW_VCPU_OFF + SVCPU_R6)(r13)
	PPC_STL	r7, (SHADOW_VCPU_OFF + SVCPU_R7)(r13)
	PPC_STL	r8, (SHADOW_VCPU_OFF + SVCPU_R8)(r13)
	PPC_STL	r9, (SHADOW_VCPU_OFF + SVCPU_R9)(r13)
	PPC_STL	r10, (SHADOW_VCPU_OFF + SVCPU_R10)(r13)
	PPC_STL	r11, (SHADOW_VCPU_OFF + SVCPU_R11)(r13)

	/* Restore R1/R2 so we can handle faults */
	PPC_LL	r1, (SHADOW_VCPU_OFF + SVCPU_HOST_R1)(r13)
	PPC_LL	r2, (SHADOW_VCPU_OFF + SVCPU_HOST_R2)(r13)

	/* Save guest PC and MSR */
	mfsrr0	r3
	mfsrr1	r4

	PPC_STL	r3, (SHADOW_VCPU_OFF + SVCPU_PC)(r13)
	PPC_STL	r4, (SHADOW_VCPU_OFF + SVCPU_SHADOW_SRR1)(r13)

	/* Get scratch'ed off registers */
	mfspr	r9, SPRN_SPRG_SCRATCH0
	PPC_LL	r8, (SHADOW_VCPU_OFF + SVCPU_SCRATCH0)(r13)
	lwz	r7, (SHADOW_VCPU_OFF + SVCPU_SCRATCH1)(r13)

	PPC_STL	r9, (SHADOW_VCPU_OFF + SVCPU_R13)(r13)
	PPC_STL	r8, (SHADOW_VCPU_OFF + SVCPU_R12)(r13)
	stw	r7, (SHADOW_VCPU_OFF + SVCPU_CR)(r13)

	/* Save more register state  */

	mfxer	r5
	mfdar	r6
	mfdsisr	r7
	mfctr	r8
	mflr	r9

	stw	r5, (SHADOW_VCPU_OFF + SVCPU_XER)(r13)
	PPC_STL	r6, (SHADOW_VCPU_OFF + SVCPU_FAULT_DAR)(r13)
	stw	r7, (SHADOW_VCPU_OFF + SVCPU_FAULT_DSISR)(r13)
	PPC_STL	r8, (SHADOW_VCPU_OFF + SVCPU_CTR)(r13)
	PPC_STL	r9, (SHADOW_VCPU_OFF + SVCPU_LR)(r13)

	/*
	 * In order for us to easily get the last instruction,
	 * we got the #vmexit at, we exploit the fact that the
	 * virtual layout is still the same here, so we can just
	 * ld from the guest's PC address
	 */

	/* We only load the last instruction when it's safe */
	cmpwi	r12, BOOK3S_INTERRUPT_DATA_STORAGE
	beq	ld_last_inst
	cmpwi	r12, BOOK3S_INTERRUPT_PROGRAM
	beq	ld_last_inst
	cmpwi	r12, BOOK3S_INTERRUPT_ALIGNMENT
	beq-	ld_last_inst

	b	no_ld_last_inst

ld_last_inst:
	/* Save off the guest instruction we're at */

	/* In case lwz faults */
	li	r0, KVM_INST_FETCH_FAILED

#ifdef USE_QUICK_LAST_INST

	/* Set guest mode to 'jump over instruction' so if lwz faults
	 * we'll just continue at the next IP. */
	li	r9, KVM_GUEST_MODE_SKIP
	stb	r9, (SHADOW_VCPU_OFF + SVCPU_IN_GUEST)(r13)

	/*    1) enable paging for data */
	mfmsr	r9
	ori	r11, r9, MSR_DR			/* Enable paging for data */
	mtmsr	r11
	sync
	/*    2) fetch the instruction */
	lwz	r0, 0(r3)
	/*    3) disable paging again */
	mtmsr	r9
	sync

#endif
	stw	r0, (SHADOW_VCPU_OFF + SVCPU_LAST_INST)(r13)

no_ld_last_inst:

	/* Unset guest mode */
	li	r9, KVM_GUEST_MODE_NONE
	stb	r9, (SHADOW_VCPU_OFF + SVCPU_IN_GUEST)(r13)

	/* Switch back to host MMU */
	LOAD_HOST_SEGMENTS

	/* Register usage at this point:
	 *
	 * R1       = host R1
	 * R2       = host R2
	 * R12      = exit handler id
	 * R13      = shadow vcpu - SHADOW_VCPU_OFF [=PACA on PPC64]
	 * SVCPU.*  = guest *
	 *
	 */

	/* RFI into the highmem handler */
	mfmsr	r7
	ori	r7, r7, MSR_IR|MSR_DR|MSR_RI|MSR_ME	/* Enable paging */
	mtsrr1	r7
	/* Load highmem handler address */
	PPC_LL	r8, (SHADOW_VCPU_OFF + SVCPU_VMHANDLER)(r13)
	mtsrr0	r8

	RFI
kvmppc_handler_trampoline_exit_end:
