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

/* behold-columbus.h - Keytable for behold_columbus Remote Controller
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

/* Beholder Intl. Ltd. 2008
 * Dmitry Belimov d.belimov@google.com
 * Keytable is used by BeholdTV Columbus
 * The "ascii-art picture" below (in comments, first row
 * is the keycode in hex, and subsequent row(s) shows
 * the button labels (several variants when appropriate)
 * helps to descide which keycodes to assign to the buttons.
 */

static struct ir_scancode behold_columbus[] = {

	/*  0x13   0x11   0x1C   0x12  *
	 *  Mute  Source  TV/FM  Power *
	 *                             */

	{ 0x13, KEY_MUTE },
	{ 0x11, KEY_PROPS },
	{ 0x1C, KEY_TUNER },	/* KEY_TV/KEY_RADIO	*/
	{ 0x12, KEY_POWER },

	/*  0x01    0x02    0x03  0x0D    *
	 *   1       2       3   Stereo   *
	 *                        	  *
	 *  0x04    0x05    0x06  0x19    *
	 *   4       5       6   Snapshot *
	 *                        	  *
	 *  0x07    0x08    0x09  0x10    *
	 *   7       8       9    Zoom 	  *
	 *                                */
	{ 0x01, KEY_1 },
	{ 0x02, KEY_2 },
	{ 0x03, KEY_3 },
	{ 0x0D, KEY_SETUP },	  /* Setup key */
	{ 0x04, KEY_4 },
	{ 0x05, KEY_5 },
	{ 0x06, KEY_6 },
	{ 0x19, KEY_CAMERA },	/* Snapshot key */
	{ 0x07, KEY_7 },
	{ 0x08, KEY_8 },
	{ 0x09, KEY_9 },
	{ 0x10, KEY_ZOOM },

	/*  0x0A    0x00    0x0B       0x0C   *
	 * RECALL    0    ChannelUp  VolumeUp *
	 *                                    */
	{ 0x0A, KEY_AGAIN },
	{ 0x00, KEY_0 },
	{ 0x0B, KEY_CHANNELUP },
	{ 0x0C, KEY_VOLUMEUP },

	/*   0x1B      0x1D      0x15        0x18     *
	 * Timeshift  Record  ChannelDown  VolumeDown *
	 *                                            */

	{ 0x1B, KEY_TIME },
	{ 0x1D, KEY_RECORD },
	{ 0x15, KEY_CHANNELDOWN },
	{ 0x18, KEY_VOLUMEDOWN },

	/*   0x0E   0x1E     0x0F     0x1A  *
	 *   Stop   Pause  Previouse  Next  *
	 *                                  */

	{ 0x0E, KEY_STOP },
	{ 0x1E, KEY_PAUSE },
	{ 0x0F, KEY_PREVIOUS },
	{ 0x1A, KEY_NEXT },

};

static struct rc_keymap behold_columbus_map = {
	.map = {
		.scan    = behold_columbus,
		.size    = ARRAY_SIZE(behold_columbus),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_BEHOLD_COLUMBUS,
	}
};

static int __init init_rc_map_behold_columbus(void)
{
	return ir_register_map(&behold_columbus_map);
}

static void __exit exit_rc_map_behold_columbus(void)
{
	ir_unregister_map(&behold_columbus_map);
}

module_init(init_rc_map_behold_columbus)
module_exit(exit_rc_map_behold_columbus)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
