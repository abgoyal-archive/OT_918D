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
    cx25840.h - definition for cx25840/1/2/3 inputs

    Copyright (C) 2006 Hans Verkuil (hverkuil@xs4all.nl)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef _CX25840_H_
#define _CX25840_H_

/* Note that the cx25840 driver requires that the bridge driver calls the
   v4l2_subdev's init operation in order to load the driver's firmware.
   Without this the audio standard detection will fail and you will
   only get mono.

   Since loading the firmware is often problematic when the driver is
   compiled into the kernel I recommend postponing calling this function
   until the first open of the video device. Another reason for
   postponing it is that loading this firmware takes a long time (seconds)
   due to the slow i2c bus speed. So it will speed up the boot process if
   you can avoid loading the fw as long as the video device isn't used. */

enum cx25840_video_input {
	/* Composite video inputs In1-In8 */
	CX25840_COMPOSITE1 = 1,
	CX25840_COMPOSITE2,
	CX25840_COMPOSITE3,
	CX25840_COMPOSITE4,
	CX25840_COMPOSITE5,
	CX25840_COMPOSITE6,
	CX25840_COMPOSITE7,
	CX25840_COMPOSITE8,

	/* S-Video inputs consist of one luma input (In1-In8) ORed with one
	   chroma input (In5-In8) */
	CX25840_SVIDEO_LUMA1 = 0x10,
	CX25840_SVIDEO_LUMA2 = 0x20,
	CX25840_SVIDEO_LUMA3 = 0x30,
	CX25840_SVIDEO_LUMA4 = 0x40,
	CX25840_SVIDEO_LUMA5 = 0x50,
	CX25840_SVIDEO_LUMA6 = 0x60,
	CX25840_SVIDEO_LUMA7 = 0x70,
	CX25840_SVIDEO_LUMA8 = 0x80,
	CX25840_SVIDEO_CHROMA4 = 0x400,
	CX25840_SVIDEO_CHROMA5 = 0x500,
	CX25840_SVIDEO_CHROMA6 = 0x600,
	CX25840_SVIDEO_CHROMA7 = 0x700,
	CX25840_SVIDEO_CHROMA8 = 0x800,

	/* S-Video aliases for common luma/chroma combinations */
	CX25840_SVIDEO1 = 0x510,
	CX25840_SVIDEO2 = 0x620,
	CX25840_SVIDEO3 = 0x730,
	CX25840_SVIDEO4 = 0x840,

	/* Allow frames to specify specific input configurations */
	CX25840_VIN1_CH1  = 0x80000000,
	CX25840_VIN2_CH1  = 0x80000001,
	CX25840_VIN3_CH1  = 0x80000002,
	CX25840_VIN4_CH1  = 0x80000003,
	CX25840_VIN5_CH1  = 0x80000004,
	CX25840_VIN6_CH1  = 0x80000005,
	CX25840_VIN7_CH1  = 0x80000006,
	CX25840_VIN8_CH1  = 0x80000007,
	CX25840_VIN4_CH2  = 0x80000000,
	CX25840_VIN5_CH2  = 0x80000010,
	CX25840_VIN6_CH2  = 0x80000020,
	CX25840_NONE_CH2  = 0x80000030,
	CX25840_VIN7_CH3  = 0x80000000,
	CX25840_VIN8_CH3  = 0x80000040,
	CX25840_NONE0_CH3 = 0x80000080,
	CX25840_NONE1_CH3 = 0x800000c0,
	CX25840_SVIDEO_ON = 0x80000100,
	CX25840_COMPONENT_ON = 0x80000200,
};

enum cx25840_audio_input {
	/* Audio inputs: serial or In4-In8 */
	CX25840_AUDIO_SERIAL,
	CX25840_AUDIO4 = 4,
	CX25840_AUDIO5,
	CX25840_AUDIO6,
	CX25840_AUDIO7,
	CX25840_AUDIO8,
};

#endif
