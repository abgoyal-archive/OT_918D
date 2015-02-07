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
 * include/linux/mfd/wm831x/status.h -- Status LEDs for WM831x
 *
 * Copyright 2009 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#ifndef __MFD_WM831X_STATUS_H__
#define __MFD_WM831X_STATUS_H__

#define WM831X_LED_SRC_MASK                    0xC000  /* LED_SRC - [15:14] */
#define WM831X_LED_SRC_SHIFT                       14  /* LED_SRC - [15:14] */
#define WM831X_LED_SRC_WIDTH                        2  /* LED_SRC - [15:14] */
#define WM831X_LED_MODE_MASK                   0x0300  /* LED_MODE - [9:8] */
#define WM831X_LED_MODE_SHIFT                       8  /* LED_MODE - [9:8] */
#define WM831X_LED_MODE_WIDTH                       2  /* LED_MODE - [9:8] */
#define WM831X_LED_SEQ_LEN_MASK                0x0030  /* LED_SEQ_LEN - [5:4] */
#define WM831X_LED_SEQ_LEN_SHIFT                    4  /* LED_SEQ_LEN - [5:4] */
#define WM831X_LED_SEQ_LEN_WIDTH                    2  /* LED_SEQ_LEN - [5:4] */
#define WM831X_LED_DUR_MASK                    0x000C  /* LED_DUR - [3:2] */
#define WM831X_LED_DUR_SHIFT                        2  /* LED_DUR - [3:2] */
#define WM831X_LED_DUR_WIDTH                        2  /* LED_DUR - [3:2] */
#define WM831X_LED_DUTY_CYC_MASK               0x0003  /* LED_DUTY_CYC - [1:0] */
#define WM831X_LED_DUTY_CYC_SHIFT                   0  /* LED_DUTY_CYC - [1:0] */
#define WM831X_LED_DUTY_CYC_WIDTH                   2  /* LED_DUTY_CYC - [1:0] */

#endif
