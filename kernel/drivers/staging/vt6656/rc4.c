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
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
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
 *
 * File: rc4.c
 *
 * Purpose:
 *
 * Functions:
 *
 * Revision History:
 *
 * Author: Kyle Hsu
 *
 * Date: Sep 4, 2002
 *
 */

#include "rc4.h"

void rc4_init(PRC4Ext pRC4, PBYTE pbyKey, unsigned int cbKey_len)
{
	unsigned int  ust1, ust2;
	unsigned int  keyindex;
	unsigned int  stateindex;
	PBYTE pbyst;
	unsigned int  idx;

	pbyst = pRC4->abystate;
	pRC4->ux = 0;
	pRC4->uy = 0;
	for (idx = 0; idx < 256; idx++)
		pbyst[idx] = (BYTE)idx;
	keyindex = 0;
	stateindex = 0;
	for (idx = 0; idx < 256; idx++) {
		ust1 = pbyst[idx];
		stateindex = (stateindex + pbyKey[keyindex] + ust1) & 0xff;
		ust2 = pbyst[stateindex];
		pbyst[stateindex] = (BYTE)ust1;
		pbyst[idx] = (BYTE)ust2;
		if (++keyindex >= cbKey_len)
			keyindex = 0;
	}
}

unsigned int rc4_byte(PRC4Ext pRC4)
{
	unsigned int ux;
	unsigned int uy;
	unsigned int ustx, usty;
	PBYTE pbyst;

	pbyst = pRC4->abystate;
	ux = (pRC4->ux + 1) & 0xff;
	ustx = pbyst[ux];
	uy = (ustx + pRC4->uy) & 0xff;
	usty = pbyst[uy];
	pRC4->ux = ux;
	pRC4->uy = uy;
	pbyst[uy] = (BYTE)ustx;
	pbyst[ux] = (BYTE)usty;

	return pbyst[(ustx + usty) & 0xff];
}

void rc4_encrypt(PRC4Ext pRC4, PBYTE pbyDest,
			PBYTE pbySrc, unsigned int cbData_len)
{
	unsigned int ii;
	for (ii = 0; ii < cbData_len; ii++)
		pbyDest[ii] = (BYTE)(pbySrc[ii] ^ rc4_byte(pRC4));
}
