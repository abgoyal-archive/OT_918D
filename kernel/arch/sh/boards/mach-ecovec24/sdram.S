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
 * Ecovec24 sdram self/auto-refresh setup code
 *
 *  Copyright (C) 2009 Magnus Damm
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/sys.h>
#include <linux/errno.h>
#include <linux/linkage.h>
#include <asm/asm-offsets.h>
#include <asm/suspend.h>
#include <asm/romimage-macros.h>

/* code to enter and leave self-refresh. must be self-contained.
 * this code will be copied to on-chip memory and executed from there.
 */
	.balign 4
ENTRY(ecovec24_sdram_enter_start)

	/* DBSC: put memory in self-refresh mode */

	ED 0xFD000010, 0x00000000 /* DBEN */
	ED 0xFD000040, 0x00000000 /* DBRFPDN0 */
	ED 0xFD000014, 0x00000002 /* DBCMDCNT (PALL) */
	ED 0xFD000014, 0x00000004 /* DBCMDCNT (REF) */
	ED 0xFD000040, 0x00000001 /* DBRFPDN0 */

	rts
	 nop

ENTRY(ecovec24_sdram_enter_end)

	.balign 4
ENTRY(ecovec24_sdram_leave_start)

	mov.l	@(SH_SLEEP_MODE, r5), r0
	tst	#SUSP_SH_RSTANDBY, r0
	bf	resume_rstandby

	/* DBSC: put memory in auto-refresh mode */

	ED 0xFD000040, 0x00000000 /* DBRFPDN0 */
	WAIT 1
	ED 0xFD000014, 0x00000002 /* DBCMDCNT (PALL) */
	ED 0xFD000014, 0x00000004 /* DBCMDCNT (REF) */
	ED 0xFD000010, 0x00000001 /* DBEN */
	ED 0xFD000040, 0x00010000 /* DBRFPDN0 */

	rts
	 nop

resume_rstandby:

	/* DBSC: re-initialize and put in auto-refresh */

	ED 0xFD000108, 0x00000181 /* DBPDCNT0 */
	ED 0xFD000020, 0x015B0002 /* DBCONF */
	ED 0xFD000030, 0x03071502 /* DBTR0 */
	ED 0xFD000034, 0x02020102 /* DBTR1 */
	ED 0xFD000038, 0x01090405 /* DBTR2 */
	ED 0xFD00003C, 0x00000002 /* DBTR3 */
	ED 0xFD000008, 0x00000005 /* DBKIND */
	ED 0xFD000040, 0x00000001 /* DBRFPDN0 */
	ED 0xFD000040, 0x00000000 /* DBRFPDN0 */
	ED 0xFD000018, 0x00000001 /* DBCKECNT */

	mov	#100,r0
WAIT_400NS:
	dt	r0
	bf	WAIT_400NS

	ED 0xFD000014, 0x00000002 /* DBCMDCNT (PALL) */
	ED 0xFD000060, 0x00020000 /* DBMRCNT (EMR2) */
	ED 0xFD000060, 0x00030000 /* DBMRCNT (EMR3) */
	ED 0xFD000060, 0x00010004 /* DBMRCNT (EMR) */
	ED 0xFD000060, 0x00000532 /* DBMRCNT (MRS) */
	ED 0xFD000014, 0x00000002 /* DBCMDCNT (PALL) */
	ED 0xFD000014, 0x00000004 /* DBCMDCNT (REF) */
	ED 0xFD000014, 0x00000004 /* DBCMDCNT (REF) */
	ED 0xFD000060, 0x00000432 /* DBMRCNT (MRS) */
	ED 0xFD000060, 0x000103c0 /* DBMRCNT (EMR) */
	ED 0xFD000060, 0x00010040 /* DBMRCNT (EMR) */

	mov	#100,r0
WAIT_400NS_2:
	dt	r0
	bf	WAIT_400NS_2

	ED 0xFD000010, 0x00000001 /* DBEN */
	ED 0xFD000044, 0x0000050f /* DBRFPDN1 */
	ED 0xFD000048, 0x236800e6 /* DBRFPDN2 */

	mov.l	DUMMY,r0
	mov.l	@r0, r1 /* force single dummy read */

	ED 0xFD000014, 0x00000002 /* DBCMDCNT (PALL) */
	ED 0xFD000014, 0x00000004 /* DBCMDCNT (REF) */
	ED 0xFD000108, 0x00000080 /* DBPDCNT0 */
	ED 0xFD000040, 0x00010000 /* DBRFPDN0 */

	rts
	 nop

	.balign 4
DUMMY:	.long	0xac400000

ENTRY(ecovec24_sdram_leave_end)
