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
 * Copyright 2005-2010 Analog Devices Inc.
 *
 * Licensed under the ADI BSD license or the GPL-2 (or later)
 */

#include <linux/linkage.h>
#include <asm/context.S>

/* void *strncpy(char *dest, const char *src, size_t n);
 * R0 = address (dest)
 * R1 = address (src)
 * R2 = size
 * Returns a pointer (R0) to the destination string dest
 *  we do this by not changing R0
 */

#ifdef CONFIG_STRNCPY_L1
.section .l1.text
#else
.text
#endif

.align 2

ENTRY(_strncpy)
	CC = R2 == 0;
	if CC JUMP 4f;

	P2 = R2 ;       /* size */
	P0 = R0 ;       /* dst*/
	P1 = R1 ;       /* src*/

	LSETUP (1f, 2f) LC0 = P2;
1:
	R1 = B [P1++] (Z);
	B [P0++] = R1;
	CC = R1 == 0;
2:
	if CC jump 3f;

	RTS;

	/* if src is shorter than n, we need to null pad bytes in dest
	 * but, we can get here when the last byte is zero, and we don't
	 * want to copy an extra byte at the end, so we need to check
	 */
3:
	R2 = LC0;
	CC = R2
	if ! CC jump 6f;

	/* if the required null padded portion is small, do it here, rather than
	 * handling the overhead of memset (which is OK when things are big).
	 */
	R3 = 0x20;
	CC = R2 < R3;
	IF CC jump 4f;

	R2 += -1;

	/* Set things up for memset
	 * R0 = address
	 * R1 = filler byte (this case it's zero, set above)
	 * R2 = count (set above)
	 */

	I1 = R0;
	R0 = RETS;
	I0 = R0;
	R0 = P0;
	pseudo_long_call _memset, p0;
	R0 = I0;
	RETS = R0;
	R0 = I1;
	RTS;

4:
	LSETUP(5f, 5f) LC0;
5:
	B [P0++] = R1;
6:
	RTS;

ENDPROC(_strncpy)
