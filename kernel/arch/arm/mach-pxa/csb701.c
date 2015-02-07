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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <linux/leds.h>

#include <asm/mach-types.h>

static struct gpio_keys_button csb701_buttons[] = {
	{
		.code	= 0x7,
		.gpio	= 1,
		.active_low = 1,
		.desc	= "SW2",
		.type	= EV_SW,
		.wakeup = 1,
	},
};

static struct gpio_keys_platform_data csb701_gpio_keys_data = {
	.buttons = csb701_buttons,
	.nbuttons = ARRAY_SIZE(csb701_buttons),
};

static struct gpio_led csb701_leds[] = {
	{
		.name	= "csb701:yellow:heartbeat",
		.default_trigger = "heartbeat",
		.gpio	= 11,
		.active_low = 1,
	},
};

static struct platform_device csb701_gpio_keys = {
	.name		= "gpio-keys",
	.id		= -1,
	.dev.platform_data = &csb701_gpio_keys_data,
};

static struct gpio_led_platform_data csb701_leds_gpio_data = {
	.leds		= csb701_leds,
	.num_leds	= ARRAY_SIZE(csb701_leds),
};

static struct platform_device csb701_leds_gpio = {
	.name		= "leds-gpio",
	.id		= -1,
	.dev.platform_data = &csb701_leds_gpio_data,
};

static struct platform_device *devices[] __initdata = {
	&csb701_gpio_keys,
	&csb701_leds_gpio,
};

static int __init csb701_init(void)
{
	if (!machine_is_csb726())
		return -ENODEV;

	return platform_add_devices(devices, ARRAY_SIZE(devices));
}

module_init(csb701_init);

