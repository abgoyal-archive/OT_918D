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
 * phycore-ac97.c  --  SoC audio for imx_phycore in AC97 mode
 *
 * Copyright 2009 Sascha Hauer, Pengutronix <s.hauer@pengutronix.de>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <asm/mach-types.h>

#include "../codecs/wm9712.h"
#include "imx-ssi.h"

static struct snd_soc_card imx_phycore;

static struct snd_soc_ops imx_phycore_hifi_ops = {
};

static struct snd_soc_dai_link imx_phycore_dai_ac97[] = {
	{
		.name		= "HiFi",
		.stream_name	= "HiFi",
		.codec_dai	= &wm9712_dai[WM9712_DAI_AC97_HIFI],
		.ops		= &imx_phycore_hifi_ops,
	},
};

static struct snd_soc_card imx_phycore = {
	.name		= "PhyCORE-audio",
	.platform	= &imx_soc_platform,
	.dai_link	= imx_phycore_dai_ac97,
	.num_links	= ARRAY_SIZE(imx_phycore_dai_ac97),
};

static struct snd_soc_device imx_phycore_snd_devdata = {
	.card		= &imx_phycore,
	.codec_dev	= &soc_codec_dev_wm9712,
};

static struct platform_device *imx_phycore_snd_device;

static int __init imx_phycore_init(void)
{
	int ret;

	if (!machine_is_pcm043() && !machine_is_pca100())
		/* return happy. We might run on a totally different machine */
		return 0;

	imx_phycore_snd_device = platform_device_alloc("soc-audio", -1);
	if (!imx_phycore_snd_device)
		return -ENOMEM;

	imx_phycore_dai_ac97[0].cpu_dai = &imx_ssi_pcm_dai[0];

	platform_set_drvdata(imx_phycore_snd_device, &imx_phycore_snd_devdata);
	imx_phycore_snd_devdata.dev = &imx_phycore_snd_device->dev;
	ret = platform_device_add(imx_phycore_snd_device);

	if (ret) {
		printk(KERN_ERR "ASoC: Platform device allocation failed\n");
		platform_device_put(imx_phycore_snd_device);
	}

	return ret;
}

static void __exit imx_phycore_exit(void)
{
	platform_device_unregister(imx_phycore_snd_device);
}

late_initcall(imx_phycore_init);
module_exit(imx_phycore_exit);

MODULE_AUTHOR("Sascha Hauer <s.hauer@pengutronix.de>");
MODULE_DESCRIPTION("PhyCORE ALSA SoC driver");
MODULE_LICENSE("GPL");
