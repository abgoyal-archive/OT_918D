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

/*****************************************************************************

    AudioScience HPI driver
    Copyright (C) 1997-2010  AudioScience Inc. <support@audioscience.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of version 2 of the GNU General Public License as
    published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Public declarations for DSP Proramming Interface to TI C6701

Shared between hpi6000.c and DSP code

(C) Copyright AudioScience Inc. 1998-2003
******************************************************************************/

#ifndef _HPI6000_H_
#define _HPI6000_H_

#define HPI_NMIXER_CONTROLS 200

/*
 * Control caching is always supported in the HPI code.
 * The DSP should make sure that dwControlCacheSizeInBytes is initialized to 0
 * during boot to make it in-active.
 */
struct hpi_hif_6000 {
	u32 host_cmd;
	u32 dsp_ack;
	u32 address;
	u32 length;
	u32 message_buffer_address;
	u32 response_buffer_address;
	u32 dsp_number;
	u32 adapter_info;
	u32 control_cache_is_dirty;
	u32 control_cache_address;
	u32 control_cache_size_in_bytes;
	u32 control_cache_count;
};

#define HPI_HIF_PACK_ADAPTER_INFO(adapter, version_major, version_minor) \
		((adapter << 16) | (version_major << 8) | version_minor)
#define HPI_HIF_ADAPTER_INFO_EXTRACT_ADAPTER(adapterinfo) \
		((adapterinfo >> 16) & 0xffff)
#define HPI_HIF_ADAPTER_INFO_EXTRACT_HWVERSION_MAJOR(adapterinfo) \
		((adapterinfo >> 8) & 0xff)
#define HPI_HIF_ADAPTER_INFO_EXTRACT_HWVERSION_MINOR(adapterinfo) \
		(adapterinfo & 0xff)

/* Command/status exchanged between host and DSP */
#define HPI_HIF_IDLE            0
#define HPI_HIF_SEND_MSG        1
#define HPI_HIF_GET_RESP        2
#define HPI_HIF_DATA_MASK       0x10
#define HPI_HIF_SEND_DATA       0x13
#define HPI_HIF_GET_DATA        0x14
#define HPI_HIF_SEND_DONE       5
#define HPI_HIF_RESET           9

#endif				/* _HPI6000_H_ */
