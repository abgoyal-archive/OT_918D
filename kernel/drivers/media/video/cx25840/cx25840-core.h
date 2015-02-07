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

/* cx25840 internal API header
 *
 * Copyright (C) 2003-2004 Chris Kennedy
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _CX25840_CORE_H_
#define _CX25840_CORE_H_


#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <linux/i2c.h>

/* ENABLE_PVR150_WORKAROUND activates a workaround for a hardware bug that is
   present in Hauppauge PVR-150 (and possibly PVR-500) cards that have
   certain NTSC tuners (tveeprom tuner model numbers 85, 99 and 112). The
   audio autodetect fails on some channels for these models and the workaround
   is to select the audio standard explicitly. Many thanks to Hauppauge for
   providing this information. */
#define CX25840_CID_ENABLE_PVR150_WORKAROUND (V4L2_CID_PRIVATE_BASE+0)

struct cx25840_state {
	struct i2c_client *c;
	struct v4l2_subdev sd;
	int pvr150_workaround;
	int radio;
	v4l2_std_id std;
	enum cx25840_video_input vid_input;
	enum cx25840_audio_input aud_input;
	u32 audclk_freq;
	int audmode;
	int unmute_volume; /* -1 if not muted */
	int default_volume;
	int vbi_line_offset;
	u32 id;
	u32 rev;
	int is_initialized;
	wait_queue_head_t fw_wait;    /* wake up when the fw load is finished */
	struct work_struct fw_work;   /* work entry for fw load */
};

static inline struct cx25840_state *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct cx25840_state, sd);
}

static inline bool is_cx2583x(struct cx25840_state *state)
{
	return state->id == V4L2_IDENT_CX25836 ||
	       state->id == V4L2_IDENT_CX25837;
}

static inline bool is_cx231xx(struct cx25840_state *state)
{
	return state->id == V4L2_IDENT_CX2310X_AV;
}

static inline bool is_cx2388x(struct cx25840_state *state)
{
	return state->id == V4L2_IDENT_CX23885_AV ||
	       state->id == V4L2_IDENT_CX23887_AV ||
	       state->id == V4L2_IDENT_CX23888_AV;
}

/* ----------------------------------------------------------------------- */
/* cx25850-core.c 							   */
int cx25840_write(struct i2c_client *client, u16 addr, u8 value);
int cx25840_write4(struct i2c_client *client, u16 addr, u32 value);
u8 cx25840_read(struct i2c_client *client, u16 addr);
u32 cx25840_read4(struct i2c_client *client, u16 addr);
int cx25840_and_or(struct i2c_client *client, u16 addr, unsigned mask, u8 value);
void cx25840_std_setup(struct i2c_client *client);

/* ----------------------------------------------------------------------- */
/* cx25850-firmware.c                                                      */
int cx25840_loadfw(struct i2c_client *client);

/* ----------------------------------------------------------------------- */
/* cx25850-audio.c                                                         */
void cx25840_audio_set_path(struct i2c_client *client);
int cx25840_s_clock_freq(struct v4l2_subdev *sd, u32 freq);
int cx25840_audio_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl);
int cx25840_audio_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl);

/* ----------------------------------------------------------------------- */
/* cx25850-vbi.c                                                           */
int cx25840_s_raw_fmt(struct v4l2_subdev *sd, struct v4l2_vbi_format *fmt);
int cx25840_s_sliced_fmt(struct v4l2_subdev *sd, struct v4l2_sliced_vbi_format *fmt);
int cx25840_g_sliced_fmt(struct v4l2_subdev *sd, struct v4l2_sliced_vbi_format *fmt);
int cx25840_decode_vbi_line(struct v4l2_subdev *sd, struct v4l2_decode_vbi_line *vbi);

#endif
