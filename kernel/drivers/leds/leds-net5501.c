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
 * Soekris board support code
 *
 * Copyright (C) 2008-2009 Tower Technologies
 * Written by Alessandro Zummo <a.zummo@towertech.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/string.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>

#include <asm/geode.h>

static struct gpio_led net5501_leds[] = {
	{
		.name = "error",
		.gpio = 6,
		.default_trigger = "default-on",
	},
};

static struct gpio_led_platform_data net5501_leds_data = {
	.num_leds = ARRAY_SIZE(net5501_leds),
	.leds = net5501_leds,
};

static struct platform_device net5501_leds_dev = {
	.name = "leds-gpio",
	.id = -1,
	.dev.platform_data = &net5501_leds_data,
};

static void __init init_net5501(void)
{
	platform_device_register(&net5501_leds_dev);
}

struct soekris_board {
	u16	offset;
	char	*sig;
	u8	len;
	void	(*init)(void);
};

static struct soekris_board __initdata boards[] = {
	{ 0xb7b, "net5501", 7, init_net5501 },	/* net5501 v1.33/1.33c */
	{ 0xb1f, "net5501", 7, init_net5501 },	/* net5501 v1.32i */
};

static int __init soekris_init(void)
{
	int i;
	unsigned char *rombase, *bios;

	if (!is_geode())
		return 0;

	rombase = ioremap(0xffff0000, 0xffff);
	if (!rombase) {
		printk(KERN_INFO "Soekris net5501 LED driver failed to get rombase");
		return 0;
	}

	bios = rombase + 0x20;	/* null terminated */

	if (strncmp(bios, "comBIOS", 7))
		goto unmap;

	for (i = 0; i < ARRAY_SIZE(boards); i++) {
		unsigned char *model = rombase + boards[i].offset;

		if (strncmp(model, boards[i].sig, boards[i].len) == 0) {
			printk(KERN_INFO "Soekris %s: %s\n", model, bios);

			if (boards[i].init)
				boards[i].init();
			break;
		}
	}

unmap:
	iounmap(rombase);
	return 0;
}

arch_initcall(soekris_init);
