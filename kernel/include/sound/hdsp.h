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

#ifndef __SOUND_HDSP_H
#define __SOUND_HDSP_H

/*
 *   Copyright (C) 2003 Thomas Charbonnel (thomas@undata.org)
 *    
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/types.h>

#define HDSP_MATRIX_MIXER_SIZE 2048

enum HDSP_IO_Type {
	Digiface,
	Multiface,
	H9652,
	H9632,
	Undefined,
};

struct hdsp_peak_rms {
	__u32 input_peaks[26];
	__u32 playback_peaks[26];
	__u32 output_peaks[28];
	__u64 input_rms[26];
	__u64 playback_rms[26];
	/* These are only used for H96xx cards */
	__u64 output_rms[26];
};

#define SNDRV_HDSP_IOCTL_GET_PEAK_RMS _IOR('H', 0x40, struct hdsp_peak_rms)

struct hdsp_config_info {
	unsigned char pref_sync_ref;
	unsigned char wordclock_sync_check;
	unsigned char spdif_sync_check;
	unsigned char adatsync_sync_check;
	unsigned char adat_sync_check[3];
	unsigned char spdif_in;
	unsigned char spdif_out;
	unsigned char spdif_professional;
	unsigned char spdif_emphasis;
	unsigned char spdif_nonaudio;
	unsigned int spdif_sample_rate;
	unsigned int system_sample_rate;
	unsigned int autosync_sample_rate;
	unsigned char system_clock_mode;
	unsigned char clock_source;
	unsigned char autosync_ref;
	unsigned char line_out;
	unsigned char passthru; 
	unsigned char da_gain;
	unsigned char ad_gain;
	unsigned char phone_gain;
	unsigned char xlr_breakout_cable;
	unsigned char analog_extension_board;
};

#define SNDRV_HDSP_IOCTL_GET_CONFIG_INFO _IOR('H', 0x41, struct hdsp_config_info)

struct hdsp_firmware {
	void __user *firmware_data;	/* 24413 x 4 bytes */
};

#define SNDRV_HDSP_IOCTL_UPLOAD_FIRMWARE _IOW('H', 0x42, struct hdsp_firmware)

struct hdsp_version {
	enum HDSP_IO_Type io_type;
	unsigned short firmware_rev;
};

#define SNDRV_HDSP_IOCTL_GET_VERSION _IOR('H', 0x43, struct hdsp_version)

struct hdsp_mixer {
	unsigned short matrix[HDSP_MATRIX_MIXER_SIZE];
};

#define SNDRV_HDSP_IOCTL_GET_MIXER _IOR('H', 0x44, struct hdsp_mixer)

struct hdsp_9632_aeb {
	int aebi;
	int aebo;
};

#define SNDRV_HDSP_IOCTL_GET_9632_AEB _IOR('H', 0x45, struct hdsp_9632_aeb)

/* typedefs for compatibility to user-space */
typedef enum HDSP_IO_Type HDSP_IO_Type;
typedef struct hdsp_peak_rms hdsp_peak_rms_t;
typedef struct hdsp_config_info hdsp_config_info_t;
typedef struct hdsp_firmware hdsp_firmware_t;
typedef struct hdsp_version hdsp_version_t;
typedef struct hdsp_mixer hdsp_mixer_t;
typedef struct hdsp_9632_aeb hdsp_9632_aeb_t;

#endif /* __SOUND_HDSP_H */
