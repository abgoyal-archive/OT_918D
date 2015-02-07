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

/* npgtech.h - Keytable for npgtech Remote Controller
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

static struct ir_scancode npgtech[] = {
	{ 0x1d, KEY_SWITCHVIDEOMODE },	/* switch inputs */
	{ 0x2a, KEY_FRONT },

	{ 0x3e, KEY_1 },
	{ 0x02, KEY_2 },
	{ 0x06, KEY_3 },
	{ 0x0a, KEY_4 },
	{ 0x0e, KEY_5 },
	{ 0x12, KEY_6 },
	{ 0x16, KEY_7 },
	{ 0x1a, KEY_8 },
	{ 0x1e, KEY_9 },
	{ 0x3a, KEY_0 },
	{ 0x22, KEY_NUMLOCK },		/* -/-- */
	{ 0x20, KEY_REFRESH },

	{ 0x03, KEY_BRIGHTNESSDOWN },
	{ 0x28, KEY_AUDIO },
	{ 0x3c, KEY_CHANNELUP },
	{ 0x3f, KEY_VOLUMEDOWN },
	{ 0x2e, KEY_MUTE },
	{ 0x3b, KEY_VOLUMEUP },
	{ 0x00, KEY_CHANNELDOWN },
	{ 0x07, KEY_BRIGHTNESSUP },
	{ 0x2c, KEY_TEXT },

	{ 0x37, KEY_RECORD },
	{ 0x17, KEY_PLAY },
	{ 0x13, KEY_PAUSE },
	{ 0x26, KEY_STOP },
	{ 0x18, KEY_FASTFORWARD },
	{ 0x14, KEY_REWIND },
	{ 0x33, KEY_ZOOM },
	{ 0x32, KEY_KEYBOARD },
	{ 0x30, KEY_GOTO },		/* Pointing arrow */
	{ 0x36, KEY_MACRO },		/* Maximize/Minimize (yellow) */
	{ 0x0b, KEY_RADIO },
	{ 0x10, KEY_POWER },

};

static struct rc_keymap npgtech_map = {
	.map = {
		.scan    = npgtech,
		.size    = ARRAY_SIZE(npgtech),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_NPGTECH,
	}
};

static int __init init_rc_map_npgtech(void)
{
	return ir_register_map(&npgtech_map);
}

static void __exit exit_rc_map_npgtech(void)
{
	ir_unregister_map(&npgtech_map);
}

module_init(init_rc_map_npgtech)
module_exit(exit_rc_map_npgtech)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
