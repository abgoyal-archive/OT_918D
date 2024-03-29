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
 * Generic IXP4xx beeper driver
 *
 * Copyright (C) 2005 Tower Technologies
 *
 * based on nslu2-io.c
 *  Copyright (C) 2004 Karen Spearel
 *
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 * Maintainers: http://www.nslu2-linux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <mach/hardware.h>

MODULE_AUTHOR("Alessandro Zummo <a.zummo@towertech.it>");
MODULE_DESCRIPTION("ixp4xx beeper driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:ixp4xx-beeper");

static DEFINE_SPINLOCK(beep_lock);

static void ixp4xx_spkr_control(unsigned int pin, unsigned int count)
{
	unsigned long flags;

	spin_lock_irqsave(&beep_lock, flags);

	 if (count) {
		gpio_line_config(pin, IXP4XX_GPIO_OUT);
		gpio_line_set(pin, IXP4XX_GPIO_LOW);

		*IXP4XX_OSRT2 = (count & ~IXP4XX_OST_RELOAD_MASK) | IXP4XX_OST_ENABLE;
	} else {
		gpio_line_config(pin, IXP4XX_GPIO_IN);
		gpio_line_set(pin, IXP4XX_GPIO_HIGH);

		*IXP4XX_OSRT2 = 0;
	}

	spin_unlock_irqrestore(&beep_lock, flags);
}

static int ixp4xx_spkr_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
	unsigned int pin = (unsigned int) input_get_drvdata(dev);
	unsigned int count = 0;

	if (type != EV_SND)
		return -1;

	switch (code) {
		case SND_BELL:
			if (value)
				value = 1000;
		case SND_TONE:
			break;
		default:
			return -1;
	}

	if (value > 20 && value < 32767)
#ifndef FREQ
		count = (ixp4xx_get_board_tick_rate() / (value * 4)) - 1;
#else
		count = (FREQ / (value * 4)) - 1;
#endif

	ixp4xx_spkr_control(pin, count);

	return 0;
}

static irqreturn_t ixp4xx_spkr_interrupt(int irq, void *dev_id)
{
	/* clear interrupt */
	*IXP4XX_OSST = IXP4XX_OSST_TIMER_2_PEND;

	/* flip the beeper output */
	*IXP4XX_GPIO_GPOUTR ^= (1 << (unsigned int) dev_id);

	return IRQ_HANDLED;
}

static int __devinit ixp4xx_spkr_probe(struct platform_device *dev)
{
	struct input_dev *input_dev;
	int err;

	input_dev = input_allocate_device();
	if (!input_dev)
		return -ENOMEM;

	input_set_drvdata(input_dev, (void *) dev->id);

	input_dev->name = "ixp4xx beeper",
	input_dev->phys = "ixp4xx/gpio";
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor  = 0x001f;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0100;
	input_dev->dev.parent = &dev->dev;

	input_dev->evbit[0] = BIT_MASK(EV_SND);
	input_dev->sndbit[0] = BIT_MASK(SND_BELL) | BIT_MASK(SND_TONE);
	input_dev->event = ixp4xx_spkr_event;

	err = request_irq(IRQ_IXP4XX_TIMER2, &ixp4xx_spkr_interrupt,
			  IRQF_DISABLED | IRQF_TIMER, "ixp4xx-beeper", (void *) dev->id);
	if (err)
		goto err_free_device;

	err = input_register_device(input_dev);
	if (err)
		goto err_free_irq;

	platform_set_drvdata(dev, input_dev);

	return 0;

 err_free_irq:
	free_irq(IRQ_IXP4XX_TIMER2, dev);
 err_free_device:
	input_free_device(input_dev);

	return err;
}

static int __devexit ixp4xx_spkr_remove(struct platform_device *dev)
{
	struct input_dev *input_dev = platform_get_drvdata(dev);
	unsigned int pin = (unsigned int) input_get_drvdata(input_dev);

	input_unregister_device(input_dev);
	platform_set_drvdata(dev, NULL);

	/* turn the speaker off */
	disable_irq(IRQ_IXP4XX_TIMER2);
	ixp4xx_spkr_control(pin, 0);

	free_irq(IRQ_IXP4XX_TIMER2, dev);

	return 0;
}

static void ixp4xx_spkr_shutdown(struct platform_device *dev)
{
	struct input_dev *input_dev = platform_get_drvdata(dev);
	unsigned int pin = (unsigned int) input_get_drvdata(input_dev);

	/* turn off the speaker */
	disable_irq(IRQ_IXP4XX_TIMER2);
	ixp4xx_spkr_control(pin, 0);
}

static struct platform_driver ixp4xx_spkr_platform_driver = {
	.driver		= {
		.name	= "ixp4xx-beeper",
		.owner	= THIS_MODULE,
	},
	.probe		= ixp4xx_spkr_probe,
	.remove		= __devexit_p(ixp4xx_spkr_remove),
	.shutdown	= ixp4xx_spkr_shutdown,
};

static int __init ixp4xx_spkr_init(void)
{
	return platform_driver_register(&ixp4xx_spkr_platform_driver);
}

static void __exit ixp4xx_spkr_exit(void)
{
	platform_driver_unregister(&ixp4xx_spkr_platform_driver);
}

module_init(ixp4xx_spkr_init);
module_exit(ixp4xx_spkr_exit);
