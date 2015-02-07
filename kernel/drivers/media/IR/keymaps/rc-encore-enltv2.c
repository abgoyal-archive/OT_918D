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

/* encore-enltv2.h - Keytable for encore_enltv2 Remote Controller
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

/* Encore ENLTV2-FM  - silver plastic - "Wand Media" written at the botton
    Mauro Carvalho Chehab <mchehab@infradead.org> */

static struct ir_scancode encore_enltv2[] = {
	{ 0x4c, KEY_POWER2 },
	{ 0x4a, KEY_TUNER },
	{ 0x40, KEY_1 },
	{ 0x60, KEY_2 },
	{ 0x50, KEY_3 },
	{ 0x70, KEY_4 },
	{ 0x48, KEY_5 },
	{ 0x68, KEY_6 },
	{ 0x58, KEY_7 },
	{ 0x78, KEY_8 },
	{ 0x44, KEY_9 },
	{ 0x54, KEY_0 },

	{ 0x64, KEY_LAST },		/* +100 */
	{ 0x4e, KEY_AGAIN },		/* Recall */

	{ 0x6c, KEY_SWITCHVIDEOMODE },	/* Video Source */
	{ 0x5e, KEY_MENU },
	{ 0x56, KEY_SCREEN },
	{ 0x7a, KEY_SETUP },

	{ 0x46, KEY_MUTE },
	{ 0x5c, KEY_MODE },		/* Stereo */
	{ 0x74, KEY_INFO },
	{ 0x7c, KEY_CLEAR },

	{ 0x55, KEY_UP },
	{ 0x49, KEY_DOWN },
	{ 0x7e, KEY_LEFT },
	{ 0x59, KEY_RIGHT },
	{ 0x6a, KEY_ENTER },

	{ 0x42, KEY_VOLUMEUP },
	{ 0x62, KEY_VOLUMEDOWN },
	{ 0x52, KEY_CHANNELUP },
	{ 0x72, KEY_CHANNELDOWN },

	{ 0x41, KEY_RECORD },
	{ 0x51, KEY_CAMERA },		/* Snapshot */
	{ 0x75, KEY_TIME },		/* Timeshift */
	{ 0x71, KEY_TV2 },		/* PIP */

	{ 0x45, KEY_REWIND },
	{ 0x6f, KEY_PAUSE },
	{ 0x7d, KEY_FORWARD },
	{ 0x79, KEY_STOP },
};

static struct rc_keymap encore_enltv2_map = {
	.map = {
		.scan    = encore_enltv2,
		.size    = ARRAY_SIZE(encore_enltv2),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_ENCORE_ENLTV2,
	}
};

static int __init init_rc_map_encore_enltv2(void)
{
	return ir_register_map(&encore_enltv2_map);
}

static void __exit exit_rc_map_encore_enltv2(void)
{
	ir_unregister_map(&encore_enltv2_map);
}

module_init(init_rc_map_encore_enltv2)
module_exit(exit_rc_map_encore_enltv2)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
