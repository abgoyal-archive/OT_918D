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

#ifndef __MAC80211_DEBUGFS_KEY_H
#define __MAC80211_DEBUGFS_KEY_H

#ifdef CONFIG_MAC80211_DEBUGFS
void ieee80211_debugfs_key_add(struct ieee80211_key *key);
void ieee80211_debugfs_key_remove(struct ieee80211_key *key);
void ieee80211_debugfs_key_add_default(struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_key_remove_default(struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_key_add_mgmt_default(
	struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_key_remove_mgmt_default(
	struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_key_sta_del(struct ieee80211_key *key,
				   struct sta_info *sta);
#else
static inline void ieee80211_debugfs_key_add(struct ieee80211_key *key)
{}
static inline void ieee80211_debugfs_key_remove(struct ieee80211_key *key)
{}
static inline void ieee80211_debugfs_key_add_default(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_key_remove_default(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_key_add_mgmt_default(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_key_remove_mgmt_default(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_key_sta_del(struct ieee80211_key *key,
						 struct sta_info *sta)
{}
#endif

#endif /* __MAC80211_DEBUGFS_KEY_H */
