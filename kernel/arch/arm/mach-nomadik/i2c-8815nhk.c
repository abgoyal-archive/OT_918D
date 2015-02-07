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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/i2c-gpio.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>

/*
 * There are two busses in the 8815NHK.
 * They could, in theory, be driven by the hardware component, but we
 * use bit-bang through GPIO by now, to keep things simple
 */

static struct i2c_gpio_platform_data nhk8815_i2c_data0 = {
	/* keep defaults for timeouts; pins are push-pull bidirectional */
	.scl_pin = 62,
	.sda_pin = 63,
};

static struct i2c_gpio_platform_data nhk8815_i2c_data1 = {
	/* keep defaults for timeouts; pins are push-pull bidirectional */
	.scl_pin = 53,
	.sda_pin = 54,
};

/* first bus: GPIO XX and YY */
static struct platform_device nhk8815_i2c_dev0 = {
	.name	= "i2c-gpio",
	.id	= 0,
	.dev	= {
		.platform_data = &nhk8815_i2c_data0,
	},
};
/* second bus: GPIO XX and YY */
static struct platform_device nhk8815_i2c_dev1 = {
	.name	= "i2c-gpio",
	.id	= 1,
	.dev	= {
		.platform_data = &nhk8815_i2c_data1,
	},
};

static int __init nhk8815_i2c_init(void)
{
	nmk_gpio_set_mode(nhk8815_i2c_data0.scl_pin, NMK_GPIO_ALT_GPIO);
	nmk_gpio_set_mode(nhk8815_i2c_data0.sda_pin, NMK_GPIO_ALT_GPIO);
	platform_device_register(&nhk8815_i2c_dev0);

	nmk_gpio_set_mode(nhk8815_i2c_data1.scl_pin, NMK_GPIO_ALT_GPIO);
	nmk_gpio_set_mode(nhk8815_i2c_data1.sda_pin, NMK_GPIO_ALT_GPIO);
	platform_device_register(&nhk8815_i2c_dev1);

	return 0;
}

static void __exit nhk8815_i2c_exit(void)
{
	platform_device_unregister(&nhk8815_i2c_dev0);
	platform_device_unregister(&nhk8815_i2c_dev1);
	return;
}

module_init(nhk8815_i2c_init);
module_exit(nhk8815_i2c_exit);
