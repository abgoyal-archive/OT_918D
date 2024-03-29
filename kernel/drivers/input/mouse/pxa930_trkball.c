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
 * PXA930 track ball mouse driver
 *
 * Copyright (C) 2007 Marvell International Ltd.
 * 2008-02-28: Yong Yao <yaoyong@marvell.com>
 *             initial version
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/input.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/slab.h>

#include <mach/hardware.h>
#include <mach/pxa930_trkball.h>

/* Trackball Controller Register Definitions */
#define TBCR		(0x000C)
#define TBCNTR		(0x0010)
#define TBSBC		(0x0014)

#define TBCR_TBRST	(1 << 1)
#define TBCR_TBSB	(1 << 10)

#define TBCR_Y_FLT(n)	(((n) & 0xf) << 6)
#define TBCR_X_FLT(n)	(((n) & 0xf) << 2)

#define TBCNTR_YM(n)	(((n) >> 24) & 0xff)
#define TBCNTR_YP(n)	(((n) >> 16) & 0xff)
#define TBCNTR_XM(n)	(((n) >> 8) & 0xff)
#define TBCNTR_XP(n)	((n) & 0xff)

#define TBSBC_TBSBC	(0x1)

struct pxa930_trkball {
	struct pxa930_trkball_platform_data *pdata;

	/* Memory Mapped Register */
	struct resource *mem;
	void __iomem *mmio_base;

	struct input_dev *input;
};

static irqreturn_t pxa930_trkball_interrupt(int irq, void *dev_id)
{
	struct pxa930_trkball *trkball = dev_id;
	struct input_dev *input = trkball->input;
	int tbcntr, x, y;

	/* According to the spec software must read TBCNTR twice:
	 * if the read value is the same, the reading is valid
	 */
	tbcntr = __raw_readl(trkball->mmio_base + TBCNTR);

	if (tbcntr == __raw_readl(trkball->mmio_base + TBCNTR)) {
		x = (TBCNTR_XP(tbcntr) - TBCNTR_XM(tbcntr)) / 2;
		y = (TBCNTR_YP(tbcntr) - TBCNTR_YM(tbcntr)) / 2;

		input_report_rel(input, REL_X, x);
		input_report_rel(input, REL_Y, y);
		input_sync(input);
	}

	__raw_writel(TBSBC_TBSBC, trkball->mmio_base + TBSBC);
	__raw_writel(0, trkball->mmio_base + TBSBC);

	return IRQ_HANDLED;
}

/* For TBCR, we need to wait for a while to make sure it has been modified. */
static int write_tbcr(struct pxa930_trkball *trkball, int v)
{
	int i = 100;

	__raw_writel(v, trkball->mmio_base + TBCR);

	while (--i) {
		if (__raw_readl(trkball->mmio_base + TBCR) == v)
			break;
		msleep(1);
	}

	if (i == 0) {
		pr_err("%s: timed out writing TBCR(%x)!\n", __func__, v);
		return -ETIMEDOUT;
	}

	return 0;
}

static void pxa930_trkball_config(struct pxa930_trkball *trkball)
{
	uint32_t tbcr;

	/* According to spec, need to write the filters of x,y to 0xf first! */
	tbcr = __raw_readl(trkball->mmio_base + TBCR);
	write_tbcr(trkball, tbcr | TBCR_X_FLT(0xf) | TBCR_Y_FLT(0xf));
	write_tbcr(trkball, TBCR_X_FLT(trkball->pdata->x_filter) |
			    TBCR_Y_FLT(trkball->pdata->y_filter));

	/* According to spec, set TBCR_TBRST first, before clearing it! */
	tbcr = __raw_readl(trkball->mmio_base + TBCR);
	write_tbcr(trkball, tbcr | TBCR_TBRST);
	write_tbcr(trkball, tbcr & ~TBCR_TBRST);

	__raw_writel(TBSBC_TBSBC, trkball->mmio_base + TBSBC);
	__raw_writel(0, trkball->mmio_base + TBSBC);

	pr_debug("%s: final TBCR=%x!\n", __func__,
		 __raw_readl(trkball->mmio_base + TBCR));
}

static int pxa930_trkball_open(struct input_dev *dev)
{
	struct pxa930_trkball *trkball = input_get_drvdata(dev);

	pxa930_trkball_config(trkball);

	return 0;
}

static void pxa930_trkball_disable(struct pxa930_trkball *trkball)
{
	uint32_t tbcr = __raw_readl(trkball->mmio_base + TBCR);

	/* Held in reset, gate the 32-KHz input clock off */
	write_tbcr(trkball, tbcr | TBCR_TBRST);
}

static void pxa930_trkball_close(struct input_dev *dev)
{
	struct pxa930_trkball *trkball = input_get_drvdata(dev);

	pxa930_trkball_disable(trkball);
}

static int __devinit pxa930_trkball_probe(struct platform_device *pdev)
{
	struct pxa930_trkball *trkball;
	struct input_dev *input;
	struct resource *res;
	int irq, error;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "failed to get trkball irq\n");
		return -ENXIO;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "failed to get register memory\n");
		return -ENXIO;
	}

	trkball = kzalloc(sizeof(struct pxa930_trkball), GFP_KERNEL);
	if (!trkball)
		return -ENOMEM;

	trkball->pdata = pdev->dev.platform_data;
	if (!trkball->pdata) {
		dev_err(&pdev->dev, "no platform data defined\n");
		error = -EINVAL;
		goto failed;
	}

	trkball->mmio_base = ioremap_nocache(res->start, resource_size(res));
	if (!trkball->mmio_base) {
		dev_err(&pdev->dev, "failed to ioremap registers\n");
		error = -ENXIO;
		goto failed;
	}

	/* held the module in reset, will be enabled in open() */
	pxa930_trkball_disable(trkball);

	error = request_irq(irq, pxa930_trkball_interrupt, IRQF_DISABLED,
			    pdev->name, trkball);
	if (error) {
		dev_err(&pdev->dev, "failed to request irq: %d\n", error);
		goto failed_free_io;
	}

	platform_set_drvdata(pdev, trkball);

	input = input_allocate_device();
	if (!input) {
		dev_err(&pdev->dev, "failed to allocate input device\n");
		error = -ENOMEM;
		goto failed_free_irq;
	}

	input->name = pdev->name;
	input->id.bustype = BUS_HOST;
	input->open = pxa930_trkball_open;
	input->close = pxa930_trkball_close;
	input->dev.parent = &pdev->dev;
	input_set_drvdata(input, trkball);

	trkball->input = input;

	input_set_capability(input, EV_REL, REL_X);
	input_set_capability(input, EV_REL, REL_Y);

	error = input_register_device(input);
	if (error) {
		dev_err(&pdev->dev, "unable to register input device\n");
		goto failed_free_input;
	}

	return 0;

failed_free_input:
	input_free_device(input);
failed_free_irq:
	free_irq(irq, trkball);
failed_free_io:
	iounmap(trkball->mmio_base);
failed:
	kfree(trkball);
	return error;
}

static int __devexit pxa930_trkball_remove(struct platform_device *pdev)
{
	struct pxa930_trkball *trkball = platform_get_drvdata(pdev);
	int irq = platform_get_irq(pdev, 0);

	input_unregister_device(trkball->input);
	free_irq(irq, trkball);
	iounmap(trkball->mmio_base);
	kfree(trkball);

	return 0;
}

static struct platform_driver pxa930_trkball_driver = {
	.driver		= {
		.name	= "pxa930-trkball",
	},
	.probe		= pxa930_trkball_probe,
	.remove		= __devexit_p(pxa930_trkball_remove),
};

static int __init pxa930_trkball_init(void)
{
	return platform_driver_register(&pxa930_trkball_driver);
}

static void __exit pxa930_trkball_exit(void)
{
	platform_driver_unregister(&pxa930_trkball_driver);
}

module_init(pxa930_trkball_init);
module_exit(pxa930_trkball_exit);

MODULE_AUTHOR("Yong Yao <yaoyong@marvell.com>");
MODULE_DESCRIPTION("PXA930 Trackball Mouse Driver");
MODULE_LICENSE("GPL");
