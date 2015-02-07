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
 * Copyright (C) 2005-2006 Micronas USA Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/videodev2.h>

#include "wis-i2c.h"

struct wis_ov7640 {
	int brightness;
	int contrast;
	int saturation;
	int hue;
};

static u8 initial_registers[] =
{
	0x12, 0x80,
	0x12, 0x54,
	0x14, 0x24,
	0x15, 0x01,
	0x28, 0x20,
	0x75, 0x82,
	0xFF, 0xFF, /* Terminator (reg 0xFF is unused) */
};

static int write_regs(struct i2c_client *client, u8 *regs)
{
	int i;

	for (i = 0; regs[i] != 0xFF; i += 2)
		if (i2c_smbus_write_byte_data(client, regs[i], regs[i + 1]) < 0)
			return -1;
	return 0;
}

static int wis_ov7640_probe(struct i2c_client *client,
			    const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = client->adapter;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	client->flags = I2C_CLIENT_SCCB;

	printk(KERN_DEBUG
		"wis-ov7640: initializing OV7640 at address %d on %s\n",
		client->addr, adapter->name);

	if (write_regs(client, initial_registers) < 0) {
		printk(KERN_ERR "wis-ov7640: error initializing OV7640\n");
		return -ENODEV;
	}

	return 0;
}

static int wis_ov7640_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id wis_ov7640_id[] = {
	{ "wis_ov7640", 0 },
	{ }
};

static struct i2c_driver wis_ov7640_driver = {
	.driver = {
		.name	= "WIS OV7640 I2C driver",
	},
	.probe		= wis_ov7640_probe,
	.remove		= wis_ov7640_remove,
	.id_table	= wis_ov7640_id,
};

static int __init wis_ov7640_init(void)
{
	return i2c_add_driver(&wis_ov7640_driver);
}

static void __exit wis_ov7640_cleanup(void)
{
	i2c_del_driver(&wis_ov7640_driver);
}

module_init(wis_ov7640_init);
module_exit(wis_ov7640_cleanup);

MODULE_LICENSE("GPL v2");
