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
 * Toshiba Bluetooth Enable Driver
 *
 * Copyright (C) 2009 Jes Sorensen <Jes.Sorensen@gmail.com>
 *
 * Thanks to Matthew Garrett for background info on ACPI innards which
 * normal people aren't meant to understand :-)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Note the Toshiba Bluetooth RFKill switch seems to be a strange
 * fish. It only provides a BT event when the switch is flipped to
 * the 'on' position. When flipping it to 'off', the USB device is
 * simply pulled away underneath us, without any BT event being
 * delivered.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>

MODULE_AUTHOR("Jes Sorensen <Jes.Sorensen@gmail.com>");
MODULE_DESCRIPTION("Toshiba Laptop ACPI Bluetooth Enable Driver");
MODULE_LICENSE("GPL");


static int toshiba_bt_rfkill_add(struct acpi_device *device);
static int toshiba_bt_rfkill_remove(struct acpi_device *device, int type);
static void toshiba_bt_rfkill_notify(struct acpi_device *device, u32 event);
static int toshiba_bt_resume(struct acpi_device *device);

static const struct acpi_device_id bt_device_ids[] = {
	{ "TOS6205", 0},
	{ "", 0},
};
MODULE_DEVICE_TABLE(acpi, bt_device_ids);

static struct acpi_driver toshiba_bt_rfkill_driver = {
	.name =		"Toshiba BT",
	.class =	"Toshiba",
	.ids =		bt_device_ids,
	.ops =		{
				.add =		toshiba_bt_rfkill_add,
				.remove =	toshiba_bt_rfkill_remove,
				.notify =	toshiba_bt_rfkill_notify,
				.resume =	toshiba_bt_resume,
			},
	.owner = 	THIS_MODULE,
};


static int toshiba_bluetooth_enable(acpi_handle handle)
{
	acpi_status res1, res2;
	u64 result;

	/*
	 * Query ACPI to verify RFKill switch is set to 'on'.
	 * If not, we return silently, no need to report it as
	 * an error.
	 */
	res1 = acpi_evaluate_integer(handle, "BTST", NULL, &result);
	if (ACPI_FAILURE(res1))
		return res1;
	if (!(result & 0x01))
		return 0;

	printk(KERN_INFO "toshiba_bluetooth: Re-enabling Toshiba Bluetooth\n");
	res1 = acpi_evaluate_object(handle, "AUSB", NULL, NULL);
	res2 = acpi_evaluate_object(handle, "BTPO", NULL, NULL);
	if (!ACPI_FAILURE(res1) || !ACPI_FAILURE(res2))
		return 0;

	printk(KERN_WARNING "toshiba_bluetooth: Failed to re-enable "
	       "Toshiba Bluetooth\n");

	return -ENODEV;
}

static void toshiba_bt_rfkill_notify(struct acpi_device *device, u32 event)
{
	toshiba_bluetooth_enable(device->handle);
}

static int toshiba_bt_resume(struct acpi_device *device)
{
	return toshiba_bluetooth_enable(device->handle);
}

static int toshiba_bt_rfkill_add(struct acpi_device *device)
{
	acpi_status status;
	u64 bt_present;
	int result = -ENODEV;

	/*
	 * Some Toshiba laptops may have a fake TOS6205 device in
	 * their ACPI BIOS, so query the _STA method to see if there
	 * is really anything there, before trying to enable it.
	 */
	status = acpi_evaluate_integer(device->handle, "_STA", NULL,
				       &bt_present);

	if (!ACPI_FAILURE(status) && bt_present) {
		printk(KERN_INFO "Detected Toshiba ACPI Bluetooth device - "
		      "installing RFKill handler\n");
		result = toshiba_bluetooth_enable(device->handle);
	}

	return result;
}

static int __init toshiba_bt_rfkill_init(void)
{
	int result;

	result = acpi_bus_register_driver(&toshiba_bt_rfkill_driver);
	if (result < 0) {
		ACPI_DEBUG_PRINT((ACPI_DB_ERROR,
				  "Error registering driver\n"));
		return result;
	}

	return 0;
}

static int toshiba_bt_rfkill_remove(struct acpi_device *device, int type)
{
	/* clean up */
	return 0;
}

static void __exit toshiba_bt_rfkill_exit(void)
{
	acpi_bus_unregister_driver(&toshiba_bt_rfkill_driver);
}

module_init(toshiba_bt_rfkill_init);
module_exit(toshiba_bt_rfkill_exit);
