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
 *  arch/arm/include/asm/hardware/ep7212.h
 *
 *  This file contains the hardware definitions of the EP7212 internal
 *  registers.
 *
 *  Copyright (C) 2000 Deep Blue Solutions Ltd.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __ASM_HARDWARE_EP7212_H
#define __ASM_HARDWARE_EP7212_H

/*
 * define EP7212_BASE to be the base address of the region
 * you want to access.
 */

#define EP7212_PHYS_BASE	(0x80000000)

#ifndef __ASSEMBLY__
#define ep_readl(off)		__raw_readl(EP7212_BASE + (off))
#define ep_writel(val,off)	__raw_writel(val, EP7212_BASE + (off))
#endif

/*
 * These registers are specific to the EP7212 only
 */
#define DAIR			0x2000
#define DAIR0			0x2040
#define DAIDR1			0x2080
#define DAIDR2			0x20c0
#define DAISR			0x2100
#define SYSCON3			0x2200
#define INTSR3			0x2240
#define INTMR3			0x2280
#define LEDFLSH			0x22c0

#define DAIR_DAIEN		(1 << 16)
#define DAIR_ECS		(1 << 17)
#define DAIR_LCTM		(1 << 19)
#define DAIR_LCRM		(1 << 20)
#define DAIR_RCTM		(1 << 21)
#define DAIR_RCRM		(1 << 22)
#define DAIR_LBM		(1 << 23)

#define DAIDR2_FIFOEN		(1 << 15)
#define DAIDR2_FIFOLEFT		(0x0d << 16)
#define DAIDR2_FIFORIGHT	(0x11 << 16)

#define DAISR_RCTS		(1 << 0)
#define DAISR_RCRS		(1 << 1)
#define DAISR_LCTS		(1 << 2)
#define DAISR_LCRS		(1 << 3)
#define DAISR_RCTU		(1 << 4)
#define DAISR_RCRO		(1 << 5)
#define DAISR_LCTU		(1 << 6)
#define DAISR_LCRO		(1 << 7)
#define DAISR_RCNF		(1 << 8)
#define DAISR_RCNE		(1 << 9)
#define DAISR_LCNF		(1 << 10)
#define DAISR_LCNE		(1 << 11)
#define DAISR_FIFO		(1 << 12)

#define SYSCON3_ADCCON		(1 << 0)
#define SYSCON3_DAISEL		(1 << 3)
#define SYSCON3_ADCCKNSEN	(1 << 4)
#define SYSCON3_FASTWAKE	(1 << 8)
#define SYSCON3_DAIEN		(1 << 9)

#endif /* __ASM_HARDWARE_EP7212_H */
