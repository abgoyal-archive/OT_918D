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
 * stmp37xx: RTC register definitions
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
#define REGS_RTC_BASE	(STMP3XXX_REGS_BASE + 0x5C000)
#define REGS_RTC_PHYS   0x8005C000
#define REGS_RTC_SIZE   0x2000

#define HW_RTC_CTRL		0x0
#define BM_RTC_CTRL_ALARM_IRQ_EN	0x00000001
#define BP_RTC_CTRL_ALARM_IRQ_EN	0
#define BM_RTC_CTRL_ONEMSEC_IRQ_EN	0x00000002
#define BM_RTC_CTRL_ALARM_IRQ	0x00000004
#define BM_RTC_CTRL_ONEMSEC_IRQ	0x00000008
#define BM_RTC_CTRL_WATCHDOGEN	0x00000010

#define HW_RTC_STAT		0x10
#define BM_RTC_STAT_NEW_REGS	0x0000FF00
#define BP_RTC_STAT_NEW_REGS	8
#define BM_RTC_STAT_STALE_REGS	0x00FF0000
#define BP_RTC_STAT_STALE_REGS	16
#define BM_RTC_STAT_RTC_PRESENT	0x80000000

#define HW_RTC_SECONDS		0x30

#define HW_RTC_ALARM		0x40

#define HW_RTC_WATCHDOG		0x50

#define HW_RTC_PERSISTENT0	0x60
#define BM_RTC_PERSISTENT0_ALARM_WAKE_EN	0x00000002
#define BM_RTC_PERSISTENT0_ALARM_EN	0x00000004
#define BM_RTC_PERSISTENT0_XTAL24MHZ_PWRUP	0x00000010
#define BM_RTC_PERSISTENT0_XTAL32KHZ_PWRUP	0x00000020
#define BM_RTC_PERSISTENT0_ALARM_WAKE	0x00000080
#define BM_RTC_PERSISTENT0_SPARE_ANALOG	0xFFFC0000
#define BP_RTC_PERSISTENT0_SPARE_ANALOG	18

#define HW_RTC_PERSISTENT1	0x70

#define HW_RTC_VERSION		0xD0