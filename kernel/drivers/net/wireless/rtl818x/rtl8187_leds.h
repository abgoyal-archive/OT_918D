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
 * Definitions for RTL8187 leds
 *
 * Copyright 2009 Larry Finger <Larry.Finger@lwfinger.net>
 *
 * Based on the LED handling in the r8187 driver, which is:
 * Copyright (c) Realtek Semiconductor Corp. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef RTL8187_LED_H
#define RTL8187_LED_H

#ifdef CONFIG_RTL8187_LEDS

#define RTL8187_LED_MAX_NAME_LEN	21

#include <linux/leds.h>
#include <linux/types.h>

enum {
	LED_PIN_LED0,
	LED_PIN_LED1,
	LED_PIN_GPIO0,
	LED_PIN_HW
};

enum {
	EEPROM_CID_RSVD0 = 0x00,
	EEPROM_CID_RSVD1 = 0xFF,
	EEPROM_CID_ALPHA0 = 0x01,
	EEPROM_CID_SERCOMM_PS = 0x02,
	EEPROM_CID_HW = 0x03,
	EEPROM_CID_TOSHIBA = 0x04,
	EEPROM_CID_QMI = 0x07,
	EEPROM_CID_DELL = 0x08
};

struct rtl8187_led {
	struct ieee80211_hw *dev;
	/* The LED class device */
	struct led_classdev led_dev;
	/* The pin/method used to control the led */
	u8 ledpin;
	/* The unique name string for this LED device. */
	char name[RTL8187_LED_MAX_NAME_LEN + 1];
	/* If the LED is radio or tx/rx */
	bool is_radio;
};

void rtl8187_leds_init(struct ieee80211_hw *dev, u16 code);
void rtl8187_leds_exit(struct ieee80211_hw *dev);

#endif /* def CONFIG_RTL8187_LEDS */

#endif /* RTL8187_LED_H */
