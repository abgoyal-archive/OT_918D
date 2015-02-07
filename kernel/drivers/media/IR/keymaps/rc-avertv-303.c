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

/* avertv-303.h - Keytable for avertv_303 Remote Controller
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

/* AVERTV STUDIO 303 Remote */

static struct ir_scancode avertv_303[] = {
	{ 0x2a, KEY_1 },
	{ 0x32, KEY_2 },
	{ 0x3a, KEY_3 },
	{ 0x4a, KEY_4 },
	{ 0x52, KEY_5 },
	{ 0x5a, KEY_6 },
	{ 0x6a, KEY_7 },
	{ 0x72, KEY_8 },
	{ 0x7a, KEY_9 },
	{ 0x0e, KEY_0 },

	{ 0x02, KEY_POWER },
	{ 0x22, KEY_VIDEO },
	{ 0x42, KEY_AUDIO },
	{ 0x62, KEY_ZOOM },
	{ 0x0a, KEY_TV },
	{ 0x12, KEY_CD },
	{ 0x1a, KEY_TEXT },

	{ 0x16, KEY_SUBTITLE },
	{ 0x1e, KEY_REWIND },
	{ 0x06, KEY_PRINT },

	{ 0x2e, KEY_SEARCH },
	{ 0x36, KEY_SLEEP },
	{ 0x3e, KEY_SHUFFLE },
	{ 0x26, KEY_MUTE },

	{ 0x4e, KEY_RECORD },
	{ 0x56, KEY_PAUSE },
	{ 0x5e, KEY_STOP },
	{ 0x46, KEY_PLAY },

	{ 0x6e, KEY_RED },
	{ 0x0b, KEY_GREEN },
	{ 0x66, KEY_YELLOW },
	{ 0x03, KEY_BLUE },

	{ 0x76, KEY_LEFT },
	{ 0x7e, KEY_RIGHT },
	{ 0x13, KEY_DOWN },
	{ 0x1b, KEY_UP },
};

static struct rc_keymap avertv_303_map = {
	.map = {
		.scan    = avertv_303,
		.size    = ARRAY_SIZE(avertv_303),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_AVERTV_303,
	}
};

static int __init init_rc_map_avertv_303(void)
{
	return ir_register_map(&avertv_303_map);
}

static void __exit exit_rc_map_avertv_303(void)
{
	ir_unregister_map(&avertv_303_map);
}

module_init(init_rc_map_avertv_303)
module_exit(exit_rc_map_avertv_303)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
