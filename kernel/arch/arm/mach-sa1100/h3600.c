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
 * Support for Compaq iPAQ H3600 handheld computer
 *
 * Copyright (c) 2000,1 Compaq Computer Corporation. (Author: Jamey Hicks)
 * Copyright (c) 2009 Dmitry Artamonow <mad_soft@inbox.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/irda.h>

#include <mach/h3xxx.h>

#include "generic.h"

/*
 * helper for sa1100fb
 */
static void h3600_lcd_power(int enable)
{
	if (gpio_request(H3XXX_EGPIO_LCD_ON, "LCD power")) {
		pr_err("%s: can't request H3XXX_EGPIO_LCD_ON\n", __func__);
		goto err1;
	}
	if (gpio_request(H3600_EGPIO_LCD_PCI, "LCD control")) {
		pr_err("%s: can't request H3XXX_EGPIO_LCD_PCI\n", __func__);
		goto err2;
	}
	if (gpio_request(H3600_EGPIO_LCD_5V_ON, "LCD 5v")) {
		pr_err("%s: can't request H3XXX_EGPIO_LCD_5V_ON\n", __func__);
		goto err3;
	}
	if (gpio_request(H3600_EGPIO_LVDD_ON, "LCD 9v/-6.5v")) {
		pr_err("%s: can't request H3600_EGPIO_LVDD_ON\n", __func__);
		goto err4;
	}

	gpio_direction_output(H3XXX_EGPIO_LCD_ON, enable);
	gpio_direction_output(H3600_EGPIO_LCD_PCI, enable);
	gpio_direction_output(H3600_EGPIO_LCD_5V_ON, enable);
	gpio_direction_output(H3600_EGPIO_LVDD_ON, enable);

	gpio_free(H3600_EGPIO_LVDD_ON);
err4:	gpio_free(H3600_EGPIO_LCD_5V_ON);
err3:	gpio_free(H3600_EGPIO_LCD_PCI);
err2:	gpio_free(H3XXX_EGPIO_LCD_ON);
err1:	return;
}

static void __init h3600_map_io(void)
{
	h3xxx_map_io();

	sa1100fb_lcd_power = h3600_lcd_power;
}

/*
 * This turns the IRDA power on or off on the Compaq H3600
 */
static int h3600_irda_set_power(struct device *dev, unsigned int state)
{
	gpio_set_value(H3600_EGPIO_IR_ON, state);
	return 0;
}

static void h3600_irda_set_speed(struct device *dev, unsigned int speed)
{
	gpio_set_value(H3600_EGPIO_IR_FSEL, !(speed < 4000000));
}

static int h3600_irda_startup(struct device *dev)
{
	int err = gpio_request(H3600_EGPIO_IR_ON, "IrDA power");
	if (err)
		goto err1;
	err = gpio_direction_output(H3600_EGPIO_IR_ON, 0);
	if (err)
		goto err2;
	err = gpio_request(H3600_EGPIO_IR_FSEL, "IrDA fsel");
	if (err)
		goto err2;
	err = gpio_direction_output(H3600_EGPIO_IR_FSEL, 0);
	if (err)
		goto err3;
	return 0;

err3:	gpio_free(H3600_EGPIO_IR_FSEL);
err2:	gpio_free(H3600_EGPIO_IR_ON);
err1:	return err;
}

static void h3600_irda_shutdown(struct device *dev)
{
	gpio_free(H3600_EGPIO_IR_ON);
	gpio_free(H3600_EGPIO_IR_FSEL);
}

static struct irda_platform_data h3600_irda_data = {
	.set_power	= h3600_irda_set_power,
	.set_speed	= h3600_irda_set_speed,
	.startup	= h3600_irda_startup,
	.shutdown	= h3600_irda_shutdown,
};

static struct gpio_default_state h3600_default_gpio[] = {
	{ H3XXX_GPIO_COM_DCD,	GPIO_MODE_IN,	"COM DCD" },
	{ H3XXX_GPIO_COM_CTS,	GPIO_MODE_IN,	"COM CTS" },
	{ H3XXX_GPIO_COM_RTS,	GPIO_MODE_OUT0,	"COM RTS" },
};

static void __init h3600_mach_init(void)
{
	h3xxx_init_gpio(h3600_default_gpio, ARRAY_SIZE(h3600_default_gpio));
	h3xxx_mach_init();
	sa11x0_register_irda(&h3600_irda_data);
}

MACHINE_START(H3600, "Compaq iPAQ H3600")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf8000000) >> 18) & 0xfffc,
	.boot_params	= 0xc0000100,
	.map_io		= h3600_map_io,
	.init_irq	= sa1100_init_irq,
	.timer		= &sa1100_timer,
	.init_machine	= h3600_mach_init,
MACHINE_END

