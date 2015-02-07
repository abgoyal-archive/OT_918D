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

#ifndef _DISPC_H
#define _DISPC_H

#include <linux/interrupt.h>

#define DISPC_PLANE_GFX			0
#define DISPC_PLANE_VID1		1
#define DISPC_PLANE_VID2		2

#define DISPC_RGB_1_BPP			0x00
#define DISPC_RGB_2_BPP			0x01
#define DISPC_RGB_4_BPP			0x02
#define DISPC_RGB_8_BPP			0x03
#define DISPC_RGB_12_BPP		0x04
#define DISPC_RGB_16_BPP		0x06
#define DISPC_RGB_24_BPP		0x08
#define DISPC_RGB_24_BPP_UNPACK_32	0x09
#define DISPC_YUV2_422			0x0a
#define DISPC_UYVY_422			0x0b

#define DISPC_BURST_4x32		0
#define DISPC_BURST_8x32		1
#define DISPC_BURST_16x32		2

#define DISPC_LOAD_CLUT_AND_FRAME	0x00
#define DISPC_LOAD_CLUT_ONLY		0x01
#define DISPC_LOAD_FRAME_ONLY		0x02
#define DISPC_LOAD_CLUT_ONCE_FRAME	0x03

#define DISPC_TFT_DATA_LINES_12		0
#define DISPC_TFT_DATA_LINES_16		1
#define DISPC_TFT_DATA_LINES_18		2
#define DISPC_TFT_DATA_LINES_24		3

extern void omap_dispc_set_lcd_size(int width, int height);

extern void omap_dispc_enable_lcd_out(int enable);
extern void omap_dispc_enable_digit_out(int enable);

extern int omap_dispc_request_irq(unsigned long irq_mask,
				   void (*callback)(void *data), void *data);
extern void omap_dispc_free_irq(unsigned long irq_mask,
				 void (*callback)(void *data), void *data);

extern const struct lcd_ctrl omap2_int_ctrl;
#endif
