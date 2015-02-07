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
 * include/linux/mfd/wm8994/gpio.h - GPIO configuration for WM8994
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

#ifndef __MFD_WM8994_GPIO_H__
#define __MFD_WM8994_GPIO_H__

#define WM8994_GPIO_MAX 11

#define WM8994_GP_FN_PIN_SPECIFIC    0
#define WM8994_GP_FN_GPIO            1
#define WM8994_GP_FN_SDOUT           2
#define WM8994_GP_FN_IRQ             3
#define WM8994_GP_FN_TEMPERATURE     4
#define WM8994_GP_FN_MICBIAS1_DET    5
#define WM8994_GP_FN_MICBIAS1_SHORT  6
#define WM8994_GP_FN_MICBIAS2_DET    7
#define WM8994_GP_FN_MICBIAS2_SHORT  8
#define WM8994_GP_FN_FLL1_LOCK       9
#define WM8994_GP_FN_FLL2_LOCK      10
#define WM8994_GP_FN_SRC1_LOCK      11
#define WM8994_GP_FN_SRC2_LOCK      12
#define WM8994_GP_FN_DRC1_ACT       13
#define WM8994_GP_FN_DRC2_ACT       14
#define WM8994_GP_FN_DRC3_ACT       15
#define WM8994_GP_FN_WSEQ_STATUS    16
#define WM8994_GP_FN_FIFO_ERROR     17
#define WM8994_GP_FN_OPCLK          18

#define WM8994_GPN_DIR                          0x8000  /* GPN_DIR */
#define WM8994_GPN_DIR_MASK                     0x8000  /* GPN_DIR */
#define WM8994_GPN_DIR_SHIFT                        15  /* GPN_DIR */
#define WM8994_GPN_DIR_WIDTH                         1  /* GPN_DIR */
#define WM8994_GPN_PU                           0x4000  /* GPN_PU */
#define WM8994_GPN_PU_MASK                      0x4000  /* GPN_PU */
#define WM8994_GPN_PU_SHIFT                         14  /* GPN_PU */
#define WM8994_GPN_PU_WIDTH                          1  /* GPN_PU */
#define WM8994_GPN_PD                           0x2000  /* GPN_PD */
#define WM8994_GPN_PD_MASK                      0x2000  /* GPN_PD */
#define WM8994_GPN_PD_SHIFT                         13  /* GPN_PD */
#define WM8994_GPN_PD_WIDTH                          1  /* GPN_PD */
#define WM8994_GPN_POL                          0x0400  /* GPN_POL */
#define WM8994_GPN_POL_MASK                     0x0400  /* GPN_POL */
#define WM8994_GPN_POL_SHIFT                        10  /* GPN_POL */
#define WM8994_GPN_POL_WIDTH                         1  /* GPN_POL */
#define WM8994_GPN_OP_CFG                       0x0200  /* GPN_OP_CFG */
#define WM8994_GPN_OP_CFG_MASK                  0x0200  /* GPN_OP_CFG */
#define WM8994_GPN_OP_CFG_SHIFT                      9  /* GPN_OP_CFG */
#define WM8994_GPN_OP_CFG_WIDTH                      1  /* GPN_OP_CFG */
#define WM8994_GPN_DB                           0x0100  /* GPN_DB */
#define WM8994_GPN_DB_MASK                      0x0100  /* GPN_DB */
#define WM8994_GPN_DB_SHIFT                          8  /* GPN_DB */
#define WM8994_GPN_DB_WIDTH                          1  /* GPN_DB */
#define WM8994_GPN_LVL                          0x0040  /* GPN_LVL */
#define WM8994_GPN_LVL_MASK                     0x0040  /* GPN_LVL */
#define WM8994_GPN_LVL_SHIFT                         6  /* GPN_LVL */
#define WM8994_GPN_LVL_WIDTH                         1  /* GPN_LVL */
#define WM8994_GPN_FN_MASK                      0x001F  /* GPN_FN - [4:0] */
#define WM8994_GPN_FN_SHIFT                          0  /* GPN_FN - [4:0] */
#define WM8994_GPN_FN_WIDTH                          5  /* GPN_FN - [4:0] */

#endif
