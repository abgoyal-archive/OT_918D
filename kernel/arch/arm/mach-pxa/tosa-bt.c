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
 * Bluetooth built-in chip control
 *
 * Copyright (c) 2008 Dmitry Baryshkov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/rfkill.h>

#include <mach/tosa_bt.h>

static void tosa_bt_on(struct tosa_bt_data *data)
{
	gpio_set_value(data->gpio_reset, 0);
	gpio_set_value(data->gpio_pwr, 1);
	gpio_set_value(data->gpio_reset, 1);
	mdelay(20);
	gpio_set_value(data->gpio_reset, 0);
}

static void tosa_bt_off(struct tosa_bt_data *data)
{
	gpio_set_value(data->gpio_reset, 1);
	mdelay(10);
	gpio_set_value(data->gpio_pwr, 0);
	gpio_set_value(data->gpio_reset, 0);
}

static int tosa_bt_set_block(void *data, bool blocked)
{
	pr_info("BT_RADIO going: %s\n", blocked ? "off" : "on");

	if (!blocked) {
		pr_info("TOSA_BT: going ON\n");
		tosa_bt_on(data);
	} else {
		pr_info("TOSA_BT: going OFF\n");
		tosa_bt_off(data);
	}

	return 0;
}

static const struct rfkill_ops tosa_bt_rfkill_ops = {
	.set_block = tosa_bt_set_block,
};

static int tosa_bt_probe(struct platform_device *dev)
{
	int rc;
	struct rfkill *rfk;

	struct tosa_bt_data *data = dev->dev.platform_data;

	rc = gpio_request(data->gpio_reset, "Bluetooth reset");
	if (rc)
		goto err_reset;
	rc = gpio_direction_output(data->gpio_reset, 0);
	if (rc)
		goto err_reset_dir;
	rc = gpio_request(data->gpio_pwr, "Bluetooth power");
	if (rc)
		goto err_pwr;
	rc = gpio_direction_output(data->gpio_pwr, 0);
	if (rc)
		goto err_pwr_dir;

	rfk = rfkill_alloc("tosa-bt", &dev->dev, RFKILL_TYPE_BLUETOOTH,
			   &tosa_bt_rfkill_ops, data);
	if (!rfk) {
		rc = -ENOMEM;
		goto err_rfk_alloc;
	}

	rfkill_set_led_trigger_name(rfk, "tosa-bt");

	rc = rfkill_register(rfk);
	if (rc)
		goto err_rfkill;

	platform_set_drvdata(dev, rfk);

	return 0;

err_rfkill:
	rfkill_destroy(rfk);
err_rfk_alloc:
	tosa_bt_off(data);
err_pwr_dir:
	gpio_free(data->gpio_pwr);
err_pwr:
err_reset_dir:
	gpio_free(data->gpio_reset);
err_reset:
	return rc;
}

static int __devexit tosa_bt_remove(struct platform_device *dev)
{
	struct tosa_bt_data *data = dev->dev.platform_data;
	struct rfkill *rfk = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

	if (rfk) {
		rfkill_unregister(rfk);
		rfkill_destroy(rfk);
	}
	rfk = NULL;

	tosa_bt_off(data);

	gpio_free(data->gpio_pwr);
	gpio_free(data->gpio_reset);

	return 0;
}

static struct platform_driver tosa_bt_driver = {
	.probe = tosa_bt_probe,
	.remove = __devexit_p(tosa_bt_remove),

	.driver = {
		.name = "tosa-bt",
		.owner = THIS_MODULE,
	},
};


static int __init tosa_bt_init(void)
{
	return platform_driver_register(&tosa_bt_driver);
}

static void __exit tosa_bt_exit(void)
{
	platform_driver_unregister(&tosa_bt_driver);
}

module_init(tosa_bt_init);
module_exit(tosa_bt_exit);
