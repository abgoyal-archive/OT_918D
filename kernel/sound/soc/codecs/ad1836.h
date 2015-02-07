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
 * File:         sound/soc/codecs/ad1836.h
 * Based on:
 * Author:       Barry Song <Barry.Song@analog.com>
 *
 * Created:      Aug 04, 2009
 * Description:  definitions for AD1836 registers
 *
 * Modified:
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __AD1836_H__
#define __AD1836_H__

#define AD1836_DAC_CTRL1               0
#define AD1836_DAC_POWERDOWN           2
#define AD1836_DAC_SERFMT_MASK	       0xE0
#define AD1836_DAC_SERFMT_PCK256       (0x4 << 5)
#define AD1836_DAC_SERFMT_PCK128       (0x5 << 5)
#define AD1836_DAC_WORD_LEN_MASK       0x18

#define AD1836_DAC_CTRL2               1
#define AD1836_DACL1_MUTE              0
#define AD1836_DACR1_MUTE              1
#define AD1836_DACL2_MUTE              2
#define AD1836_DACR2_MUTE              3
#define AD1836_DACL3_MUTE              4
#define AD1836_DACR3_MUTE              5

#define AD1836_DAC_L1_VOL              2
#define AD1836_DAC_R1_VOL              3
#define AD1836_DAC_L2_VOL              4
#define AD1836_DAC_R2_VOL              5
#define AD1836_DAC_L3_VOL              6
#define AD1836_DAC_R3_VOL              7

#define AD1836_ADC_CTRL1               12
#define AD1836_ADC_POWERDOWN           7
#define AD1836_ADC_HIGHPASS_FILTER     8

#define AD1836_ADC_CTRL2               13
#define AD1836_ADCL1_MUTE 		0
#define AD1836_ADCR1_MUTE 		1
#define AD1836_ADCL2_MUTE 		2
#define AD1836_ADCR2_MUTE 		3
#define AD1836_ADC_WORD_LEN_MASK       0x30
#define AD1836_ADC_SERFMT_MASK	       (7 << 6)
#define AD1836_ADC_SERFMT_PCK256       (0x4 << 6)
#define AD1836_ADC_SERFMT_PCK128       (0x5 << 6)
#define AD1836_ADC_AUX                 (0x6 << 6)

#define AD1836_ADC_CTRL3               14

#define AD1836_NUM_REGS                16

extern struct snd_soc_dai ad1836_dai;
extern struct snd_soc_codec_device soc_codec_dev_ad1836;
#endif
