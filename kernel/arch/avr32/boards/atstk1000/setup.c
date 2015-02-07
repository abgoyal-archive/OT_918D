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
 * ATSTK1000 board-specific setup code.
 *
 * Copyright (C) 2005-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/bootmem.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/linkage.h>

#include <video/atmel_lcdc.h>

#include <asm/setup.h>

#include <mach/at32ap700x.h>
#include <mach/board.h>
#include <mach/portmux.h>

#include "atstk1000.h"

/* Initialized by bootloader-specific startup code. */
struct tag *bootloader_tags __initdata;

static struct fb_videomode __initdata ltv350qv_modes[] = {
	{
		.name		= "320x240 @ 75",
		.refresh	= 75,
		.xres		= 320,		.yres		= 240,
		.pixclock	= KHZ2PICOS(6891),

		.left_margin	= 17,		.right_margin	= 33,
		.upper_margin	= 10,		.lower_margin	= 10,
		.hsync_len	= 16,		.vsync_len	= 1,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
};

static struct fb_monspecs __initdata atstk1000_default_monspecs = {
	.manufacturer		= "SNG",
	.monitor		= "LTV350QV",
	.modedb			= ltv350qv_modes,
	.modedb_len		= ARRAY_SIZE(ltv350qv_modes),
	.hfmin			= 14820,
	.hfmax			= 22230,
	.vfmin			= 60,
	.vfmax			= 90,
	.dclkmax		= 30000000,
};

struct atmel_lcdfb_info __initdata atstk1000_lcdc_data = {
	.default_bpp		= 24,
	.default_dmacon		= ATMEL_LCDC_DMAEN | ATMEL_LCDC_DMA2DEN,
	.default_lcdcon2	= (ATMEL_LCDC_DISTYPE_TFT
				   | ATMEL_LCDC_INVCLK
				   | ATMEL_LCDC_CLKMOD_ALWAYSACTIVE
				   | ATMEL_LCDC_MEMOR_BIG),
	.default_monspecs	= &atstk1000_default_monspecs,
	.guard_time		= 2,
};

#ifdef CONFIG_BOARD_ATSTK1000_J2_LED
#include <linux/leds.h>

static struct gpio_led stk1000_j2_led[] = {
#ifdef CONFIG_BOARD_ATSTK1000_J2_LED8
#define LEDSTRING "J2 jumpered to LED8"
	{ .name = "led0:amber", .gpio = GPIO_PIN_PB( 8), },
	{ .name = "led1:amber", .gpio = GPIO_PIN_PB( 9), },
	{ .name = "led2:amber", .gpio = GPIO_PIN_PB(10), },
	{ .name = "led3:amber", .gpio = GPIO_PIN_PB(13), },
	{ .name = "led4:amber", .gpio = GPIO_PIN_PB(14), },
	{ .name = "led5:amber", .gpio = GPIO_PIN_PB(15), },
	{ .name = "led6:amber", .gpio = GPIO_PIN_PB(16), },
	{ .name = "led7:amber", .gpio = GPIO_PIN_PB(30),
			.default_trigger = "heartbeat", },
#else	/* RGB */
#define LEDSTRING "J2 jumpered to RGB LEDs"
	{ .name = "r1:red",     .gpio = GPIO_PIN_PB( 8), },
	{ .name = "g1:green",   .gpio = GPIO_PIN_PB(10), },
	{ .name = "b1:blue",    .gpio = GPIO_PIN_PB(14), },

	{ .name = "r2:red",     .gpio = GPIO_PIN_PB( 9),
			.default_trigger = "heartbeat", },
	{ .name = "g2:green",   .gpio = GPIO_PIN_PB(13), },
	{ .name = "b2:blue",    .gpio = GPIO_PIN_PB(15),
			.default_trigger = "heartbeat", },
	/* PB16, PB30 unused */
#endif
};

static struct gpio_led_platform_data stk1000_j2_led_data = {
	.num_leds	= ARRAY_SIZE(stk1000_j2_led),
	.leds		= stk1000_j2_led,
};

static struct platform_device stk1000_j2_led_dev = {
	.name		= "leds-gpio",
	.id		= 2,	/* gpio block J2 */
	.dev		= {
		.platform_data	= &stk1000_j2_led_data,
	},
};

void __init atstk1000_setup_j2_leds(void)
{
	unsigned	i;

	for (i = 0; i < ARRAY_SIZE(stk1000_j2_led); i++)
		at32_select_gpio(stk1000_j2_led[i].gpio, AT32_GPIOF_OUTPUT);

	printk("STK1000: " LEDSTRING "\n");
	platform_device_register(&stk1000_j2_led_dev);
}
#else /* CONFIG_BOARD_ATSTK1000_J2_LED */
void __init atstk1000_setup_j2_leds(void)
{

}
#endif /* CONFIG_BOARD_ATSTK1000_J2_LED */