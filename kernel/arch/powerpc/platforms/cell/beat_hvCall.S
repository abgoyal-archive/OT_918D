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
 * Beat hypervisor call I/F
 *
 * (C) Copyright 2007 TOSHIBA CORPORATION
 *
 * This code is based on arch/powerpc/platforms/pseries/hvCall.S.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <asm/ppc_asm.h>

#define	STK_PARM(i)	(48 + ((i)-3)*8)

/* Not implemented on Beat, now */
#define	HCALL_INST_PRECALL
#define	HCALL_INST_POSTCALL

	.text

#define	HVSC	.long	0x44000022

/* Note: takes only 7 input parameters at maximum */
_GLOBAL(beat_hcall_norets)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	mr	r11,r3
	mr	r3,r4
	mr	r4,r5
	mr	r5,r6
	mr	r6,r7
	mr	r7,r8
	mr	r8,r9

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */

/* Note: takes 8 input parameters at maximum */
_GLOBAL(beat_hcall_norets8)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	mr	r11,r3
	mr	r3,r4
	mr	r4,r5
	mr	r5,r6
	mr	r6,r7
	mr	r7,r8
	mr	r8,r9
	ld	r10,STK_PARM(r10)(r1)

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */

/* Note: takes only 6 input parameters, 1 output parameters at maximum */
_GLOBAL(beat_hcall1)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	std	r4,STK_PARM(r4)(r1)	/* save ret buffer */

	mr	r11,r3
	mr	r3,r5
	mr	r4,r6
	mr	r5,r7
	mr	r6,r8
	mr	r7,r9
	mr	r8,r10

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	ld	r12,STK_PARM(r4)(r1)
	std	r4,  0(r12)

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */

/* Note: takes only 6 input parameters, 2 output parameters at maximum */
_GLOBAL(beat_hcall2)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	std	r4,STK_PARM(r4)(r1)	/* save ret buffer */

	mr	r11,r3
	mr	r3,r5
	mr	r4,r6
	mr	r5,r7
	mr	r6,r8
	mr	r7,r9
	mr	r8,r10

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	ld	r12,STK_PARM(r4)(r1)
	std	r4,  0(r12)
	std	r5,  8(r12)

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */

/* Note: takes only 6 input parameters, 3 output parameters at maximum */
_GLOBAL(beat_hcall3)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	std	r4,STK_PARM(r4)(r1)	/* save ret buffer */

	mr	r11,r3
	mr	r3,r5
	mr	r4,r6
	mr	r5,r7
	mr	r6,r8
	mr	r7,r9
	mr	r8,r10

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	ld	r12,STK_PARM(r4)(r1)
	std	r4,  0(r12)
	std	r5,  8(r12)
	std	r6, 16(r12)

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */

/* Note: takes only 6 input parameters, 4 output parameters at maximum */
_GLOBAL(beat_hcall4)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	std	r4,STK_PARM(r4)(r1)	/* save ret buffer */

	mr	r11,r3
	mr	r3,r5
	mr	r4,r6
	mr	r5,r7
	mr	r6,r8
	mr	r7,r9
	mr	r8,r10

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	ld	r12,STK_PARM(r4)(r1)
	std	r4,  0(r12)
	std	r5,  8(r12)
	std	r6, 16(r12)
	std	r7, 24(r12)

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */

/* Note: takes only 6 input parameters, 5 output parameters at maximum */
_GLOBAL(beat_hcall5)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	std	r4,STK_PARM(r4)(r1)	/* save ret buffer */

	mr	r11,r3
	mr	r3,r5
	mr	r4,r6
	mr	r5,r7
	mr	r6,r8
	mr	r7,r9
	mr	r8,r10

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	ld	r12,STK_PARM(r4)(r1)
	std	r4,  0(r12)
	std	r5,  8(r12)
	std	r6, 16(r12)
	std	r7, 24(r12)
	std	r8, 32(r12)

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */

/* Note: takes only 6 input parameters, 6 output parameters at maximum */
_GLOBAL(beat_hcall6)
	HMT_MEDIUM

	mfcr	r0
	stw	r0,8(r1)

	HCALL_INST_PRECALL

	std	r4,STK_PARM(r4)(r1)	/* save ret buffer */

	mr	r11,r3
	mr	r3,r5
	mr	r4,r6
	mr	r5,r7
	mr	r6,r8
	mr	r7,r9
	mr	r8,r10

	HVSC				/* invoke the hypervisor */

	HCALL_INST_POSTCALL

	ld	r12,STK_PARM(r4)(r1)
	std	r4,  0(r12)
	std	r5,  8(r12)
	std	r6, 16(r12)
	std	r7, 24(r12)
	std	r8, 32(r12)
	std	r9, 40(r12)

	lwz	r0,8(r1)
	mtcrf	0xff,r0

	blr				/* return r3 = status */
