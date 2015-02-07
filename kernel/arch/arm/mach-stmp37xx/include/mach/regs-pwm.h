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
 * stmp37xx: PWM register definitions
 *
 * Copyright (c) 2008 Freescale Semiconductor
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#define REGS_PWM_BASE	(STMP3XXX_REGS_BASE + 0x64000)

#define HW_PWM_CTRL		0x0
#define BM_PWM_CTRL_PWM2_ENABLE	0x00000004
#define BM_PWM_CTRL_PWM2_ANA_CTRL_ENABLE	0x00000020

#define HW_PWM_ACTIVE0		(0x10 + 0 * 0x20)
#define HW_PWM_ACTIVE1		(0x10 + 1 * 0x20)
#define HW_PWM_ACTIVE2		(0x10 + 2 * 0x20)
#define HW_PWM_ACTIVE3		(0x10 + 3 * 0x20)

#define HW_PWM_ACTIVEn		0x10
#define BM_PWM_ACTIVEn_ACTIVE	0x0000FFFF
#define BP_PWM_ACTIVEn_ACTIVE	0
#define BM_PWM_ACTIVEn_INACTIVE	0xFFFF0000
#define BP_PWM_ACTIVEn_INACTIVE	16

#define HW_PWM_PERIOD0		(0x20 + 0 * 0x20)
#define HW_PWM_PERIOD1		(0x20 + 1 * 0x20)
#define HW_PWM_PERIOD2		(0x20 + 2 * 0x20)
#define HW_PWM_PERIOD3		(0x20 + 3 * 0x20)

#define HW_PWM_PERIODn		0x20
#define BM_PWM_PERIODn_PERIOD	0x0000FFFF
#define BP_PWM_PERIODn_PERIOD	0
#define BM_PWM_PERIODn_ACTIVE_STATE	0x00030000
#define BP_PWM_PERIODn_ACTIVE_STATE	16
#define BM_PWM_PERIODn_INACTIVE_STATE	0x000C0000
#define BP_PWM_PERIODn_INACTIVE_STATE	18
#define BM_PWM_PERIODn_CDIV	0x00700000
#define BP_PWM_PERIODn_CDIV	20
