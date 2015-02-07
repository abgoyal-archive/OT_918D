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
 * Driver for the enhanced rotary controller on pxa930 and pxa935
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/slab.h>

#include <mach/pxa930_rotary.h>

#define SBCR	(0x04)
#define ERCR	(0x0c)

#define SBCR_ERSB	(1 << 5)

struct pxa930_rotary {
	struct input_dev	*input_dev;
	void __iomem		*mmio_base;
	int			last_ercr;

	struct pxa930_rotary_platform_data *pdata;
};

static void clear_sbcr(struct pxa930_rotary *r)
{
	uint32_t sbcr = __raw_readl(r->mmio_base + SBCR);

	__raw_writel(sbcr | SBCR_ERSB, r->mmio_base + SBCR);
	__raw_writel(sbcr & ~SBCR_ERSB, r->mmio_base + SBCR);
}

static irqreturn_t rotary_irq(int irq, void *dev_id)
{
	struct pxa930_rotary *r = dev_id;
	struct pxa930_rotary_platform_data *pdata = r->pdata;
	int ercr, delta, key;

	ercr = __raw_readl(r->mmio_base + ERCR) & 0xf;
	clear_sbcr(r);

	delta = ercr - r->last_ercr;
	if (delta == 0)
		return IRQ_HANDLED;

	r->last_ercr = ercr;

	if (pdata->up_key && pdata->down_key) {
		key = (delta > 0) ? pdata->up_key : pdata->down_key;
		input_report_key(r->input_dev, key, 1);
		input_sync(r->input_dev);
		input_report_key(r->input_dev, key, 0);
	} else
		input_report_rel(r->input_dev, pdata->rel_code, delta);

	input_sync(r->input_dev);

	return IRQ_HANDLED;
}

static int pxa930_rotary_open(struct input_dev *dev)
{
	struct pxa930_rotary *r = input_get_drvdata(dev);

	clear_sbcr(r);

	return 0;
}

static void pxa930_rotary_close(struct input_dev *dev)
{
	struct pxa930_rotary *r = input_get_drvdata(dev);

	clear_sbcr(r);
}

static int __devinit pxa930_rotary_probe(struct platform_device *pdev)
{
	struct pxa930_rotary_platform_data *pdata = pdev->dev.platform_data;
	struct pxa930_rotary *r;
	struct input_dev *input_dev;
	struct resource *res;
	int irq;
	int err;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "no irq for rotary controller\n");
		return -ENXIO;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "no I/O memory defined\n");
		return -ENXIO;
	}

	if (!pdata) {
		dev_err(&pdev->dev, "no platform data defined\n");
		return -EINVAL;
	}

	r = kzalloc(sizeof(struct pxa930_rotary), GFP_KERNEL);
	if (!r)
		return -ENOMEM;

	r->mmio_base = ioremap_nocache(res->start, resource_size(res));
	if (r->mmio_base == NULL) {
		dev_err(&pdev->dev, "failed to remap IO memory\n");
		err = -ENXIO;
		goto failed_free;
	}

	r->pdata = pdata;
	platform_set_drvdata(pdev, r);

	/* allocate and register the input device */
	input_dev = input_allocate_device();
	if (!input_dev) {
		dev_err(&pdev->dev, "failed to allocate input device\n");
		err = -ENOMEM;
		goto failed_free_io;
	}

	input_dev->name = pdev->name;
	input_dev->id.bustype = BUS_HOST;
	input_dev->open = pxa930_rotary_open;
	input_dev->close = pxa930_rotary_close;
	input_dev->dev.parent = &pdev->dev;

	if (pdata->up_key && pdata->down_key) {
		__set_bit(pdata->up_key, input_dev->keybit);
		__set_bit(pdata->down_key, input_dev->keybit);
		__set_bit(EV_KEY, input_dev->evbit);
	} else {
		__set_bit(pdata->rel_code, input_dev->relbit);
		__set_bit(EV_REL, input_dev->evbit);
	}

	r->input_dev = input_dev;
	input_set_drvdata(input_dev, r);

	err = request_irq(irq, rotary_irq, IRQF_DISABLED,
			"enhanced rotary", r);
	if (err) {
		dev_err(&pdev->dev, "failed to request IRQ\n");
		goto failed_free_input;
	}

	err = input_register_device(input_dev);
	if (err) {
		dev_err(&pdev->dev, "failed to register input device\n");
		goto failed_free_irq;
	}

	return 0;

failed_free_irq:
	free_irq(irq, r);
failed_free_input:
	input_free_device(input_dev);
failed_free_io:
	iounmap(r->mmio_base);
failed_free:
	kfree(r);
	return err;
}

static int __devexit pxa930_rotary_remove(struct platform_device *pdev)
{
	struct pxa930_rotary *r = platform_get_drvdata(pdev);

	free_irq(platform_get_irq(pdev, 0), r);
	input_unregister_device(r->input_dev);
	iounmap(r->mmio_base);
	platform_set_drvdata(pdev, NULL);
	kfree(r);

	return 0;
}

static struct platform_driver pxa930_rotary_driver = {
	.driver		= {
		.name	= "pxa930-rotary",
		.owner	= THIS_MODULE,
	},
	.probe		= pxa930_rotary_probe,
	.remove		= __devexit_p(pxa930_rotary_remove),
};

static int __init pxa930_rotary_init(void)
{
	return platform_driver_register(&pxa930_rotary_driver);
}
module_init(pxa930_rotary_init);

static void __exit pxa930_rotary_exit(void)
{
	platform_driver_unregister(&pxa930_rotary_driver);
}
module_exit(pxa930_rotary_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver for PXA93x Enhanced Rotary Controller");
MODULE_AUTHOR("Yao Yong <yaoyong@marvell.com>");