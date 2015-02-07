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

/*****************************************************************************
* Copyright 2006 - 2008 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

/*
 *
 * Low-level IRQ helper macros for BCMRing-based platforms
 *
 */
#include <mach/irqs.h>
#include <mach/hardware.h>
#include <mach/csp/mm_io.h>

		.macro	disable_fiq
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =(MM_IO_BASE_INTC0)
		ldr	\irqstat, [\base, #0]		@ get status
                ldr     \irqnr, [\base, #0x10]          @ mask with enable register
                ands    \irqstat, \irqstat, \irqnr
		mov	\irqnr, #IRQ_INTC0_START
		cmp	\irqstat, #0
		bne	1001f

		ldr	\base, =(MM_IO_BASE_INTC1)
		ldr	\irqstat, [\base, #0]		@ get status
                ldr     \irqnr, [\base, #0x10]          @ mask with enable register
                ands    \irqstat, \irqstat, \irqnr
		mov	\irqnr, #IRQ_INTC1_START
		cmp	\irqstat, #0
		bne	1001f

		ldr	\base, =(MM_IO_BASE_SINTC)
		ldr	\irqstat, [\base, #0]		@ get status
                ldr     \irqnr, [\base, #0x10]          @ mask with enable register
                ands    \irqstat, \irqstat, \irqnr
		mov	\irqnr, #0xffffffff             @ code meaning no interrupt bits set
		cmp	\irqstat, #0
		beq	1002f

		mov	\irqnr, #IRQ_SINTC_START        @ something is set, so fixup return value

1001:
		movs	\tmp, \irqstat, lsl #16
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #16

		movs	\tmp, \irqstat, lsl #8
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #8

		movs	\tmp, \irqstat, lsl #4
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #4

		movs	\tmp, \irqstat, lsl #2
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #2

		movs	\tmp, \irqstat, lsl #1
		addeq	\irqnr, \irqnr, #1
		orrs	\base, \base, #1

1002:           @ irqnr will be set to 0xffffffff if no irq bits are set
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	irq_prio_table
		.endm

