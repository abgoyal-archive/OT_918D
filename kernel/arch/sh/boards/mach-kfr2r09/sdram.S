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
 * KFR2R09 sdram self/auto-refresh setup code
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
ENTRY(kfr2r09_sdram_enter_start)

	/* DBSC: put memory in self-refresh mode */

	ED 0xFD000010, 0x00000000 /* DBEN */
	ED 0xFD000040, 0x00000000 /* DBRFPDN0 */
	ED 0xFD000014, 0x00000002 /* DBCMDCNT (PALL) */
	ED 0xFD000014, 0x00000004 /* DBCMDCNT (REF) */
	ED 0xFD000040, 0x00000001 /* DBRFPDN0 */

	rts
	 nop

ENTRY(kfr2r09_sdram_enter_end)

	.balign 4
ENTRY(kfr2r09_sdram_leave_start)

	/* DBSC: put memory in auto-refresh mode */

	mov.l	@(SH_SLEEP_MODE, r5), r0
	tst	#SUSP_SH_RSTANDBY, r0
	bf	resume_rstandby

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

	ED 0xFD000108, 0x40000301 /* DBPDCNT0 */
	ED 0xFD000020, 0x011B0002 /* DBCONF */
	ED 0xFD000030, 0x03060E02 /* DBTR0 */
	ED 0xFD000034, 0x01020102 /* DBTR1 */
	ED 0xFD000038, 0x01090406 /* DBTR2 */
	ED 0xFD000008, 0x00000004 /* DBKIND */
	ED 0xFD000040, 0x00000001 /* DBRFPDN0 */
	ED 0xFD000040, 0x00000000 /* DBRFPDN0 */
	ED 0xFD000018, 0x00000001 /* DBCKECNT */
	WAIT 1
	ED 0xFD000010, 0x00000001 /* DBEN */
	ED 0xFD000044, 0x000004AF /* DBRFPDN1 */
	ED 0xFD000048, 0x20CF0037 /* DBRFPDN2 */
	ED 0xFD000014, 0x00000004 /* DBCMDCNT (REF) */
	ED 0xFD000108, 0x40000300 /* DBPDCNT0 */
	ED 0xFD000040, 0x00010000 /* DBRFPDN0 */

	rts
	 nop

ENTRY(kfr2r09_sdram_leave_end)
