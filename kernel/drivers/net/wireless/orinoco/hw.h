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

/* Encapsulate basic setting changes on Hermes hardware
 *
 * See copyright notice in main.c
 */
#ifndef _ORINOCO_HW_H_
#define _ORINOCO_HW_H_

#include <linux/types.h>
#include <linux/wireless.h>
#include <net/cfg80211.h>

/* Hardware BAPs */
#define USER_BAP 0
#define IRQ_BAP  1

/* WEP key sizes */
#define SMALL_KEY_SIZE 5
#define LARGE_KEY_SIZE 13

/* Number of supported channels */
#define NUM_CHANNELS 14

/* Forward declarations */
struct orinoco_private;

int determine_fw_capabilities(struct orinoco_private *priv, char *fw_name,
			      size_t fw_name_len, u32 *hw_ver);
int orinoco_hw_read_card_settings(struct orinoco_private *priv, u8 *dev_addr);
int orinoco_hw_allocate_fid(struct orinoco_private *priv);
int orinoco_get_bitratemode(int bitrate, int automatic);
void orinoco_get_ratemode_cfg(int ratemode, int *bitrate, int *automatic);

int orinoco_hw_program_rids(struct orinoco_private *priv);
int orinoco_hw_get_tkip_iv(struct orinoco_private *priv, int key, u8 *tsc);
int __orinoco_hw_set_bitrate(struct orinoco_private *priv);
int orinoco_hw_get_act_bitrate(struct orinoco_private *priv, int *bitrate);
int __orinoco_hw_set_wap(struct orinoco_private *priv);
int __orinoco_hw_setup_wepkeys(struct orinoco_private *priv);
int __orinoco_hw_setup_enc(struct orinoco_private *priv);
int __orinoco_hw_set_tkip_key(struct orinoco_private *priv, int key_idx,
			      int set_tx, u8 *key, u8 *rsc, size_t rsc_len,
			      u8 *tsc, size_t tsc_len);
int orinoco_clear_tkip_key(struct orinoco_private *priv, int key_idx);
int __orinoco_hw_set_multicast_list(struct orinoco_private *priv,
				    struct net_device *dev,
				    int mc_count, int promisc);
int orinoco_hw_get_essid(struct orinoco_private *priv, int *active,
			 char buf[IW_ESSID_MAX_SIZE+1]);
int orinoco_hw_get_freq(struct orinoco_private *priv);
int orinoco_hw_get_bitratelist(struct orinoco_private *priv,
			       int *numrates, s32 *rates, int max);
int orinoco_hw_trigger_scan(struct orinoco_private *priv,
			    const struct cfg80211_ssid *ssid);
int orinoco_hw_disassociate(struct orinoco_private *priv,
			    u8 *addr, u16 reason_code);
int orinoco_hw_get_current_bssid(struct orinoco_private *priv,
				 u8 *addr);

#endif /* _ORINOCO_HW_H_ */
