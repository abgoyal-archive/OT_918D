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
 * OpenCores Keyboard Controller Driver
 * http://www.opencores.org/project,keyboardcontroller
 *
 * Copyright 2007-2009 HV Sistemas S.L.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

struct opencores_kbd {
	struct input_dev *input;
	struct resource *addr_res;
	void __iomem *addr;
	int irq;
	unsigned short keycodes[128];
};

static irqreturn_t opencores_kbd_isr(int irq, void *dev_id)
{
	struct opencores_kbd *opencores_kbd = dev_id;
	struct input_dev *input = opencores_kbd->input;
	unsigned char c;

	c = readb(opencores_kbd->addr);
	input_report_key(input, c & 0x7f, c & 0x80 ? 0 : 1);
	input_sync(input);

	return IRQ_HANDLED;
}

static int __devinit opencores_kbd_probe(struct platform_device *pdev)
{
	struct input_dev *input;
	struct opencores_kbd *opencores_kbd;
	struct resource *res;
	int irq, i, error;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "missing board memory resource\n");
		return -EINVAL;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "missing board IRQ resource\n");
		return -EINVAL;
	}

	opencores_kbd = kzalloc(sizeof(*opencores_kbd), GFP_KERNEL);
	input = input_allocate_device();
	if (!opencores_kbd || !input) {
		dev_err(&pdev->dev, "failed to allocate device structures\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

	opencores_kbd->addr_res = res;
	res = request_mem_region(res->start, resource_size(res), pdev->name);
	if (!res) {
		dev_err(&pdev->dev, "failed to request I/O memory\n");
		error = -EBUSY;
		goto err_free_mem;
	}

	opencores_kbd->addr = ioremap(res->start, resource_size(res));
	if (!opencores_kbd->addr) {
		dev_err(&pdev->dev, "failed to remap I/O memory\n");
		error = -ENXIO;
		goto err_rel_mem;
	}

	opencores_kbd->input = input;
	opencores_kbd->irq = irq;

	input->name = pdev->name;
	input->phys = "opencores-kbd/input0";
	input->dev.parent = &pdev->dev;

	input_set_drvdata(input, opencores_kbd);

	input->id.bustype = BUS_HOST;
	input->id.vendor = 0x0001;
	input->id.product = 0x0001;
	input->id.version = 0x0100;

	input->keycode = opencores_kbd->keycodes;
	input->keycodesize = sizeof(opencores_kbd->keycodes[0]);
	input->keycodemax = ARRAY_SIZE(opencores_kbd->keycodes);

	__set_bit(EV_KEY, input->evbit);

	for (i = 0; i < ARRAY_SIZE(opencores_kbd->keycodes); i++) {
		/*
		 * OpenCores controller happens to have scancodes match
		 * our KEY_* definitions.
		 */
		opencores_kbd->keycodes[i] = i;
		__set_bit(opencores_kbd->keycodes[i], input->keybit);
	}
	__clear_bit(KEY_RESERVED, input->keybit);

	error = request_irq(irq, &opencores_kbd_isr,
			    IRQF_TRIGGER_RISING, pdev->name, opencores_kbd);
	if (error) {
		dev_err(&pdev->dev, "unable to claim irq %d\n", irq);
		goto err_unmap_mem;
	}

	error = input_register_device(input);
	if (error) {
		dev_err(&pdev->dev, "unable to register input device\n");
		goto err_free_irq;
	}

	platform_set_drvdata(pdev, opencores_kbd);

	return 0;

 err_free_irq:
	free_irq(irq, opencores_kbd);
 err_unmap_mem:
	iounmap(opencores_kbd->addr);
 err_rel_mem:
	release_mem_region(res->start, resource_size(res));
 err_free_mem:
	input_free_device(input);
	kfree(opencores_kbd);

	return error;
}

static int __devexit opencores_kbd_remove(struct platform_device *pdev)
{
	struct opencores_kbd *opencores_kbd = platform_get_drvdata(pdev);

	free_irq(opencores_kbd->irq, opencores_kbd);

	iounmap(opencores_kbd->addr);
	release_mem_region(opencores_kbd->addr_res->start,
		resource_size(opencores_kbd->addr_res));
	input_unregister_device(opencores_kbd->input);
	kfree(opencores_kbd);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

static struct platform_driver opencores_kbd_device_driver = {
	.probe    = opencores_kbd_probe,
	.remove   = __devexit_p(opencores_kbd_remove),
	.driver   = {
		.name = "opencores-kbd",
	},
};

static int __init opencores_kbd_init(void)
{
	return platform_driver_register(&opencores_kbd_device_driver);
}
module_init(opencores_kbd_init);

static void __exit opencores_kbd_exit(void)
{
	platform_driver_unregister(&opencores_kbd_device_driver);
}
module_exit(opencores_kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Javier Herrero <jherrero@hvsistemas.es>");
MODULE_DESCRIPTION("Keyboard driver for OpenCores Keyboard Controller");
