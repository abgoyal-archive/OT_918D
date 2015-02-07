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
 *  Driver for the Auvitek USB bridge
 *
 *  Copyright (c) 2008 Steven Toth <stoth@linuxtv.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* We'll start to rename these registers once we have a better
 * understanding of their meaning.
 */
#define REG_000 0x000
#define REG_001 0x001
#define REG_002 0x002
#define REG_003 0x003

#define AU0828_SENSORCTRL_100 0x100
#define AU0828_SENSORCTRL_VBI_103 0x103

/* I2C registers */
#define AU0828_I2C_TRIGGER_200		0x200
#define AU0828_I2C_STATUS_201		0x201
#define AU0828_I2C_CLK_DIVIDER_202	0x202
#define AU0828_I2C_DEST_ADDR_203	0x203
#define AU0828_I2C_WRITE_FIFO_205	0x205
#define AU0828_I2C_READ_FIFO_209	0x209
#define AU0828_I2C_MULTIBYTE_MODE_2FF	0x2ff

/* Audio registers */
#define AU0828_AUDIOCTRL_50C 0x50C

#define REG_600 0x600

/*********************************************************************/
/* Here are constants for values associated with the above registers */

/* I2C Trigger (Reg 0x200) */
#define AU0828_I2C_TRIGGER_WRITE	0x01
#define AU0828_I2C_TRIGGER_READ		0x20
#define AU0828_I2C_TRIGGER_HOLD		0x40

/* I2C Status (Reg 0x201) */
#define AU0828_I2C_STATUS_READ_DONE	0x01
#define AU0828_I2C_STATUS_NO_READ_ACK	0x02
#define AU0828_I2C_STATUS_WRITE_DONE	0x04
#define AU0828_I2C_STATUS_NO_WRITE_ACK	0x08
#define AU0828_I2C_STATUS_BUSY		0x10

/* I2C Clock Divider (Reg 0x202) */
#define AU0828_I2C_CLK_250KHZ 0x07
#define AU0828_I2C_CLK_100KHZ 0x14
#define AU0828_I2C_CLK_30KHZ  0x40
