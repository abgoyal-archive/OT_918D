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

/* Helpers for managing scan queues
 *
 * See copyright notice in main.c
 */

#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ieee80211.h>
#include <net/cfg80211.h>

#include "hermes.h"
#include "orinoco.h"
#include "main.h"

#include "scan.h"

#define ZERO_DBM_OFFSET 0x95
#define MAX_SIGNAL_LEVEL 0x8A
#define MIN_SIGNAL_LEVEL 0x2F

#define SIGNAL_TO_DBM(x)					\
	(clamp_t(s32, (x), MIN_SIGNAL_LEVEL, MAX_SIGNAL_LEVEL)	\
	 - ZERO_DBM_OFFSET)
#define SIGNAL_TO_MBM(x) (SIGNAL_TO_DBM(x) * 100)

static int symbol_build_supp_rates(u8 *buf, const __le16 *rates)
{
	int i;
	u8 rate;

	buf[0] = WLAN_EID_SUPP_RATES;
	for (i = 0; i < 5; i++) {
		rate = le16_to_cpu(rates[i]);
		/* NULL terminated */
		if (rate == 0x0)
			break;
		buf[i + 2] = rate;
	}
	buf[1] = i;

	return i + 2;
}

static int prism_build_supp_rates(u8 *buf, const u8 *rates)
{
	int i;

	buf[0] = WLAN_EID_SUPP_RATES;
	for (i = 0; i < 8; i++) {
		/* NULL terminated */
		if (rates[i] == 0x0)
			break;
		buf[i + 2] = rates[i];
	}
	buf[1] = i;

	/* We might still have another 2 rates, which need to go in
	 * extended supported rates */
	if (i == 8 && rates[i] > 0) {
		buf[10] = WLAN_EID_EXT_SUPP_RATES;
		for (; i < 10; i++) {
			/* NULL terminated */
			if (rates[i] == 0x0)
				break;
			buf[i + 2] = rates[i];
		}
		buf[11] = i - 8;
	}

	return (i < 8) ? i + 2 : i + 4;
}

static void orinoco_add_hostscan_result(struct orinoco_private *priv,
					const union hermes_scan_info *bss)
{
	struct wiphy *wiphy = priv_to_wiphy(priv);
	struct ieee80211_channel *channel;
	u8 *ie;
	u8 ie_buf[46];
	u64 timestamp;
	s32 signal;
	u16 capability;
	u16 beacon_interval;
	int ie_len;
	int freq;
	int len;

	len = le16_to_cpu(bss->a.essid_len);

	/* Reconstruct SSID and bitrate IEs to pass up */
	ie_buf[0] = WLAN_EID_SSID;
	ie_buf[1] = len;
	memcpy(&ie_buf[2], bss->a.essid, len);

	ie = ie_buf + len + 2;
	ie_len = ie_buf[1] + 2;
	switch (priv->firmware_type) {
	case FIRMWARE_TYPE_SYMBOL:
		ie_len += symbol_build_supp_rates(ie, bss->s.rates);
		break;

	case FIRMWARE_TYPE_INTERSIL:
		ie_len += prism_build_supp_rates(ie, bss->p.rates);
		break;

	case FIRMWARE_TYPE_AGERE:
	default:
		break;
	}

	freq = ieee80211_dsss_chan_to_freq(le16_to_cpu(bss->a.channel));
	channel = ieee80211_get_channel(wiphy, freq);
	timestamp = 0;
	capability = le16_to_cpu(bss->a.capabilities);
	beacon_interval = le16_to_cpu(bss->a.beacon_interv);
	signal = SIGNAL_TO_MBM(le16_to_cpu(bss->a.level));

	cfg80211_inform_bss(wiphy, channel, bss->a.bssid, timestamp,
			    capability, beacon_interval, ie_buf, ie_len,
			    signal, GFP_KERNEL);
}

void orinoco_add_extscan_result(struct orinoco_private *priv,
				struct agere_ext_scan_info *bss,
				size_t len)
{
	struct wiphy *wiphy = priv_to_wiphy(priv);
	struct ieee80211_channel *channel;
	const u8 *ie;
	u64 timestamp;
	s32 signal;
	u16 capability;
	u16 beacon_interval;
	size_t ie_len;
	int chan, freq;

	ie_len = len - sizeof(*bss);
	ie = cfg80211_find_ie(WLAN_EID_DS_PARAMS, bss->data, ie_len);
	chan = ie ? ie[2] : 0;
	freq = ieee80211_dsss_chan_to_freq(chan);
	channel = ieee80211_get_channel(wiphy, freq);

	timestamp = le64_to_cpu(bss->timestamp);
	capability = le16_to_cpu(bss->capabilities);
	beacon_interval = le16_to_cpu(bss->beacon_interval);
	ie = bss->data;
	signal = SIGNAL_TO_MBM(bss->level);

	cfg80211_inform_bss(wiphy, channel, bss->bssid, timestamp,
			    capability, beacon_interval, ie, ie_len,
			    signal, GFP_KERNEL);
}

void orinoco_add_hostscan_results(struct orinoco_private *priv,
				  unsigned char *buf,
				  size_t len)
{
	int offset;		/* In the scan data */
	size_t atom_len;
	bool abort = false;

	switch (priv->firmware_type) {
	case FIRMWARE_TYPE_AGERE:
		atom_len = sizeof(struct agere_scan_apinfo);
		offset = 0;
		break;

	case FIRMWARE_TYPE_SYMBOL:
		/* Lack of documentation necessitates this hack.
		 * Different firmwares have 68 or 76 byte long atoms.
		 * We try modulo first.  If the length divides by both,
		 * we check what would be the channel in the second
		 * frame for a 68-byte atom.  76-byte atoms have 0 there.
		 * Valid channel cannot be 0.  */
		if (len % 76)
			atom_len = 68;
		else if (len % 68)
			atom_len = 76;
		else if (len >= 1292 && buf[68] == 0)
			atom_len = 76;
		else
			atom_len = 68;
		offset = 0;
		break;

	case FIRMWARE_TYPE_INTERSIL:
		offset = 4;
		if (priv->has_hostscan) {
			atom_len = le16_to_cpup((__le16 *)buf);
			/* Sanity check for atom_len */
			if (atom_len < sizeof(struct prism2_scan_apinfo)) {
				printk(KERN_ERR "%s: Invalid atom_len in scan "
				       "data: %zu\n", priv->ndev->name,
				       atom_len);
				abort = true;
				goto scan_abort;
			}
		} else
			atom_len = offsetof(struct prism2_scan_apinfo, atim);
		break;

	default:
		abort = true;
		goto scan_abort;
	}

	/* Check that we got an whole number of atoms */
	if ((len - offset) % atom_len) {
		printk(KERN_ERR "%s: Unexpected scan data length %zu, "
		       "atom_len %zu, offset %d\n", priv->ndev->name, len,
		       atom_len, offset);
		abort = true;
		goto scan_abort;
	}

	/* Process the entries one by one */
	for (; offset + atom_len <= len; offset += atom_len) {
		union hermes_scan_info *atom;

		atom = (union hermes_scan_info *) (buf + offset);

		orinoco_add_hostscan_result(priv, atom);
	}

 scan_abort:
	if (priv->scan_request) {
		cfg80211_scan_done(priv->scan_request, abort);
		priv->scan_request = NULL;
	}
}
