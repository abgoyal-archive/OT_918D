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
 * wm2000.h  --  WM2000 Soc Audio driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _WM2000_H
#define _WM2000_H

struct wm2000_setup_data {
	unsigned short i2c_address;
	int mclk_div;   /* Set to a non-zero value if MCLK_DIV_2 required */
};

extern int wm2000_add_controls(struct snd_soc_codec *codec);

extern struct snd_soc_dai wm2000_dai;
extern struct snd_soc_codec_device soc_codec_dev_wm2000;

#define WM2000_REG_SYS_START	    0x8000
#define WM2000_REG_SPEECH_CLARITY   0x8fef
#define WM2000_REG_SYS_WATCHDOG     0x8ff6
#define WM2000_REG_ANA_VMID_PD_TIME 0x8ff7
#define WM2000_REG_ANA_VMID_PU_TIME 0x8ff8
#define WM2000_REG_CAT_FLTR_INDX    0x8ff9
#define WM2000_REG_CAT_GAIN_0       0x8ffa
#define WM2000_REG_SYS_STATUS       0x8ffc
#define WM2000_REG_SYS_MODE_CNTRL   0x8ffd
#define WM2000_REG_SYS_START0       0x8ffe
#define WM2000_REG_SYS_START1       0x8fff
#define WM2000_REG_ID1              0xf000
#define WM2000_REG_ID2              0xf001
#define WM2000_REG_REVISON          0xf002
#define WM2000_REG_SYS_CTL1         0xf003
#define WM2000_REG_SYS_CTL2         0xf004
#define WM2000_REG_ANC_STAT         0xf005
#define WM2000_REG_IF_CTL           0xf006

/* SPEECH_CLARITY */
#define WM2000_SPEECH_CLARITY   0x01

/* SYS_STATUS */
#define WM2000_STATUS_MOUSE_ACTIVE              0x40
#define WM2000_STATUS_CAT_FREQ_COMPLETE         0x20
#define WM2000_STATUS_CAT_GAIN_COMPLETE         0x10
#define WM2000_STATUS_THERMAL_SHUTDOWN_COMPLETE 0x08
#define WM2000_STATUS_ANC_DISABLED              0x04
#define WM2000_STATUS_POWER_DOWN_COMPLETE       0x02
#define WM2000_STATUS_BOOT_COMPLETE             0x01

/* SYS_MODE_CNTRL */
#define WM2000_MODE_ANA_SEQ_INCLUDE 0x80
#define WM2000_MODE_MOUSE_ENABLE    0x40
#define WM2000_MODE_CAT_FREQ_ENABLE 0x20
#define WM2000_MODE_CAT_GAIN_ENABLE 0x10
#define WM2000_MODE_BYPASS_ENTRY    0x08
#define WM2000_MODE_STANDBY_ENTRY   0x04
#define WM2000_MODE_THERMAL_ENABLE  0x02
#define WM2000_MODE_POWER_DOWN      0x01

/* SYS_CTL1 */
#define WM2000_SYS_STBY          0x01

/* SYS_CTL2 */
#define WM2000_MCLK_DIV2_ENA_CLR 0x80
#define WM2000_MCLK_DIV2_ENA_SET 0x40
#define WM2000_ANC_ENG_CLR       0x20
#define WM2000_ANC_ENG_SET       0x10
#define WM2000_ANC_INT_N_CLR     0x08
#define WM2000_ANC_INT_N_SET     0x04
#define WM2000_RAM_CLR           0x02
#define WM2000_RAM_SET           0x01

/* ANC_STAT */
#define WM2000_ANC_ENG_IDLE      0x01

#endif
