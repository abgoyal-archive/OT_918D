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
 * PC-Speaker driver for Linux
 *
 * Copyright (C) 1997-2001  David Woodhouse
 * Copyright (C) 2001-2008  Stas Sergeev
 */

#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <asm/bitops.h>
#include "pcsp_input.h"
#include "pcsp.h"

MODULE_AUTHOR("Stas Sergeev <stsp@users.sourceforge.net>");
MODULE_DESCRIPTION("PC-Speaker driver");
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE("{{PC-Speaker, pcsp}}");
MODULE_ALIAS("platform:pcspkr");

static int index = SNDRV_DEFAULT_IDX1;	/* Index 0-MAX */
static char *id = SNDRV_DEFAULT_STR1;	/* ID for this card */
static int enable = SNDRV_DEFAULT_ENABLE1;	/* Enable this card */
static int nopcm;	/* Disable PCM capability of the driver */

module_param(index, int, 0444);
MODULE_PARM_DESC(index, "Index value for pcsp soundcard.");
module_param(id, charp, 0444);
MODULE_PARM_DESC(id, "ID string for pcsp soundcard.");
module_param(enable, bool, 0444);
MODULE_PARM_DESC(enable, "Enable PC-Speaker sound.");
module_param(nopcm, bool, 0444);
MODULE_PARM_DESC(nopcm, "Disable PC-Speaker PCM sound. Only beeps remain.");

struct snd_pcsp pcsp_chip;

static int __devinit snd_pcsp_create(struct snd_card *card)
{
	static struct snd_device_ops ops = { };
	struct timespec tp;
	int err;
	int div, min_div, order;

	if (!nopcm) {
		hrtimer_get_res(CLOCK_MONOTONIC, &tp);
		if (tp.tv_sec || tp.tv_nsec > PCSP_MAX_PERIOD_NS) {
			printk(KERN_ERR "PCSP: Timer resolution is not sufficient "
				"(%linS)\n", tp.tv_nsec);
			printk(KERN_ERR "PCSP: Make sure you have HPET and ACPI "
				"enabled.\n");
			printk(KERN_ERR "PCSP: Turned into nopcm mode.\n");
			nopcm = 1;
		}
	}

	if (loops_per_jiffy >= PCSP_MIN_LPJ && tp.tv_nsec <= PCSP_MIN_PERIOD_NS)
		min_div = MIN_DIV;
	else
		min_div = MAX_DIV;
#if PCSP_DEBUG
	printk(KERN_DEBUG "PCSP: lpj=%li, min_div=%i, res=%li\n",
	       loops_per_jiffy, min_div, tp.tv_nsec);
#endif

	div = MAX_DIV / min_div;
	order = fls(div) - 1;

	pcsp_chip.max_treble = min(order, PCSP_MAX_TREBLE);
	pcsp_chip.treble = min(pcsp_chip.max_treble, PCSP_DEFAULT_TREBLE);
	pcsp_chip.playback_ptr = 0;
	pcsp_chip.period_ptr = 0;
	atomic_set(&pcsp_chip.timer_active, 0);
	pcsp_chip.enable = 1;
	pcsp_chip.pcspkr = 1;

	spin_lock_init(&pcsp_chip.substream_lock);

	pcsp_chip.card = card;
	pcsp_chip.port = 0x61;
	pcsp_chip.irq = -1;
	pcsp_chip.dma = -1;

	/* Register device */
	err = snd_device_new(card, SNDRV_DEV_LOWLEVEL, &pcsp_chip, &ops);
	if (err < 0)
		return err;

	return 0;
}

static int __devinit snd_card_pcsp_probe(int devnum, struct device *dev)
{
	struct snd_card *card;
	int err;

	if (devnum != 0)
		return -EINVAL;

	hrtimer_init(&pcsp_chip.timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	pcsp_chip.timer.function = pcsp_do_timer;

	err = snd_card_create(index, id, THIS_MODULE, 0, &card);
	if (err < 0)
		return err;

	err = snd_pcsp_create(card);
	if (err < 0) {
		snd_card_free(card);
		return err;
	}
	if (!nopcm) {
		err = snd_pcsp_new_pcm(&pcsp_chip);
		if (err < 0) {
			snd_card_free(card);
			return err;
		}
	}
	err = snd_pcsp_new_mixer(&pcsp_chip, nopcm);
	if (err < 0) {
		snd_card_free(card);
		return err;
	}

	snd_card_set_dev(pcsp_chip.card, dev);

	strcpy(card->driver, "PC-Speaker");
	strcpy(card->shortname, "pcsp");
	sprintf(card->longname, "Internal PC-Speaker at port 0x%x",
		pcsp_chip.port);

	err = snd_card_register(card);
	if (err < 0) {
		snd_card_free(card);
		return err;
	}

	return 0;
}

static int __devinit alsa_card_pcsp_init(struct device *dev)
{
	int err;

	err = snd_card_pcsp_probe(0, dev);
	if (err) {
		printk(KERN_ERR "PC-Speaker initialization failed.\n");
		return err;
	}

#ifdef CONFIG_DEBUG_PAGEALLOC
	/* Well, CONFIG_DEBUG_PAGEALLOC makes the sound horrible. Lets alert */
	printk(KERN_WARNING "PCSP: CONFIG_DEBUG_PAGEALLOC is enabled, "
	       "which may make the sound noisy.\n");
#endif

	return 0;
}

static void __devexit alsa_card_pcsp_exit(struct snd_pcsp *chip)
{
	snd_card_free(chip->card);
}

static int __devinit pcsp_probe(struct platform_device *dev)
{
	int err;

	err = pcspkr_input_init(&pcsp_chip.input_dev, &dev->dev);
	if (err < 0)
		return err;

	err = alsa_card_pcsp_init(&dev->dev);
	if (err < 0) {
		pcspkr_input_remove(pcsp_chip.input_dev);
		return err;
	}

	platform_set_drvdata(dev, &pcsp_chip);
	return 0;
}

static int __devexit pcsp_remove(struct platform_device *dev)
{
	struct snd_pcsp *chip = platform_get_drvdata(dev);
	alsa_card_pcsp_exit(chip);
	pcspkr_input_remove(chip->input_dev);
	platform_set_drvdata(dev, NULL);
	return 0;
}

static void pcsp_stop_beep(struct snd_pcsp *chip)
{
	pcsp_sync_stop(chip);
	pcspkr_stop_sound();
}

#ifdef CONFIG_PM
static int pcsp_suspend(struct platform_device *dev, pm_message_t state)
{
	struct snd_pcsp *chip = platform_get_drvdata(dev);
	pcsp_stop_beep(chip);
	snd_pcm_suspend_all(chip->pcm);
	return 0;
}
#else
#define pcsp_suspend NULL
#endif	/* CONFIG_PM */

static void pcsp_shutdown(struct platform_device *dev)
{
	struct snd_pcsp *chip = platform_get_drvdata(dev);
	pcsp_stop_beep(chip);
}

static struct platform_driver pcsp_platform_driver = {
	.driver		= {
		.name	= "pcspkr",
		.owner	= THIS_MODULE,
	},
	.probe		= pcsp_probe,
	.remove		= __devexit_p(pcsp_remove),
	.suspend	= pcsp_suspend,
	.shutdown	= pcsp_shutdown,
};

static int __init pcsp_init(void)
{
	if (!enable)
		return -ENODEV;
	return platform_driver_register(&pcsp_platform_driver);
}

static void __exit pcsp_exit(void)
{
	platform_driver_unregister(&pcsp_platform_driver);
}

module_init(pcsp_init);
module_exit(pcsp_exit);
