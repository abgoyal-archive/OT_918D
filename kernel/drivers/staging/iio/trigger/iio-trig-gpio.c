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
 * Industrial I/O - gpio based trigger support
 *
 * Copyright (c) 2008 Jonathan Cameron
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Currently this is more of a functioning proof of concept that a fully
 * fledged trigger driver.
 *
 * TODO:
 *
 * Add board config elements to allow specification of startup settings.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#include "../iio.h"
#include "../trigger.h"

static LIST_HEAD(iio_gpio_trigger_list);
static DEFINE_MUTEX(iio_gpio_trigger_list_lock);

struct iio_gpio_trigger_info {
	struct mutex in_use;
	unsigned int irq;
};
/*
 * Need to reference count these triggers and only enable gpio interrupts
 * as appropriate.
 */

/* So what functionality do we want in here?... */
/* set high / low as interrupt type? */

static irqreturn_t iio_gpio_trigger_poll(int irq, void *private)
{
	iio_trigger_poll(private);
	return IRQ_HANDLED;
}

static DEVICE_ATTR(name, S_IRUGO, iio_trigger_read_name, NULL);

static struct attribute *iio_gpio_trigger_attrs[] = {
	&dev_attr_name.attr,
	NULL,
};

static const struct attribute_group iio_gpio_trigger_attr_group = {
	.attrs = iio_gpio_trigger_attrs,
};

static int iio_gpio_trigger_probe(struct platform_device *pdev)
{
	struct iio_gpio_trigger_info *trig_info;
	struct iio_trigger *trig, *trig2;
	unsigned long irqflags;
	struct resource *irq_res;
	int irq, ret = 0, irq_res_cnt = 0;

	do {
		irq_res = platform_get_resource(pdev,
				IORESOURCE_IRQ, irq_res_cnt);

		if (irq_res == NULL) {
			if (irq_res_cnt == 0)
				dev_err(&pdev->dev, "No GPIO IRQs specified");
			break;
		}
		irqflags = (irq_res->flags & IRQF_TRIGGER_MASK) | IRQF_SHARED;

		for (irq = irq_res->start; irq <= irq_res->end; irq++) {

			trig = iio_allocate_trigger();
			if (!trig) {
				ret = -ENOMEM;
				goto error_free_completed_registrations;
			}

			trig_info = kzalloc(sizeof(*trig_info), GFP_KERNEL);
			if (!trig_info) {
				ret = -ENOMEM;
				goto error_put_trigger;
			}
			trig->control_attrs = &iio_gpio_trigger_attr_group;
			trig->private_data = trig_info;
			trig_info->irq = irq;
			trig->owner = THIS_MODULE;
			trig->name = kmalloc(IIO_TRIGGER_NAME_LENGTH,
					GFP_KERNEL);
			if (!trig->name) {
				ret = -ENOMEM;
				goto error_free_trig_info;
			}
			snprintf((char *)trig->name,
				 IIO_TRIGGER_NAME_LENGTH,
				 "irqtrig%d", irq);

			ret = request_irq(irq, iio_gpio_trigger_poll,
					  irqflags, trig->name, trig);
			if (ret) {
				dev_err(&pdev->dev,
					"request IRQ-%d failed", irq);
				goto error_free_name;
			}

			ret = iio_trigger_register(trig);
			if (ret)
				goto error_release_irq;

			list_add_tail(&trig->alloc_list,
					&iio_gpio_trigger_list);
		}

		irq_res_cnt++;
	} while (irq_res != NULL);


	return 0;

/* First clean up the partly allocated trigger */
error_release_irq:
	free_irq(irq, trig);
error_free_name:
	kfree(trig->name);
error_free_trig_info:
	kfree(trig_info);
error_put_trigger:
	iio_put_trigger(trig);
error_free_completed_registrations:
	/* The rest should have been added to the iio_gpio_trigger_list */
	list_for_each_entry_safe(trig,
				 trig2,
				 &iio_gpio_trigger_list,
				 alloc_list) {
		trig_info = trig->private_data;
		free_irq(gpio_to_irq(trig_info->irq), trig);
		kfree(trig->name);
		kfree(trig_info);
		iio_trigger_unregister(trig);
	}

	return ret;
}

static int iio_gpio_trigger_remove(struct platform_device *pdev)
{
	struct iio_trigger *trig, *trig2;
	struct iio_gpio_trigger_info *trig_info;

	mutex_lock(&iio_gpio_trigger_list_lock);
	list_for_each_entry_safe(trig,
				 trig2,
				 &iio_gpio_trigger_list,
				 alloc_list) {
		trig_info = trig->private_data;
		iio_trigger_unregister(trig);
		free_irq(trig_info->irq, trig);
		kfree(trig->name);
		kfree(trig_info);
		iio_put_trigger(trig);
	}
	mutex_unlock(&iio_gpio_trigger_list_lock);

	return 0;
}

static struct platform_driver iio_gpio_trigger_driver = {
	.probe = iio_gpio_trigger_probe,
	.remove = iio_gpio_trigger_remove,
	.driver = {
		.name = "iio_gpio_trigger",
		.owner = THIS_MODULE,
	},
};

static int __init iio_gpio_trig_init(void)
{
	return platform_driver_register(&iio_gpio_trigger_driver);
}
module_init(iio_gpio_trig_init);

static void __exit iio_gpio_trig_exit(void)
{
	platform_driver_unregister(&iio_gpio_trigger_driver);
}
module_exit(iio_gpio_trig_exit);

MODULE_AUTHOR("Jonathan Cameron <jic23@cam.ac.uk>");
MODULE_DESCRIPTION("Example gpio trigger for the iio subsystem");
MODULE_LICENSE("GPL v2");
