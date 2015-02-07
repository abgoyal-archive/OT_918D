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
 * Generic TXx9 ACLC machine driver
 *
 * Copyright (C) 2009 Atsushi Nemoto
 *
 * Based on RBTX49xx patch from CELF patch archive.
 * (C) Copyright TOSHIBA CORPORATION 2004-2006
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This is a very generic AC97 sound machine driver for boards which
 * have (AC97) audio at ACLC (e.g. RBTX49XX boards).
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include "../codecs/ac97.h"
#include "txx9aclc.h"

static struct snd_soc_dai_link txx9aclc_generic_dai = {
	.name = "AC97",
	.stream_name = "AC97 HiFi",
	.cpu_dai = &txx9aclc_ac97_dai,
	.codec_dai = &ac97_dai,
};

static struct snd_soc_card txx9aclc_generic_card = {
	.name		= "Generic TXx9 ACLC Audio",
	.platform	= &txx9aclc_soc_platform,
	.dai_link	= &txx9aclc_generic_dai,
	.num_links	= 1,
};

static struct txx9aclc_soc_device txx9aclc_generic_soc_device = {
	.soc_dev = {
		.card		= &txx9aclc_generic_card,
		.codec_dev	= &soc_codec_dev_ac97,
	},
};

static int __init txx9aclc_generic_probe(struct platform_device *pdev)
{
	struct txx9aclc_soc_device *dev = &txx9aclc_generic_soc_device;
	struct platform_device *soc_pdev;
	int ret;

	soc_pdev = platform_device_alloc("soc-audio", -1);
	if (!soc_pdev)
		return -ENOMEM;
	platform_set_drvdata(soc_pdev, &dev->soc_dev);
	dev->soc_dev.dev = &soc_pdev->dev;
	ret = platform_device_add(soc_pdev);
	if (ret) {
		platform_device_put(soc_pdev);
		return ret;
	}
	platform_set_drvdata(pdev, soc_pdev);
	return 0;
}

static int __exit txx9aclc_generic_remove(struct platform_device *pdev)
{
	struct platform_device *soc_pdev = platform_get_drvdata(pdev);

	platform_device_unregister(soc_pdev);
	return 0;
}

static struct platform_driver txx9aclc_generic_driver = {
	.remove = txx9aclc_generic_remove,
	.driver = {
		.name = "txx9aclc-generic",
		.owner = THIS_MODULE,
	},
};

static int __init txx9aclc_generic_init(void)
{
	return platform_driver_probe(&txx9aclc_generic_driver,
				     txx9aclc_generic_probe);
}

static void __exit txx9aclc_generic_exit(void)
{
	platform_driver_unregister(&txx9aclc_generic_driver);
}

module_init(txx9aclc_generic_init);
module_exit(txx9aclc_generic_exit);

MODULE_AUTHOR("Atsushi Nemoto <anemo@mba.ocn.ne.jp>");
MODULE_DESCRIPTION("Generic TXx9 ACLC ALSA SoC audio driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:txx9aclc-generic");
