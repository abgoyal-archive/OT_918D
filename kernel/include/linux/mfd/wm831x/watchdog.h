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
 * include/linux/mfd/wm831x/watchdog.h -- Watchdog for WM831x
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

#ifndef __MFD_WM831X_WATCHDOG_H__
#define __MFD_WM831X_WATCHDOG_H__


/*
 * R16388 (0x4004) - Watchdog
 */
#define WM831X_WDOG_ENA                         0x8000  /* WDOG_ENA */
#define WM831X_WDOG_ENA_MASK                    0x8000  /* WDOG_ENA */
#define WM831X_WDOG_ENA_SHIFT                       15  /* WDOG_ENA */
#define WM831X_WDOG_ENA_WIDTH                        1  /* WDOG_ENA */
#define WM831X_WDOG_DEBUG                       0x4000  /* WDOG_DEBUG */
#define WM831X_WDOG_DEBUG_MASK                  0x4000  /* WDOG_DEBUG */
#define WM831X_WDOG_DEBUG_SHIFT                     14  /* WDOG_DEBUG */
#define WM831X_WDOG_DEBUG_WIDTH                      1  /* WDOG_DEBUG */
#define WM831X_WDOG_RST_SRC                     0x2000  /* WDOG_RST_SRC */
#define WM831X_WDOG_RST_SRC_MASK                0x2000  /* WDOG_RST_SRC */
#define WM831X_WDOG_RST_SRC_SHIFT                   13  /* WDOG_RST_SRC */
#define WM831X_WDOG_RST_SRC_WIDTH                    1  /* WDOG_RST_SRC */
#define WM831X_WDOG_SLPENA                      0x1000  /* WDOG_SLPENA */
#define WM831X_WDOG_SLPENA_MASK                 0x1000  /* WDOG_SLPENA */
#define WM831X_WDOG_SLPENA_SHIFT                    12  /* WDOG_SLPENA */
#define WM831X_WDOG_SLPENA_WIDTH                     1  /* WDOG_SLPENA */
#define WM831X_WDOG_RESET                       0x0800  /* WDOG_RESET */
#define WM831X_WDOG_RESET_MASK                  0x0800  /* WDOG_RESET */
#define WM831X_WDOG_RESET_SHIFT                     11  /* WDOG_RESET */
#define WM831X_WDOG_RESET_WIDTH                      1  /* WDOG_RESET */
#define WM831X_WDOG_SECACT_MASK                 0x0300  /* WDOG_SECACT - [9:8] */
#define WM831X_WDOG_SECACT_SHIFT                     8  /* WDOG_SECACT - [9:8] */
#define WM831X_WDOG_SECACT_WIDTH                     2  /* WDOG_SECACT - [9:8] */
#define WM831X_WDOG_PRIMACT_MASK                0x0030  /* WDOG_PRIMACT - [5:4] */
#define WM831X_WDOG_PRIMACT_SHIFT                    4  /* WDOG_PRIMACT - [5:4] */
#define WM831X_WDOG_PRIMACT_WIDTH                    2  /* WDOG_PRIMACT - [5:4] */
#define WM831X_WDOG_TO_MASK                     0x0007  /* WDOG_TO - [2:0] */
#define WM831X_WDOG_TO_SHIFT                         0  /* WDOG_TO - [2:0] */
#define WM831X_WDOG_TO_WIDTH                         3  /* WDOG_TO - [2:0] */

#endif
