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

/* gadmei-rm008z.h - Keytable for gadmei_rm008z Remote Controller
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

/* GADMEI UTV330+ RM008Z remote
   Shine Liu <shinel@foxmail.com>
 */

static struct ir_scancode gadmei_rm008z[] = {
	{ 0x14, KEY_POWER2},		/* POWER OFF */
	{ 0x0c, KEY_MUTE},		/* MUTE */

	{ 0x18, KEY_TV},		/* TV */
	{ 0x0e, KEY_VIDEO},		/* AV */
	{ 0x0b, KEY_AUDIO},		/* SV */
	{ 0x0f, KEY_RADIO},		/* FM */

	{ 0x00, KEY_1},
	{ 0x01, KEY_2},
	{ 0x02, KEY_3},
	{ 0x03, KEY_4},
	{ 0x04, KEY_5},
	{ 0x05, KEY_6},
	{ 0x06, KEY_7},
	{ 0x07, KEY_8},
	{ 0x08, KEY_9},
	{ 0x09, KEY_0},
	{ 0x0a, KEY_INFO},		/* OSD */
	{ 0x1c, KEY_BACKSPACE},		/* LAST */

	{ 0x0d, KEY_PLAY},		/* PLAY */
	{ 0x1e, KEY_CAMERA},		/* SNAPSHOT */
	{ 0x1a, KEY_RECORD},		/* RECORD */
	{ 0x17, KEY_STOP},		/* STOP */

	{ 0x1f, KEY_UP},		/* UP */
	{ 0x44, KEY_DOWN},		/* DOWN */
	{ 0x46, KEY_TAB},		/* BACK */
	{ 0x4a, KEY_ZOOM},		/* FULLSECREEN */

	{ 0x10, KEY_VOLUMEUP},		/* VOLUMEUP */
	{ 0x11, KEY_VOLUMEDOWN},	/* VOLUMEDOWN */
	{ 0x12, KEY_CHANNELUP},		/* CHANNELUP */
	{ 0x13, KEY_CHANNELDOWN},	/* CHANNELDOWN */
	{ 0x15, KEY_ENTER},		/* OK */
};

static struct rc_keymap gadmei_rm008z_map = {
	.map = {
		.scan    = gadmei_rm008z,
		.size    = ARRAY_SIZE(gadmei_rm008z),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_GADMEI_RM008Z,
	}
};

static int __init init_rc_map_gadmei_rm008z(void)
{
	return ir_register_map(&gadmei_rm008z_map);
}

static void __exit exit_rc_map_gadmei_rm008z(void)
{
	ir_unregister_map(&gadmei_rm008z_map);
}

module_init(init_rc_map_gadmei_rm008z)
module_exit(exit_rc_map_gadmei_rm008z)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
