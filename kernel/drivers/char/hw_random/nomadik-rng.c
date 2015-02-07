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
 * Nomadik RNG support
 *  Copyright 2009 Alessandro Rubini
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/amba/bus.h>
#include <linux/hw_random.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>

static struct clk *rng_clk;

static int nmk_rng_read(struct hwrng *rng, void *data, size_t max, bool wait)
{
	void __iomem *base = (void __iomem *)rng->priv;

	/*
	 * The register is 32 bits and gives 16 random bits (low half).
	 * A subsequent read will delay the core for 400ns, so we just read
	 * once and accept the very unlikely very small delay, even if wait==0.
	 */
	*(u16 *)data = __raw_readl(base + 8) & 0xffff;
	return 2;
}

/* we have at most one RNG per machine, granted */
static struct hwrng nmk_rng = {
	.name		= "nomadik",
	.read		= nmk_rng_read,
};

static int nmk_rng_probe(struct amba_device *dev, struct amba_id *id)
{
	void __iomem *base;
	int ret;

	rng_clk = clk_get(&dev->dev, NULL);
	if (IS_ERR(rng_clk)) {
		dev_err(&dev->dev, "could not get rng clock\n");
		ret = PTR_ERR(rng_clk);
		return ret;
	}

	clk_enable(rng_clk);

	ret = amba_request_regions(dev, dev->dev.init_name);
	if (ret)
		return ret;
	ret = -ENOMEM;
	base = ioremap(dev->res.start, resource_size(&dev->res));
	if (!base)
		goto out_release;
	nmk_rng.priv = (unsigned long)base;
	ret = hwrng_register(&nmk_rng);
	if (ret)
		goto out_unmap;
	return 0;

out_unmap:
	iounmap(base);
out_release:
	amba_release_regions(dev);
	clk_disable(rng_clk);
	clk_put(rng_clk);
	return ret;
}

static int nmk_rng_remove(struct amba_device *dev)
{
	void __iomem *base = (void __iomem *)nmk_rng.priv;
	hwrng_unregister(&nmk_rng);
	iounmap(base);
	amba_release_regions(dev);
	clk_disable(rng_clk);
	clk_put(rng_clk);
	return 0;
}

static struct amba_id nmk_rng_ids[] = {
	{
		.id	= 0x000805e1,
		.mask	= 0x000fffff, /* top bits are rev and cfg: accept all */
	},
	{0, 0},
};

static struct amba_driver nmk_rng_driver = {
	.drv = {
		.owner = THIS_MODULE,
		.name = "rng",
		},
	.probe = nmk_rng_probe,
	.remove = nmk_rng_remove,
	.id_table = nmk_rng_ids,
};

static int __init nmk_rng_init(void)
{
	return amba_driver_register(&nmk_rng_driver);
}

static void __devexit nmk_rng_exit(void)
{
	amba_driver_unregister(&nmk_rng_driver);
}

module_init(nmk_rng_init);
module_exit(nmk_rng_exit);

MODULE_LICENSE("GPL");
