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

/* avermedia-dvbt.h - Keytable for avermedia_dvbt Remote Controller
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

/* Matt Jesson <dvb@jesson.eclipse.co.uk */

static struct ir_scancode avermedia_dvbt[] = {
	{ 0x28, KEY_0 },		/* '0' / 'enter' */
	{ 0x22, KEY_1 },		/* '1' */
	{ 0x12, KEY_2 },		/* '2' / 'up arrow' */
	{ 0x32, KEY_3 },		/* '3' */
	{ 0x24, KEY_4 },		/* '4' / 'left arrow' */
	{ 0x14, KEY_5 },		/* '5' */
	{ 0x34, KEY_6 },		/* '6' / 'right arrow' */
	{ 0x26, KEY_7 },		/* '7' */
	{ 0x16, KEY_8 },		/* '8' / 'down arrow' */
	{ 0x36, KEY_9 },		/* '9' */

	{ 0x20, KEY_LIST },		/* 'source' */
	{ 0x10, KEY_TEXT },		/* 'teletext' */
	{ 0x00, KEY_POWER },		/* 'power' */
	{ 0x04, KEY_AUDIO },		/* 'audio' */
	{ 0x06, KEY_ZOOM },		/* 'full screen' */
	{ 0x18, KEY_VIDEO },		/* 'display' */
	{ 0x38, KEY_SEARCH },		/* 'loop' */
	{ 0x08, KEY_INFO },		/* 'preview' */
	{ 0x2a, KEY_REWIND },		/* 'backward <<' */
	{ 0x1a, KEY_FASTFORWARD },	/* 'forward >>' */
	{ 0x3a, KEY_RECORD },		/* 'capture' */
	{ 0x0a, KEY_MUTE },		/* 'mute' */
	{ 0x2c, KEY_RECORD },		/* 'record' */
	{ 0x1c, KEY_PAUSE },		/* 'pause' */
	{ 0x3c, KEY_STOP },		/* 'stop' */
	{ 0x0c, KEY_PLAY },		/* 'play' */
	{ 0x2e, KEY_RED },		/* 'red' */
	{ 0x01, KEY_BLUE },		/* 'blue' / 'cancel' */
	{ 0x0e, KEY_YELLOW },		/* 'yellow' / 'ok' */
	{ 0x21, KEY_GREEN },		/* 'green' */
	{ 0x11, KEY_CHANNELDOWN },	/* 'channel -' */
	{ 0x31, KEY_CHANNELUP },	/* 'channel +' */
	{ 0x1e, KEY_VOLUMEDOWN },	/* 'volume -' */
	{ 0x3e, KEY_VOLUMEUP },		/* 'volume +' */
};

static struct rc_keymap avermedia_dvbt_map = {
	.map = {
		.scan    = avermedia_dvbt,
		.size    = ARRAY_SIZE(avermedia_dvbt),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_AVERMEDIA_DVBT,
	}
};

static int __init init_rc_map_avermedia_dvbt(void)
{
	return ir_register_map(&avermedia_dvbt_map);
}

static void __exit exit_rc_map_avermedia_dvbt(void)
{
	ir_unregister_map(&avermedia_dvbt_map);
}

module_init(init_rc_map_avermedia_dvbt)
module_exit(exit_rc_map_avermedia_dvbt)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
