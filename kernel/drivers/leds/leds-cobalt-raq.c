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
 *  LEDs driver for the Cobalt Raq series.
 *
 *  Copyright (C) 2007  Yoichi Yuasa <yuasa@linux-mips.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/types.h>

#define LED_WEB		0x04
#define LED_POWER_OFF	0x08

static void __iomem *led_port;
static u8 led_value;
static DEFINE_SPINLOCK(led_value_lock);

static void raq_web_led_set(struct led_classdev *led_cdev,
			    enum led_brightness brightness)
{
	unsigned long flags;

	spin_lock_irqsave(&led_value_lock, flags);

	if (brightness)
		led_value |= LED_WEB;
	else
		led_value &= ~LED_WEB;
	writeb(led_value, led_port);

	spin_unlock_irqrestore(&led_value_lock, flags);
}

static struct led_classdev raq_web_led = {
	.name		= "raq::web",
	.brightness_set	= raq_web_led_set,
};

static void raq_power_off_led_set(struct led_classdev *led_cdev,
				  enum led_brightness brightness)
{
	unsigned long flags;

	spin_lock_irqsave(&led_value_lock, flags);

	if (brightness)
		led_value |= LED_POWER_OFF;
	else
		led_value &= ~LED_POWER_OFF;
	writeb(led_value, led_port);

	spin_unlock_irqrestore(&led_value_lock, flags);
}

static struct led_classdev raq_power_off_led = {
	.name			= "raq::power-off",
	.brightness_set		= raq_power_off_led_set,
	.default_trigger	= "power-off",
};

static int __devinit cobalt_raq_led_probe(struct platform_device *pdev)
{
	struct resource *res;
	int retval;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -EBUSY;

	led_port = ioremap(res->start, resource_size(res));
	if (!led_port)
		return -ENOMEM;

	retval = led_classdev_register(&pdev->dev, &raq_power_off_led);
	if (retval)
		goto err_iounmap;

	retval = led_classdev_register(&pdev->dev, &raq_web_led);
	if (retval)
		goto err_unregister;

	return 0;

err_unregister:
	led_classdev_unregister(&raq_power_off_led);

err_iounmap:
	iounmap(led_port);
	led_port = NULL;

	return retval;
}

static int __devexit cobalt_raq_led_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&raq_power_off_led);
	led_classdev_unregister(&raq_web_led);

	if (led_port) {
		iounmap(led_port);
		led_port = NULL;
	}

	return 0;
}

static struct platform_driver cobalt_raq_led_driver = {
	.probe	= cobalt_raq_led_probe,
	.remove	= __devexit_p(cobalt_raq_led_remove),
	.driver = {
		.name	= "cobalt-raq-leds",
		.owner	= THIS_MODULE,
	},
};

static int __init cobalt_raq_led_init(void)
{
	return platform_driver_register(&cobalt_raq_led_driver);
}

module_init(cobalt_raq_led_init);
