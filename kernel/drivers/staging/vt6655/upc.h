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
 * File: upc.h
 *
 * Purpose: Macros to access device
 *
 * Author: Tevin Chen
 *
 * Date: Mar 17, 1997
 *
 */

#ifndef __UPC_H__
#define __UPC_H__

#include "device.h"
#include "ttype.h"

/*---------------------  Export Definitions -------------------------*/


//
//  For IO mapped
//

#ifdef IO_MAP

#define VNSvInPortB(dwIOAddress, pbyData) {                     \
	*(pbyData) = inb(dwIOAddress);                              \
}


#define VNSvInPortW(dwIOAddress, pwData) {                      \
	    *(pwData) = inw(dwIOAddress);                           \
}

#define VNSvInPortD(dwIOAddress, pdwData) {                     \
	    *(pdwData) = inl(dwIOAddress);                          \
}


#define VNSvOutPortB(dwIOAddress, byData) {                     \
        outb(byData, dwIOAddress);                              \
}


#define VNSvOutPortW(dwIOAddress, wData) {                      \
        outw(wData, dwIOAddress);                               \
}

#define VNSvOutPortD(dwIOAddress, dwData) {                     \
        outl(dwData, dwIOAddress);                              \
}

#else

//
//  For memory mapped IO
//


#define VNSvInPortB(dwIOAddress, pbyData) {                     \
	volatile BYTE* pbyAddr = ((PBYTE)(dwIOAddress));            \
	*(pbyData) = readb(pbyAddr);                           \
}


#define VNSvInPortW(dwIOAddress, pwData) {                      \
	volatile WORD* pwAddr = ((PWORD)(dwIOAddress));             \
	*(pwData) = readw(pwAddr);                             \
}

#define VNSvInPortD(dwIOAddress, pdwData) {                     \
	volatile DWORD* pdwAddr = ((PDWORD)(dwIOAddress));          \
	*(pdwData) = readl(pdwAddr);                           \
}


#define VNSvOutPortB(dwIOAddress, byData) {                     \
    volatile BYTE* pbyAddr = ((PBYTE)(dwIOAddress));            \
    writeb((BYTE)byData, pbyAddr);							\
}


#define VNSvOutPortW(dwIOAddress, wData) {                      \
    volatile WORD* pwAddr = ((PWORD)(dwIOAddress));             \
    writew((WORD)wData, pwAddr);							\
}

#define VNSvOutPortD(dwIOAddress, dwData) {                     \
    volatile DWORD* pdwAddr = ((PDWORD)(dwIOAddress));          \
    writel((DWORD)dwData, pdwAddr);					    \
}

#endif


//
// ALWAYS IO-Mapped IO when in 16-bit/32-bit environment
//
#define PCBvInPortB(dwIOAddress, pbyData) {     \
	    *(pbyData) = inb(dwIOAddress);          \
}

#define PCBvInPortW(dwIOAddress, pwData) {      \
	    *(pwData) = inw(dwIOAddress);           \
}

#define PCBvInPortD(dwIOAddress, pdwData) {     \
	    *(pdwData) = inl(dwIOAddress);          \
}

#define PCBvOutPortB(dwIOAddress, byData) {     \
        outb(byData, dwIOAddress);              \
}

#define PCBvOutPortW(dwIOAddress, wData) {      \
        outw(wData, dwIOAddress);               \
}

#define PCBvOutPortD(dwIOAddress, dwData) {     \
        outl(dwData, dwIOAddress);              \
}


#define PCAvDelayByIO(uDelayUnit) {             \
    BYTE    byData;                             \
    ULONG   ii;                                 \
                                                \
    if (uDelayUnit <= 50) {                     \
        udelay(uDelayUnit);                     \
    }                                           \
    else {                                      \
        for (ii = 0; ii < (uDelayUnit); ii++)   \
		     byData = inb(0x61);				\
    }                                           \
}


/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/




#endif // __UPC_H__

