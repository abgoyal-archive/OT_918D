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

/* rc-pixelview-mk12.h - Keytable for pixelview Remote Controller
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

/*
 * Keytable for MK-F12 IR remote provided together with Pixelview
 * Ultra Pro Remote Controller. Uses NEC extended format.
 */
static struct ir_scancode pixelview_mk12[] = {
	{ 0x866b03, KEY_TUNER },	/* Timeshift */
	{ 0x866b1e, KEY_POWER2 },	/* power */

	{ 0x866b01, KEY_1 },
	{ 0x866b0b, KEY_2 },
	{ 0x866b1b, KEY_3 },
	{ 0x866b05, KEY_4 },
	{ 0x866b09, KEY_5 },
	{ 0x866b15, KEY_6 },
	{ 0x866b06, KEY_7 },
	{ 0x866b0a, KEY_8 },
	{ 0x866b12, KEY_9 },
	{ 0x866b02, KEY_0 },

	{ 0x866b13, KEY_AGAIN },	/* loop */
	{ 0x866b10, KEY_DIGITS },	/* +100 */

	{ 0x866b00, KEY_MEDIA },	/* source */
	{ 0x866b18, KEY_MUTE },		/* mute */
	{ 0x866b19, KEY_CAMERA },	/* snapshot */
	{ 0x866b1a, KEY_SEARCH },	/* scan */

	{ 0x866b16, KEY_CHANNELUP },	/* chn + */
	{ 0x866b14, KEY_CHANNELDOWN },	/* chn - */
	{ 0x866b1f, KEY_VOLUMEUP },	/* vol + */
	{ 0x866b17, KEY_VOLUMEDOWN },	/* vol - */
	{ 0x866b1c, KEY_ZOOM },		/* zoom */

	{ 0x866b04, KEY_REWIND },
	{ 0x866b0e, KEY_RECORD },
	{ 0x866b0c, KEY_FORWARD },

	{ 0x866b1d, KEY_STOP },
	{ 0x866b08, KEY_PLAY },
	{ 0x866b0f, KEY_PAUSE },

	{ 0x866b0d, KEY_TV },
	{ 0x866b07, KEY_RADIO },	/* FM */
};

static struct rc_keymap pixelview_map = {
	.map = {
		.scan    = pixelview_mk12,
		.size    = ARRAY_SIZE(pixelview_mk12),
		.ir_type = IR_TYPE_NEC,
		.name    = RC_MAP_PIXELVIEW_MK12,
	}
};

static int __init init_rc_map_pixelview(void)
{
	return ir_register_map(&pixelview_map);
}

static void __exit exit_rc_map_pixelview(void)
{
	ir_unregister_map(&pixelview_map);
}

module_init(init_rc_map_pixelview)
module_exit(exit_rc_map_pixelview)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
