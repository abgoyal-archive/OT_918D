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
 *  Force feedback support for PantherLord/GreenAsia based devices
 *
 *  The devices are distributed under various names and the same USB device ID
 *  can be used in both adapters and actual game controllers.
 *
 *  0810:0001 "Twin USB Joystick"
 *   - tested with PantherLord USB/PS2 2in1 Adapter
 *   - contains two reports, one for each port (HID_QUIRK_MULTI_INPUT)
 *
 *  0e8f:0003 "GreenAsia Inc.    USB Joystick     "
 *   - tested with K�nig Gaming gamepad
 *
 *  0e8f:0003 "GASIA USB Gamepad"
 *   - another version of the K�nig gamepad
 *
 *  Copyright (c) 2007, 2009 Anssi Hannula <anssi.hannula@gmail.com>
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


/* #define DEBUG */

#define debug(format, arg...) pr_debug("hid-plff: " format "\n" , ## arg)

#include <linux/input.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/hid.h>

#include "hid-ids.h"

#ifdef CONFIG_PANTHERLORD_FF
#include "usbhid/usbhid.h"

struct plff_device {
	struct hid_report *report;
	s32 *strong;
	s32 *weak;
};

static int hid_plff_play(struct input_dev *dev, void *data,
			 struct ff_effect *effect)
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct plff_device *plff = data;
	int left, right;

	left = effect->u.rumble.strong_magnitude;
	right = effect->u.rumble.weak_magnitude;
	debug("called with 0x%04x 0x%04x", left, right);

	left = left * 0x7f / 0xffff;
	right = right * 0x7f / 0xffff;

	*plff->strong = left;
	*plff->weak = right;
	debug("running with 0x%02x 0x%02x", left, right);
	usbhid_submit_report(hid, plff->report, USB_DIR_OUT);

	return 0;
}

static int plff_init(struct hid_device *hid)
{
	struct plff_device *plff;
	struct hid_report *report;
	struct hid_input *hidinput;
	struct list_head *report_list =
			&hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct list_head *report_ptr = report_list;
	struct input_dev *dev;
	int error;
	s32 *strong;
	s32 *weak;

	/* The device contains one output report per physical device, all
	   containing 1 field, which contains 4 ff00.0002 usages and 4 16bit
	   absolute values.

	   The input reports also contain a field which contains
	   8 ff00.0001 usages and 8 boolean values. Their meaning is
	   currently unknown.
	   
	   A version of the 0e8f:0003 exists that has all the values in
	   separate fields and misses the extra input field, thus resembling
	   Zeroplus (hid-zpff) devices.
	*/

	if (list_empty(report_list)) {
		dev_err(&hid->dev, "no output reports found\n");
		return -ENODEV;
	}

	list_for_each_entry(hidinput, &hid->inputs, list) {

		report_ptr = report_ptr->next;

		if (report_ptr == report_list) {
			dev_err(&hid->dev, "required output report is "
					"missing\n");
			return -ENODEV;
		}

		report = list_entry(report_ptr, struct hid_report, list);
		if (report->maxfield < 1) {
			dev_err(&hid->dev, "no fields in the report\n");
			return -ENODEV;
		}

		if (report->field[0]->report_count >= 4) {
			report->field[0]->value[0] = 0x00;
			report->field[0]->value[1] = 0x00;
			strong = &report->field[0]->value[2];
			weak = &report->field[0]->value[3];
			debug("detected single-field device");
		} else if (report->maxfield >= 4 && report->field[0]->maxusage == 1 &&
				report->field[0]->usage[0].hid == (HID_UP_LED | 0x43)) {
			report->field[0]->value[0] = 0x00;
			report->field[1]->value[0] = 0x00;
			strong = &report->field[2]->value[0];
			weak = &report->field[3]->value[0];
			debug("detected 4-field device");
		} else {
			dev_err(&hid->dev, "not enough fields or values\n");
			return -ENODEV;
		}

		plff = kzalloc(sizeof(struct plff_device), GFP_KERNEL);
		if (!plff)
			return -ENOMEM;

		dev = hidinput->input;

		set_bit(FF_RUMBLE, dev->ffbit);

		error = input_ff_create_memless(dev, plff, hid_plff_play);
		if (error) {
			kfree(plff);
			return error;
		}

		plff->report = report;
		plff->strong = strong;
		plff->weak = weak;

		*strong = 0x00;
		*weak = 0x00;
		usbhid_submit_report(hid, plff->report, USB_DIR_OUT);
	}

	dev_info(&hid->dev, "Force feedback for PantherLord/GreenAsia "
	       "devices by Anssi Hannula <anssi.hannula@gmail.com>\n");

	return 0;
}
#else
static inline int plff_init(struct hid_device *hid)
{
	return 0;
}
#endif

static int pl_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	int ret;

	if (id->driver_data)
		hdev->quirks |= HID_QUIRK_MULTI_INPUT;

	ret = hid_parse(hdev);
	if (ret) {
		dev_err(&hdev->dev, "parse failed\n");
		goto err;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT & ~HID_CONNECT_FF);
	if (ret) {
		dev_err(&hdev->dev, "hw start failed\n");
		goto err;
	}

	plff_init(hdev);

	return 0;
err:
	return ret;
}

static const struct hid_device_id pl_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_GAMERON, USB_DEVICE_ID_GAMERON_DUAL_PSX_ADAPTOR),
		.driver_data = 1 }, /* Twin USB Joystick */
	{ HID_USB_DEVICE(USB_VENDOR_ID_GAMERON, USB_DEVICE_ID_GAMERON_DUAL_PCS_ADAPTOR),
		.driver_data = 1 }, /* Twin USB Joystick */
	{ HID_USB_DEVICE(USB_VENDOR_ID_GREENASIA, 0x0003), },
	{ }
};
MODULE_DEVICE_TABLE(hid, pl_devices);

static struct hid_driver pl_driver = {
	.name = "pantherlord",
	.id_table = pl_devices,
	.probe = pl_probe,
};

static int __init pl_init(void)
{
	return hid_register_driver(&pl_driver);
}

static void __exit pl_exit(void)
{
	hid_unregister_driver(&pl_driver);
}

module_init(pl_init);
module_exit(pl_exit);
MODULE_LICENSE("GPL");
