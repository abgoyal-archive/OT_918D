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
 * Linux RFKILL support for RTL8187
 *
 * Copyright (c) 2009 Herton Ronaldo Krzesinski <herton@mandriva.com.br>
 *
 * Based on the RFKILL handling in the r8187 driver, which is:
 * Copyright (c) Realtek Semiconductor Corp. All rights reserved.
 *
 * Thanks to Realtek for their support!
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/usb.h>
#include <net/mac80211.h>

#include "rtl8187.h"
#include "rtl8187_rfkill.h"

static bool rtl8187_is_radio_enabled(struct rtl8187_priv *priv)
{
	u8 gpio;

	gpio = rtl818x_ioread8(priv, &priv->map->GPIO0);
	rtl818x_iowrite8(priv, &priv->map->GPIO0, gpio & ~priv->rfkill_mask);
	gpio = rtl818x_ioread8(priv, &priv->map->GPIO1);

	return gpio & priv->rfkill_mask;
}

void rtl8187_rfkill_init(struct ieee80211_hw *hw)
{
	struct rtl8187_priv *priv = hw->priv;

	priv->rfkill_off = rtl8187_is_radio_enabled(priv);
	printk(KERN_INFO "rtl8187: wireless switch is %s\n",
	       priv->rfkill_off ? "on" : "off");
	wiphy_rfkill_set_hw_state(hw->wiphy, !priv->rfkill_off);
	wiphy_rfkill_start_polling(hw->wiphy);
}

void rtl8187_rfkill_poll(struct ieee80211_hw *hw)
{
	bool enabled;
	struct rtl8187_priv *priv = hw->priv;

	mutex_lock(&priv->conf_mutex);
	enabled = rtl8187_is_radio_enabled(priv);
	if (unlikely(enabled != priv->rfkill_off)) {
		priv->rfkill_off = enabled;
		printk(KERN_INFO "rtl8187: wireless radio switch turned %s\n",
		       enabled ? "on" : "off");
		wiphy_rfkill_set_hw_state(hw->wiphy, !enabled);
	}
	mutex_unlock(&priv->conf_mutex);
}

void rtl8187_rfkill_exit(struct ieee80211_hw *hw)
{
	wiphy_rfkill_stop_polling(hw->wiphy);
}
