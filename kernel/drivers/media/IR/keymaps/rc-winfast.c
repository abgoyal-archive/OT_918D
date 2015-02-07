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

/* winfast.h - Keytable for winfast Remote Controller
 *
 * keymap imported from ir-keymaps.c
 *
 * Copyright (c) 2010 by Mauro Carvalho Chehab <mchehab@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <media/rc-map.h>

/* Table for Leadtek Winfast Remote Controls - used by both bttv and cx88 */

static struct ir_scancode winfast[] = {
	/* Keys 0 to 9 */
	{ 0x12, KEY_0 },
	{ 0x05, KEY_1 },
	{ 0x06, KEY_2 },
	{ 0x07, KEY_3 },
	{ 0x09, KEY_4 },
	{ 0x0a, KEY_5 },
	{ 0x0b, KEY_6 },
	{ 0x0d, KEY_7 },
	{ 0x0e, KEY_8 },
	{ 0x0f, KEY_9 },

	{ 0x00, KEY_POWER },
	{ 0x1b, KEY_AUDIO },		/* Audio Source */
	{ 0x02, KEY_TUNER },		/* TV/FM, not on Y0400052 */
	{ 0x1e, KEY_VIDEO },		/* Video Source */
	{ 0x16, KEY_INFO },		/* Display information */
	{ 0x04, KEY_VOLUMEUP },
	{ 0x08, KEY_VOLUMEDOWN },
	{ 0x0c, KEY_CHANNELUP },
	{ 0x10, KEY_CHANNELDOWN },
	{ 0x03, KEY_ZOOM },		/* fullscreen */
	{ 0x1f, KEY_TEXT },		/* closed caption/teletext */
	{ 0x20, KEY_SLEEP },
	{ 0x29, KEY_CLEAR },		/* boss key */
	{ 0x14, KEY_MUTE },
	{ 0x2b, KEY_RED },
	{ 0x2c, KEY_GREEN },
	{ 0x2d, KEY_YELLOW },
	{ 0x2e, KEY_BLUE },
	{ 0x18, KEY_KPPLUS },		/* fine tune + , not on Y040052 */
	{ 0x19, KEY_KPMINUS },		/* fine tune - , not on Y040052 */
	{ 0x2a, KEY_MEDIA },		/* PIP (Picture in picture */
	{ 0x21, KEY_DOT },
	{ 0x13, KEY_ENTER },
	{ 0x11, KEY_LAST },		/* Recall (last channel */
	{ 0x22, KEY_PREVIOUS },
	{ 0x23, KEY_PLAYPAUSE },
	{ 0x24, KEY_NEXT },
	{ 0x25, KEY_TIME },		/* Time Shifting */
	{ 0x26, KEY_STOP },
	{ 0x27, KEY_RECORD },
	{ 0x28, KEY_SAVE },		/* Screenshot */
	{ 0x2f, KEY_MENU },
	{ 0x30, KEY_CANCEL },
	{ 0x31, KEY_CHANNEL },		/* Channel Surf */
	{ 0x32, KEY_SUBTITLE },
	{ 0x33, KEY_LANGUAGE },
	{ 0x34, KEY_REWIND },
	{ 0x35, KEY_FASTFORWARD },
	{ 0x36, KEY_TV },
	{ 0x37, KEY_RADIO },		/* FM */
	{ 0x38, KEY_DVD },

	{ 0x1a, KEY_MODE},		/* change to MCE mode on Y04G0051 */
	{ 0x3e, KEY_F21 },		/* MCE +VOL, on Y04G0033 */
	{ 0x3a, KEY_F22 },		/* MCE -VOL, on Y04G0033 */
	{ 0x3b, KEY_F23 },		/* MCE +CH,  on Y04G0033 */
	{ 0x3f, KEY_F24 }		/* MCE -CH,  on Y04G0033 */
};

static struct rc_keymap winfast_map = {
	.map = {
		.scan    = winfast,
		.size    = ARRAY_SIZE(winfast),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_WINFAST,
	}
};

static int __init init_rc_map_winfast(void)
{
	return ir_register_map(&winfast_map);
}

static void __exit exit_rc_map_winfast(void)
{
	ir_unregister_map(&winfast_map);
}

module_init(init_rc_map_winfast)
module_exit(exit_rc_map_winfast)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
