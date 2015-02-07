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
 * Common code for mac80211 Prism54 drivers
 *
 * Copyright (c) 2006, Michael Wu <flamingice@sourmilk.net>
 * Copyright (c) 2007-2009, Christian Lamparter <chunkeey@web.de>
 * Copyright 2008, Johannes Berg <johannes@sipsolutions.net>
 *
 * Based on:
 * - the islsm (softmac prism54) driver, which is:
 *   Copyright 2004-2006 Jean-Baptiste Note <jbnote@gmail.com>, et al.
 * - stlc45xx driver
 *   Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/firmware.h>
#include <linux/etherdevice.h>

#include <net/mac80211.h>
#ifdef CONFIG_P54_LEDS
#include <linux/leds.h>
#endif /* CONFIG_P54_LEDS */

#include "p54.h"
#include "lmac.h"

static void p54_update_leds(struct work_struct *work)
{
	struct p54_common *priv = container_of(work, struct p54_common,
					       led_work.work);
	int err, i, tmp, blink_delay = 400;
	bool rerun = false;

	/* Don't toggle the LED, when the device is down. */
	if (priv->mode == NL80211_IFTYPE_UNSPECIFIED)
		return ;

	for (i = 0; i < ARRAY_SIZE(priv->leds); i++)
		if (priv->leds[i].toggled) {
			priv->softled_state |= BIT(i);

			tmp = 70 + 200 / (priv->leds[i].toggled);
			if (tmp < blink_delay)
				blink_delay = tmp;

			if (priv->leds[i].led_dev.brightness == LED_OFF)
				rerun = true;

			priv->leds[i].toggled =
				!!priv->leds[i].led_dev.brightness;
		} else
			priv->softled_state &= ~BIT(i);

	err = p54_set_leds(priv);
	if (err && net_ratelimit())
		printk(KERN_ERR "%s: failed to update LEDs (%d).\n",
			wiphy_name(priv->hw->wiphy), err);

	if (rerun)
		ieee80211_queue_delayed_work(priv->hw, &priv->led_work,
			msecs_to_jiffies(blink_delay));
}

static void p54_led_brightness_set(struct led_classdev *led_dev,
				   enum led_brightness brightness)
{
	struct p54_led_dev *led = container_of(led_dev, struct p54_led_dev,
					       led_dev);
	struct ieee80211_hw *dev = led->hw_dev;
	struct p54_common *priv = dev->priv;

	if (priv->mode == NL80211_IFTYPE_UNSPECIFIED)
		return ;

	if ((brightness) && (led->registered)) {
		led->toggled++;
		ieee80211_queue_delayed_work(priv->hw, &priv->led_work, HZ/10);
	}
}

static int p54_register_led(struct p54_common *priv,
			    unsigned int led_index,
			    char *name, char *trigger)
{
	struct p54_led_dev *led = &priv->leds[led_index];
	int err;

	if (led->registered)
		return -EEXIST;

	snprintf(led->name, sizeof(led->name), "p54-%s::%s",
		 wiphy_name(priv->hw->wiphy), name);
	led->hw_dev = priv->hw;
	led->index = led_index;
	led->led_dev.name = led->name;
	led->led_dev.default_trigger = trigger;
	led->led_dev.brightness_set = p54_led_brightness_set;

	err = led_classdev_register(wiphy_dev(priv->hw->wiphy), &led->led_dev);
	if (err)
		printk(KERN_ERR "%s: Failed to register %s LED.\n",
			wiphy_name(priv->hw->wiphy), name);
	else
		led->registered = 1;

	return err;
}

int p54_init_leds(struct p54_common *priv)
{
	int err;

	/*
	 * TODO:
	 * Figure out if the EEPROM contains some hints about the number
	 * of available/programmable LEDs of the device.
	 */

	INIT_DELAYED_WORK(&priv->led_work, p54_update_leds);

	err = p54_register_led(priv, 0, "assoc",
			       ieee80211_get_assoc_led_name(priv->hw));
	if (err)
		return err;

	err = p54_register_led(priv, 1, "tx",
			       ieee80211_get_tx_led_name(priv->hw));
	if (err)
		return err;

	err = p54_register_led(priv, 2, "rx",
			       ieee80211_get_rx_led_name(priv->hw));
	if (err)
		return err;

	err = p54_register_led(priv, 3, "radio",
			       ieee80211_get_radio_led_name(priv->hw));
	if (err)
		return err;

	err = p54_set_leds(priv);
	return err;
}

void p54_unregister_leds(struct p54_common *priv)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(priv->leds); i++) {
		if (priv->leds[i].registered) {
			priv->leds[i].registered = false;
			priv->leds[i].toggled = 0;
			led_classdev_unregister(&priv->leds[i].led_dev);
		}
	}

	cancel_delayed_work_sync(&priv->led_work);
}
