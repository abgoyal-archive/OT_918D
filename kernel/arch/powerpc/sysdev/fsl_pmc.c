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
 * Suspend/resume support
 *
 * Copyright 2009  MontaVista Software, Inc.
 *
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/suspend.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/of_platform.h>

struct pmc_regs {
	__be32 devdisr;
	__be32 devdisr2;
	__be32 :32;
	__be32 :32;
	__be32 pmcsr;
#define PMCSR_SLP	(1 << 17)
};

static struct device *pmc_dev;
static struct pmc_regs __iomem *pmc_regs;

static int pmc_suspend_enter(suspend_state_t state)
{
	int ret;

	setbits32(&pmc_regs->pmcsr, PMCSR_SLP);
	/* At this point, the CPU is asleep. */

	/* Upon resume, wait for SLP bit to be clear. */
	ret = spin_event_timeout((in_be32(&pmc_regs->pmcsr) & PMCSR_SLP) == 0,
				 10000, 10) ? 0 : -ETIMEDOUT;
	if (ret)
		dev_err(pmc_dev, "tired waiting for SLP bit to clear\n");
	return ret;
}

static int pmc_suspend_valid(suspend_state_t state)
{
	if (state != PM_SUSPEND_STANDBY)
		return 0;
	return 1;
}

static struct platform_suspend_ops pmc_suspend_ops = {
	.valid = pmc_suspend_valid,
	.enter = pmc_suspend_enter,
};

static int pmc_probe(struct of_device *ofdev, const struct of_device_id *id)
{
	pmc_regs = of_iomap(ofdev->dev.of_node, 0);
	if (!pmc_regs)
		return -ENOMEM;

	pmc_dev = &ofdev->dev;
	suspend_set_ops(&pmc_suspend_ops);
	return 0;
}

static const struct of_device_id pmc_ids[] = {
	{ .compatible = "fsl,mpc8548-pmc", },
	{ .compatible = "fsl,mpc8641d-pmc", },
	{ },
};

static struct of_platform_driver pmc_driver = {
	.driver = {
		.name = "fsl-pmc",
		.owner = THIS_MODULE,
		.of_match_table = pmc_ids,
	},
	.probe = pmc_probe,
};

static int __init pmc_init(void)
{
	return of_register_platform_driver(&pmc_driver);
}
device_initcall(pmc_init);
