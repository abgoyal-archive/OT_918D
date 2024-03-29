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
 * Copyright (C) 2009
 * Guennadi Liakhovetski, DENX Software Engineering, <lg@denx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>

#include <mach/common.h>
#if defined(CONFIG_SPI_IMX) || defined(CONFIG_SPI_IMX_MODULE)
#include <mach/spi.h>
#endif
#include <mach/iomux-mx3.h>

#include <asm/mach-types.h>

#include "pcm037.h"
#include "devices.h"

static unsigned int pcm037_eet_pins[] = {
	/* Reserve and hardwire GPIO 57 high - S6E63D6 chipselect */
	IOMUX_MODE(MX31_PIN_KEY_COL7, IOMUX_CONFIG_GPIO),
	/* GPIO keys */
	IOMUX_MODE(MX31_PIN_GPIO1_0,	IOMUX_CONFIG_GPIO), /* 0 */
	IOMUX_MODE(MX31_PIN_GPIO1_1,	IOMUX_CONFIG_GPIO), /* 1 */
	IOMUX_MODE(MX31_PIN_GPIO1_2,	IOMUX_CONFIG_GPIO), /* 2 */
	IOMUX_MODE(MX31_PIN_GPIO1_3,	IOMUX_CONFIG_GPIO), /* 3 */
	IOMUX_MODE(MX31_PIN_SVEN0,	IOMUX_CONFIG_GPIO), /* 32 */
	IOMUX_MODE(MX31_PIN_STX0,	IOMUX_CONFIG_GPIO), /* 33 */
	IOMUX_MODE(MX31_PIN_SRX0,	IOMUX_CONFIG_GPIO), /* 34 */
	IOMUX_MODE(MX31_PIN_SIMPD0,	IOMUX_CONFIG_GPIO), /* 35 */
	IOMUX_MODE(MX31_PIN_RTS1,	IOMUX_CONFIG_GPIO), /* 38 */
	IOMUX_MODE(MX31_PIN_CTS1,	IOMUX_CONFIG_GPIO), /* 39 */
	IOMUX_MODE(MX31_PIN_KEY_ROW4,	IOMUX_CONFIG_GPIO), /* 50 */
	IOMUX_MODE(MX31_PIN_KEY_ROW5,	IOMUX_CONFIG_GPIO), /* 51 */
	IOMUX_MODE(MX31_PIN_KEY_ROW6,	IOMUX_CONFIG_GPIO), /* 52 */
	IOMUX_MODE(MX31_PIN_KEY_ROW7,	IOMUX_CONFIG_GPIO), /* 53 */

	/* LEDs */
	IOMUX_MODE(MX31_PIN_DTR_DTE1,	IOMUX_CONFIG_GPIO), /* 44 */
	IOMUX_MODE(MX31_PIN_DSR_DTE1,	IOMUX_CONFIG_GPIO), /* 45 */
	IOMUX_MODE(MX31_PIN_KEY_COL5,	IOMUX_CONFIG_GPIO), /* 55 */
	IOMUX_MODE(MX31_PIN_KEY_COL6,	IOMUX_CONFIG_GPIO), /* 56 */
};

/* SPI */
static struct spi_board_info pcm037_spi_dev[] = {
	{
		.modalias	= "dac124s085",
		.max_speed_hz	= 400000,
		.bus_num	= 0,
		.chip_select	= 0,		/* Index in pcm037_spi1_cs[] */
		.mode		= SPI_CPHA,
	},
};

/* Platform Data for MXC CSPI */
#if defined(CONFIG_SPI_IMX) || defined(CONFIG_SPI_IMX_MODULE)
static int pcm037_spi1_cs[] = {MXC_SPI_CS(1), IOMUX_TO_GPIO(MX31_PIN_KEY_COL7)};

struct spi_imx_master pcm037_spi1_master = {
	.chipselect = pcm037_spi1_cs,
	.num_chipselect = ARRAY_SIZE(pcm037_spi1_cs),
};
#endif

/* GPIO-keys input device */
static struct gpio_keys_button pcm037_gpio_keys[] = {
	{
		.type	= EV_KEY,
		.code	= KEY_L,
		.gpio	= 0,
		.desc	= "Wheel Manual",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_A,
		.gpio	= 1,
		.desc	= "Wheel AF",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_V,
		.gpio	= 2,
		.desc	= "Wheel View",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_M,
		.gpio	= 3,
		.desc	= "Wheel Menu",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_UP,
		.gpio	= 32,
		.desc	= "Nav Pad Up",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_RIGHT,
		.gpio	= 33,
		.desc	= "Nav Pad Right",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_DOWN,
		.gpio	= 34,
		.desc	= "Nav Pad Down",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_LEFT,
		.gpio	= 35,
		.desc	= "Nav Pad Left",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_ENTER,
		.gpio	= 38,
		.desc	= "Nav Pad Ok",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= KEY_O,
		.gpio	= 39,
		.desc	= "Wheel Off",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= BTN_FORWARD,
		.gpio	= 50,
		.desc	= "Focus Forward",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= BTN_BACK,
		.gpio	= 51,
		.desc	= "Focus Backward",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= BTN_MIDDLE,
		.gpio	= 52,
		.desc	= "Release Half",
		.wakeup	= 0,
	}, {
		.type	= EV_KEY,
		.code	= BTN_EXTRA,
		.gpio	= 53,
		.desc	= "Release Full",
		.wakeup	= 0,
	},
};

static struct gpio_keys_platform_data pcm037_gpio_keys_platform_data = {
	.buttons	= pcm037_gpio_keys,
	.nbuttons	= ARRAY_SIZE(pcm037_gpio_keys),
	.rep		= 0, /* No auto-repeat */
};

static struct platform_device pcm037_gpio_keys_device = {
	.name	= "gpio-keys",
	.id	= -1,
	.dev	= {
		.platform_data	= &pcm037_gpio_keys_platform_data,
	},
};

static int eet_init_devices(void)
{
	if (!machine_is_pcm037() || pcm037_variant() != PCM037_EET)
		return 0;

	mxc_iomux_setup_multiple_pins(pcm037_eet_pins,
				ARRAY_SIZE(pcm037_eet_pins), "pcm037_eet");

	/* SPI */
	spi_register_board_info(pcm037_spi_dev, ARRAY_SIZE(pcm037_spi_dev));
#if defined(CONFIG_SPI_IMX) || defined(CONFIG_SPI_IMX_MODULE)
	mxc_register_device(&mxc_spi_device0, &pcm037_spi1_master);
#endif

	platform_device_register(&pcm037_gpio_keys_device);

	return 0;
}

late_initcall(eet_init_devices);
