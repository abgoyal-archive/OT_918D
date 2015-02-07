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

/* pinnacle-color.h - Keytable for pinnacle_color Remote Controller
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

static struct ir_scancode pinnacle_color[] = {
	{ 0x59, KEY_MUTE },
	{ 0x4a, KEY_POWER },

	{ 0x18, KEY_TEXT },
	{ 0x26, KEY_TV },
	{ 0x3d, KEY_PRINT },

	{ 0x48, KEY_RED },
	{ 0x04, KEY_GREEN },
	{ 0x11, KEY_YELLOW },
	{ 0x00, KEY_BLUE },

	{ 0x2d, KEY_VOLUMEUP },
	{ 0x1e, KEY_VOLUMEDOWN },

	{ 0x49, KEY_MENU },

	{ 0x16, KEY_CHANNELUP },
	{ 0x17, KEY_CHANNELDOWN },

	{ 0x20, KEY_UP },
	{ 0x21, KEY_DOWN },
	{ 0x22, KEY_LEFT },
	{ 0x23, KEY_RIGHT },
	{ 0x0d, KEY_SELECT },

	{ 0x08, KEY_BACK },
	{ 0x07, KEY_REFRESH },

	{ 0x2f, KEY_ZOOM },
	{ 0x29, KEY_RECORD },

	{ 0x4b, KEY_PAUSE },
	{ 0x4d, KEY_REWIND },
	{ 0x2e, KEY_PLAY },
	{ 0x4e, KEY_FORWARD },
	{ 0x53, KEY_PREVIOUS },
	{ 0x4c, KEY_STOP },
	{ 0x54, KEY_NEXT },

	{ 0x69, KEY_0 },
	{ 0x6a, KEY_1 },
	{ 0x6b, KEY_2 },
	{ 0x6c, KEY_3 },
	{ 0x6d, KEY_4 },
	{ 0x6e, KEY_5 },
	{ 0x6f, KEY_6 },
	{ 0x70, KEY_7 },
	{ 0x71, KEY_8 },
	{ 0x72, KEY_9 },

	{ 0x74, KEY_CHANNEL },
	{ 0x0a, KEY_BACKSPACE },
};

static struct rc_keymap pinnacle_color_map = {
	.map = {
		.scan    = pinnacle_color,
		.size    = ARRAY_SIZE(pinnacle_color),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_PINNACLE_COLOR,
	}
};

static int __init init_rc_map_pinnacle_color(void)
{
	return ir_register_map(&pinnacle_color_map);
}

static void __exit exit_rc_map_pinnacle_color(void)
{
	ir_unregister_map(&pinnacle_color_map);
}

module_init(init_rc_map_pinnacle_color)
module_exit(exit_rc_map_pinnacle_color)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
