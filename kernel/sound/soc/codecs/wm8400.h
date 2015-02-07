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
 * wm8400.h  --  audio driver for WM8400
 *
 * Copyright 2008 Wolfson Microelectronics PLC.
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#ifndef _WM8400_CODEC_H
#define _WM8400_CODEC_H

#define WM8400_MCLK_DIV 0
#define WM8400_DACCLK_DIV 1
#define WM8400_ADCCLK_DIV 2
#define WM8400_BCLK_DIV 3

#define WM8400_MCLK_DIV_1 0x400
#define WM8400_MCLK_DIV_2 0x800

#define WM8400_DAC_CLKDIV_1    0x00
#define WM8400_DAC_CLKDIV_1_5  0x04
#define WM8400_DAC_CLKDIV_2    0x08
#define WM8400_DAC_CLKDIV_3    0x0c
#define WM8400_DAC_CLKDIV_4    0x10
#define WM8400_DAC_CLKDIV_5_5  0x14
#define WM8400_DAC_CLKDIV_6    0x18

#define WM8400_ADC_CLKDIV_1    0x00
#define WM8400_ADC_CLKDIV_1_5  0x20
#define WM8400_ADC_CLKDIV_2    0x40
#define WM8400_ADC_CLKDIV_3    0x60
#define WM8400_ADC_CLKDIV_4    0x80
#define WM8400_ADC_CLKDIV_5_5  0xa0
#define WM8400_ADC_CLKDIV_6    0xc0


#define WM8400_BCLK_DIV_1                       (0x0 << 1)
#define WM8400_BCLK_DIV_1_5                     (0x1 << 1)
#define WM8400_BCLK_DIV_2                       (0x2 << 1)
#define WM8400_BCLK_DIV_3                       (0x3 << 1)
#define WM8400_BCLK_DIV_4                       (0x4 << 1)
#define WM8400_BCLK_DIV_5_5                     (0x5 << 1)
#define WM8400_BCLK_DIV_6                       (0x6 << 1)
#define WM8400_BCLK_DIV_8                       (0x7 << 1)
#define WM8400_BCLK_DIV_11                      (0x8 << 1)
#define WM8400_BCLK_DIV_12                      (0x9 << 1)
#define WM8400_BCLK_DIV_16                      (0xA << 1)
#define WM8400_BCLK_DIV_22                      (0xB << 1)
#define WM8400_BCLK_DIV_24                      (0xC << 1)
#define WM8400_BCLK_DIV_32                      (0xD << 1)
#define WM8400_BCLK_DIV_44                      (0xE << 1)
#define WM8400_BCLK_DIV_48                      (0xF << 1)

extern struct snd_soc_dai wm8400_dai;
extern struct snd_soc_codec_device soc_codec_dev_wm8400;

#endif
