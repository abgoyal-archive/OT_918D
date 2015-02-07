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

/* ati-tv-wonder-hd-600.h - Keytable for ati_tv_wonder_hd_600 Remote Controller
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

/* ATI TV Wonder HD 600 USB
   Devin Heitmueller <devin.heitmueller@gmail.com>
 */

static struct ir_scancode ati_tv_wonder_hd_600[] = {
	{ 0x00, KEY_RECORD},		/* Row 1 */
	{ 0x01, KEY_PLAYPAUSE},
	{ 0x02, KEY_STOP},
	{ 0x03, KEY_POWER},
	{ 0x04, KEY_PREVIOUS},	/* Row 2 */
	{ 0x05, KEY_REWIND},
	{ 0x06, KEY_FORWARD},
	{ 0x07, KEY_NEXT},
	{ 0x08, KEY_EPG},		/* Row 3 */
	{ 0x09, KEY_HOME},
	{ 0x0a, KEY_MENU},
	{ 0x0b, KEY_CHANNELUP},
	{ 0x0c, KEY_BACK},		/* Row 4 */
	{ 0x0d, KEY_UP},
	{ 0x0e, KEY_INFO},
	{ 0x0f, KEY_CHANNELDOWN},
	{ 0x10, KEY_LEFT},		/* Row 5 */
	{ 0x11, KEY_SELECT},
	{ 0x12, KEY_RIGHT},
	{ 0x13, KEY_VOLUMEUP},
	{ 0x14, KEY_LAST},		/* Row 6 */
	{ 0x15, KEY_DOWN},
	{ 0x16, KEY_MUTE},
	{ 0x17, KEY_VOLUMEDOWN},
};

static struct rc_keymap ati_tv_wonder_hd_600_map = {
	.map = {
		.scan    = ati_tv_wonder_hd_600,
		.size    = ARRAY_SIZE(ati_tv_wonder_hd_600),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_ATI_TV_WONDER_HD_600,
	}
};

static int __init init_rc_map_ati_tv_wonder_hd_600(void)
{
	return ir_register_map(&ati_tv_wonder_hd_600_map);
}

static void __exit exit_rc_map_ati_tv_wonder_hd_600(void)
{
	ir_unregister_map(&ati_tv_wonder_hd_600_map);
}

module_init(init_rc_map_ati_tv_wonder_hd_600)
module_exit(exit_rc_map_ati_tv_wonder_hd_600)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
