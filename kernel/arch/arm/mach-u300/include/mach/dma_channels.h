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
 *
 * arch/arm/mach-u300/include/mach/dma_channels.h
 *
 *
 * Copyright (C) 2007-2009 ST-Ericsson
 * License terms: GNU General Public License (GPL) version 2
 * Map file for the U300 dma driver.
 * Author: Per Friden <per.friden@stericsson.com>
 */

#ifndef DMA_CHANNELS_H
#define DMA_CHANNELS_H

#define U300_DMA_MSL_TX_0             0
#define U300_DMA_MSL_TX_1             1
#define U300_DMA_MSL_TX_2             2
#define U300_DMA_MSL_TX_3             3
#define U300_DMA_MSL_TX_4             4
#define U300_DMA_MSL_TX_5             5
#define U300_DMA_MSL_TX_6             6
#define U300_DMA_MSL_RX_0             7
#define U300_DMA_MSL_RX_1             8
#define U300_DMA_MSL_RX_2             9
#define U300_DMA_MSL_RX_3             10
#define U300_DMA_MSL_RX_4             11
#define U300_DMA_MSL_RX_5             12
#define U300_DMA_MSL_RX_6             13
#define U300_DMA_MMCSD_RX_TX          14
#define U300_DMA_MSPRO_TX             15
#define U300_DMA_MSPRO_RX             16
#define U300_DMA_UART0_TX             17
#define U300_DMA_UART0_RX             18
#define U300_DMA_APEX_TX              19
#define U300_DMA_APEX_RX              20
#define U300_DMA_PCM_I2S0_TX          21
#define U300_DMA_PCM_I2S0_RX          22
#define U300_DMA_PCM_I2S1_TX          23
#define U300_DMA_PCM_I2S1_RX          24
#define U300_DMA_XGAM_CDI             25
#define U300_DMA_XGAM_PDI             26
#define U300_DMA_SPI_TX               27
#define U300_DMA_SPI_RX               28
#define U300_DMA_GENERAL_PURPOSE_0    29
#define U300_DMA_GENERAL_PURPOSE_1    30
#define U300_DMA_GENERAL_PURPOSE_2    31
#define U300_DMA_GENERAL_PURPOSE_3    32
#define U300_DMA_GENERAL_PURPOSE_4    33
#define U300_DMA_GENERAL_PURPOSE_5    34
#define U300_DMA_GENERAL_PURPOSE_6    35
#define U300_DMA_GENERAL_PURPOSE_7    36
#define U300_DMA_GENERAL_PURPOSE_8    37
#ifdef CONFIG_MACH_U300_BS335
#define U300_DMA_UART1_TX             38
#define U300_DMA_UART1_RX             39
#else
#define U300_DMA_GENERAL_PURPOSE_9    38
#define U300_DMA_GENERAL_PURPOSE_10   39
#endif

#ifdef CONFIG_MACH_U300_BS335
#define U300_DMA_DEVICE_CHANNELS      32
#else
#define U300_DMA_DEVICE_CHANNELS      30
#endif
#define U300_DMA_CHANNELS             40


#endif /* DMA_CHANNELS_H */
