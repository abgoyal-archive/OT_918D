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

/*
 * HID driver for TwinHan IR remote control
 *
 * Based on hid-gyration.c
 *
 * Copyright (c) 2009 Bruno Prémont <bonbons@linux-vserver.org>
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License.
 */

#include <linux/device.h>
#include <linux/input.h>
#include <linux/hid.h>
#include <linux/module.h>

#include "hid-ids.h"

/*	Remote control key layout + listing:
 *
 * 	Full Screen                              Power
 *	KEY_SCREEN                          KEY_POWER2
 *
 *	1                     2                      3
 *	KEY_NUMERIC_1   KEY_NUMERIC_2    KEY_NUMERIC_3
 *
 *	4                     5                      6
 *	KEY_NUMERIC_4   KEY_NUMERIC_5    KEY_NUMERIC_6
 *
 *	7                     8                      9
 *	KEY_NUMERIC_7   KEY_NUMERIC_8    KEY_NUMERIC_9
 *
 *	REC                   0               Favorite
 *	KEY_RECORD      KEY_NUMERIC_0    KEY_FAVORITES
 *
 *	Rewind                                 Forward
 *	KEY_REWIND           CH+           KEY_FORWARD
 *	               KEY_CHANNELUP
 *
 *	VOL-                  >                   VOL+
 *	KEY_VOLUMEDOWN    KEY_PLAY        KEY_VOLUMEUP
 *
 *	                     CH-
 *	              KEY_CHANNELDOWN
 *	Recall                                    Stop
 *	KEY_RESTART                           KEY_STOP
 *
 *	Timeshift/Pause     Mute                Cancel
 *	KEY_PAUSE         KEY_MUTE          KEY_CANCEL
 *
 *	Capture            Preview                 EPG
 *	KEY_PRINT        KEY_PROGRAM           KEY_EPG
 *
 *	Record List          Tab              Teletext
 *	KEY_LIST            KEY_TAB           KEY_TEXT
 */

#define th_map_key_clear(c)	hid_map_usage_clear(hi, usage, bit, max, \
					EV_KEY, (c))
static int twinhan_input_mapping(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	if ((usage->hid & HID_USAGE_PAGE) != HID_UP_KEYBOARD)
		return 0;

	switch (usage->hid & HID_USAGE) {
	/* Map all keys from Twinhan Remote */
	case 0x004: th_map_key_clear(KEY_TEXT);         break;
	case 0x006: th_map_key_clear(KEY_RESTART);      break;
	case 0x008: th_map_key_clear(KEY_EPG);          break;
	case 0x00c: th_map_key_clear(KEY_REWIND);       break;
	case 0x00e: th_map_key_clear(KEY_PROGRAM);      break;
	case 0x00f: th_map_key_clear(KEY_LIST);         break;
	case 0x010: th_map_key_clear(KEY_MUTE);         break;
	case 0x011: th_map_key_clear(KEY_FORWARD);      break;
	case 0x013: th_map_key_clear(KEY_PRINT);        break;
	case 0x017: th_map_key_clear(KEY_PAUSE);        break;
	case 0x019: th_map_key_clear(KEY_FAVORITES);    break;
	case 0x01d: th_map_key_clear(KEY_SCREEN);       break;
	case 0x01e: th_map_key_clear(KEY_NUMERIC_1);    break;
	case 0x01f: th_map_key_clear(KEY_NUMERIC_2);    break;
	case 0x020: th_map_key_clear(KEY_NUMERIC_3);    break;
	case 0x021: th_map_key_clear(KEY_NUMERIC_4);    break;
	case 0x022: th_map_key_clear(KEY_NUMERIC_5);    break;
	case 0x023: th_map_key_clear(KEY_NUMERIC_6);    break;
	case 0x024: th_map_key_clear(KEY_NUMERIC_7);    break;
	case 0x025: th_map_key_clear(KEY_NUMERIC_8);    break;
	case 0x026: th_map_key_clear(KEY_NUMERIC_9);    break;
	case 0x027: th_map_key_clear(KEY_NUMERIC_0);    break;
	case 0x028: th_map_key_clear(KEY_PLAY);         break;
	case 0x029: th_map_key_clear(KEY_CANCEL);       break;
	case 0x02b: th_map_key_clear(KEY_TAB);          break;
	/* Power       = 0x0e0 + 0x0e1 + 0x0e2 + 0x03f */
	case 0x03f: th_map_key_clear(KEY_POWER2);       break;
	case 0x04a: th_map_key_clear(KEY_RECORD);       break;
	case 0x04b: th_map_key_clear(KEY_CHANNELUP);    break;
	case 0x04d: th_map_key_clear(KEY_STOP);         break;
	case 0x04e: th_map_key_clear(KEY_CHANNELDOWN);  break;
	/* Volume down = 0x0e1 + 0x051                 */
	case 0x051: th_map_key_clear(KEY_VOLUMEDOWN);   break;
	/* Volume up   = 0x0e1 + 0x052                 */
	case 0x052: th_map_key_clear(KEY_VOLUMEUP);     break;
	/* Kill the extra keys used for multi-key "power" and "volume" keys
	 * as well as continuously to release CTRL,ALT,META,... keys */
	case 0x0e0:
	case 0x0e1:
	case 0x0e2:
	case 0x0e3:
	case 0x0e4:
	case 0x0e5:
	case 0x0e6:
	case 0x0e7:
	default:
		return -1;
	}
	return 1;
}

static const struct hid_device_id twinhan_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_TWINHAN, USB_DEVICE_ID_TWINHAN_IR_REMOTE) },
	{ }
};
MODULE_DEVICE_TABLE(hid, twinhan_devices);

static struct hid_driver twinhan_driver = {
	.name = "twinhan",
	.id_table = twinhan_devices,
	.input_mapping = twinhan_input_mapping,
};

static int __init twinhan_init(void)
{
	return hid_register_driver(&twinhan_driver);
}

static void __exit twinhan_exit(void)
{
	hid_unregister_driver(&twinhan_driver);
}

module_init(twinhan_init);
module_exit(twinhan_exit);
MODULE_LICENSE("GPL");
