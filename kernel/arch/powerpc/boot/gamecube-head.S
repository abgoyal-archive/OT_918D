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
 * arch/powerpc/boot/gamecube-head.S
 *
 * Nintendo GameCube bootwrapper entry.
 * Copyright (C) 2004-2009 The GameCube Linux Team
 * Copyright (C) 2008,2009 Albert Herranz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 */

#include "ppc_asm.h"

/*
 * The entry code does no assumptions regarding:
 * - if the data and instruction caches are enabled or not
 * - if the MMU is enabled or not
 *
 * We enable the caches if not already enabled, enable the MMU with an
 * identity mapping scheme and jump to the start code.
 */

	.text

	.globl _zimage_start
_zimage_start:

	/* turn the MMU off */
	mfmsr	9
	rlwinm	9, 9, 0, ~((1<<4)|(1<<5)) /* MSR_DR|MSR_IR */
	bcl	20, 31, 1f
1:
	mflr	8
	clrlwi	8, 8, 3		/* convert to a real address */
	addi	8, 8, _mmu_off - 1b
	mtsrr0	8
	mtsrr1	9
	rfi
_mmu_off:
	/* MMU disabled */

	/* setup BATs */
	isync
	li      8, 0
	mtspr	0x210, 8	/* IBAT0U */
	mtspr	0x212, 8	/* IBAT1U */
	mtspr	0x214, 8	/* IBAT2U */
	mtspr	0x216, 8	/* IBAT3U */
	mtspr	0x218, 8	/* DBAT0U */
	mtspr	0x21a, 8	/* DBAT1U */
	mtspr	0x21c, 8	/* DBAT2U */
	mtspr	0x21e, 8	/* DBAT3U */

	li	8, 0x01ff	/* first 16MiB */
	li	9, 0x0002	/* rw */
	mtspr	0x211, 9	/* IBAT0L */
	mtspr	0x210, 8	/* IBAT0U */
	mtspr	0x219, 9	/* DBAT0L */
	mtspr	0x218, 8	/* DBAT0U */

	lis	8, 0x0c00	/* I/O mem */
	ori	8, 8, 0x3ff	/* 32MiB */
	lis	9, 0x0c00
	ori	9, 9, 0x002a	/* uncached, guarded, rw */
	mtspr	0x21b, 9	/* DBAT1L */
	mtspr	0x21a, 8	/* DBAT1U */

	lis	8, 0x0100	/* next 8MiB */
	ori	8, 8, 0x00ff	/* 8MiB */
	lis	9, 0x0100
	ori	9, 9, 0x0002	/* rw */
	mtspr	0x215, 9	/* IBAT2L */
	mtspr	0x214, 8	/* IBAT2U */
	mtspr	0x21d, 9	/* DBAT2L */
	mtspr	0x21c, 8	/* DBAT2U */

	/* enable and invalidate the caches if not already enabled */
	mfspr	8, 0x3f0	/* HID0 */
	andi.	0, 8, (1<<15)		/* HID0_ICE */
	bne	1f
	ori	8, 8, (1<<15)|(1<<11)	/* HID0_ICE|HID0_ICFI*/
1:
	andi.	0, 8, (1<<14)		/* HID0_DCE */
	bne	1f
	ori	8, 8, (1<<14)|(1<<10)	/* HID0_DCE|HID0_DCFI*/
1:
	mtspr	0x3f0, 8	/* HID0 */
	isync

	/* initialize arguments */
	li	3, 0
	li	4, 0
	li	5, 0

	/* turn the MMU on */
	bcl	20, 31, 1f
1:
	mflr	8
	addi	8, 8, _mmu_on - 1b
	mfmsr	9
	ori	9, 9, (1<<4)|(1<<5) /* MSR_DR|MSR_IR */
	mtsrr0	8
	mtsrr1	9
	sync
	rfi
_mmu_on:
	b _zimage_start_lib

