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

/* tbs-nec.h - Keytable for tbs_nec Remote Controller
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

static struct ir_scancode tbs_nec[] = {
	{ 0x04, KEY_POWER2},	/*power*/
	{ 0x14, KEY_MUTE},	/*mute*/
	{ 0x07, KEY_1},
	{ 0x06, KEY_2},
	{ 0x05, KEY_3},
	{ 0x0b, KEY_4},
	{ 0x0a, KEY_5},
	{ 0x09, KEY_6},
	{ 0x0f, KEY_7},
	{ 0x0e, KEY_8},
	{ 0x0d, KEY_9},
	{ 0x12, KEY_0},
	{ 0x16, KEY_CHANNELUP},	/*ch+*/
	{ 0x11, KEY_CHANNELDOWN},/*ch-*/
	{ 0x13, KEY_VOLUMEUP},	/*vol+*/
	{ 0x0c, KEY_VOLUMEDOWN},/*vol-*/
	{ 0x03, KEY_RECORD},	/*rec*/
	{ 0x18, KEY_PAUSE},	/*pause*/
	{ 0x19, KEY_OK},	/*ok*/
	{ 0x1a, KEY_CAMERA},	/* snapshot */
	{ 0x01, KEY_UP},
	{ 0x10, KEY_LEFT},
	{ 0x02, KEY_RIGHT},
	{ 0x08, KEY_DOWN},
	{ 0x15, KEY_FAVORITES},
	{ 0x17, KEY_SUBTITLE},
	{ 0x1d, KEY_ZOOM},
	{ 0x1f, KEY_EXIT},
	{ 0x1e, KEY_MENU},
	{ 0x1c, KEY_EPG},
	{ 0x00, KEY_PREVIOUS},
	{ 0x1b, KEY_MODE},
};

static struct rc_keymap tbs_nec_map = {
	.map = {
		.scan    = tbs_nec,
		.size    = ARRAY_SIZE(tbs_nec),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_TBS_NEC,
	}
};

static int __init init_rc_map_tbs_nec(void)
{
	return ir_register_map(&tbs_nec_map);
}

static void __exit exit_rc_map_tbs_nec(void)
{
	ir_unregister_map(&tbs_nec_map);
}

module_init(init_rc_map_tbs_nec)
module_exit(exit_rc_map_tbs_nec)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
