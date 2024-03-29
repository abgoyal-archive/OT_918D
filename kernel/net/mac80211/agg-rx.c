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
 * HT handling
 *
 * Copyright 2003, Jouni Malinen <jkmaline@cc.hut.fi>
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007, Michael Wu <flamingice@sourmilk.net>
 * Copyright 2007-2008, Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ieee80211.h>
#include <linux/slab.h>
#include <net/mac80211.h>
#include "ieee80211_i.h"
#include "driver-ops.h"

static void ___ieee80211_stop_rx_ba_session(struct sta_info *sta, u16 tid,
					    u16 initiator, u16 reason,
					    bool from_timer)
{
	struct ieee80211_local *local = sta->local;
	struct tid_ampdu_rx *tid_rx;
	int i;

	spin_lock_bh(&sta->lock);

	/* check if TID is in operational state */
	if (!sta->ampdu_mlme.tid_active_rx[tid]) {
		spin_unlock_bh(&sta->lock);
		return;
	}

	sta->ampdu_mlme.tid_active_rx[tid] = false;

	tid_rx = sta->ampdu_mlme.tid_rx[tid];

#ifdef CONFIG_MAC80211_HT_DEBUG
	printk(KERN_DEBUG "Rx BA session stop requested for %pM tid %u\n",
	       sta->sta.addr, tid);
#endif /* CONFIG_MAC80211_HT_DEBUG */

	if (drv_ampdu_action(local, sta->sdata, IEEE80211_AMPDU_RX_STOP,
			     &sta->sta, tid, NULL))
		printk(KERN_DEBUG "HW problem - can not stop rx "
				"aggregation for tid %d\n", tid);

	/* check if this is a self generated aggregation halt */
	if (initiator == WLAN_BACK_RECIPIENT)
		ieee80211_send_delba(sta->sdata, sta->sta.addr,
				     tid, 0, reason);

	/* free the reordering buffer */
	for (i = 0; i < tid_rx->buf_size; i++) {
		if (tid_rx->reorder_buf[i]) {
			/* release the reordered frames */
			dev_kfree_skb(tid_rx->reorder_buf[i]);
			tid_rx->stored_mpdu_num--;
			tid_rx->reorder_buf[i] = NULL;
		}
	}

	/* free resources */
	kfree(tid_rx->reorder_buf);
	kfree(tid_rx->reorder_time);
	sta->ampdu_mlme.tid_rx[tid] = NULL;

	spin_unlock_bh(&sta->lock);

	if (!from_timer)
		del_timer_sync(&tid_rx->session_timer);
	kfree(tid_rx);
}

void __ieee80211_stop_rx_ba_session(struct sta_info *sta, u16 tid,
				    u16 initiator, u16 reason)
{
	___ieee80211_stop_rx_ba_session(sta, tid, initiator, reason, false);
}

/*
 * After accepting the AddBA Request we activated a timer,
 * resetting it after each frame that arrives from the originator.
 */
static void sta_rx_agg_session_timer_expired(unsigned long data)
{
	/* not an elegant detour, but there is no choice as the timer passes
	 * only one argument, and various sta_info are needed here, so init
	 * flow in sta_info_create gives the TID as data, while the timer_to_id
	 * array gives the sta through container_of */
	u8 *ptid = (u8 *)data;
	u8 *timer_to_id = ptid - *ptid;
	struct sta_info *sta = container_of(timer_to_id, struct sta_info,
					 timer_to_tid[0]);

#ifdef CONFIG_MAC80211_HT_DEBUG
	printk(KERN_DEBUG "rx session timer expired on tid %d\n", (u16)*ptid);
#endif
	___ieee80211_stop_rx_ba_session(sta, *ptid, WLAN_BACK_RECIPIENT,
					WLAN_REASON_QSTA_TIMEOUT, true);
}

static void ieee80211_send_addba_resp(struct ieee80211_sub_if_data *sdata, u8 *da, u16 tid,
				      u8 dialog_token, u16 status, u16 policy,
				      u16 buf_size, u16 timeout)
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt;
	u16 capab;

	skb = dev_alloc_skb(sizeof(*mgmt) + local->hw.extra_tx_headroom);

	if (!skb) {
		printk(KERN_DEBUG "%s: failed to allocate buffer "
		       "for addba resp frame\n", sdata->name);
		return;
	}

	skb_reserve(skb, local->hw.extra_tx_headroom);
	mgmt = (struct ieee80211_mgmt *) skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, da, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	if (sdata->vif.type == NL80211_IFTYPE_AP ||
	    sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
		memcpy(mgmt->bssid, sdata->vif.addr, ETH_ALEN);
	else if (sdata->vif.type == NL80211_IFTYPE_STATION)
		memcpy(mgmt->bssid, sdata->u.mgd.bssid, ETH_ALEN);

	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_ACTION);

	skb_put(skb, 1 + sizeof(mgmt->u.action.u.addba_resp));
	mgmt->u.action.category = WLAN_CATEGORY_BACK;
	mgmt->u.action.u.addba_resp.action_code = WLAN_ACTION_ADDBA_RESP;
	mgmt->u.action.u.addba_resp.dialog_token = dialog_token;

	capab = (u16)(policy << 1);	/* bit 1 aggregation policy */
	capab |= (u16)(tid << 2); 	/* bit 5:2 TID number */
	capab |= (u16)(buf_size << 6);	/* bit 15:6 max size of aggregation */

	mgmt->u.action.u.addba_resp.capab = cpu_to_le16(capab);
	mgmt->u.action.u.addba_resp.timeout = cpu_to_le16(timeout);
	mgmt->u.action.u.addba_resp.status = cpu_to_le16(status);

	ieee80211_tx_skb(sdata, skb);
}

void ieee80211_process_addba_request(struct ieee80211_local *local,
				     struct sta_info *sta,
				     struct ieee80211_mgmt *mgmt,
				     size_t len)
{
	struct ieee80211_hw *hw = &local->hw;
	struct ieee80211_conf *conf = &hw->conf;
	struct tid_ampdu_rx *tid_agg_rx;
	u16 capab, tid, timeout, ba_policy, buf_size, start_seq_num, status;
	u8 dialog_token;
	int ret = -EOPNOTSUPP;

	/* extract session parameters from addba request frame */
	dialog_token = mgmt->u.action.u.addba_req.dialog_token;
	timeout = le16_to_cpu(mgmt->u.action.u.addba_req.timeout);
	start_seq_num =
		le16_to_cpu(mgmt->u.action.u.addba_req.start_seq_num) >> 4;

	capab = le16_to_cpu(mgmt->u.action.u.addba_req.capab);
	ba_policy = (capab & IEEE80211_ADDBA_PARAM_POLICY_MASK) >> 1;
	tid = (capab & IEEE80211_ADDBA_PARAM_TID_MASK) >> 2;
	buf_size = (capab & IEEE80211_ADDBA_PARAM_BUF_SIZE_MASK) >> 6;

	status = WLAN_STATUS_REQUEST_DECLINED;

	if (test_sta_flags(sta, WLAN_STA_BLOCK_BA)) {
#ifdef CONFIG_MAC80211_HT_DEBUG
		printk(KERN_DEBUG "Suspend in progress. "
		       "Denying ADDBA request\n");
#endif
		goto end_no_lock;
	}

	/* sanity check for incoming parameters:
	 * check if configuration can support the BA policy
	 * and if buffer size does not exceeds max value */
	/* XXX: check own ht delayed BA capability?? */
	if (((ba_policy != 1) &&
	     (!(sta->sta.ht_cap.cap & IEEE80211_HT_CAP_DELAY_BA))) ||
	    (buf_size > IEEE80211_MAX_AMPDU_BUF)) {
		status = WLAN_STATUS_INVALID_QOS_PARAM;
#ifdef CONFIG_MAC80211_HT_DEBUG
		if (net_ratelimit())
			printk(KERN_DEBUG "AddBA Req with bad params from "
				"%pM on tid %u. policy %d, buffer size %d\n",
				mgmt->sa, tid, ba_policy,
				buf_size);
#endif /* CONFIG_MAC80211_HT_DEBUG */
		goto end_no_lock;
	}
	/* determine default buffer size */
	if (buf_size == 0) {
		struct ieee80211_supported_band *sband;

		sband = local->hw.wiphy->bands[conf->channel->band];
		buf_size = IEEE80211_MIN_AMPDU_BUF;
		buf_size = buf_size << sband->ht_cap.ampdu_factor;
	}


	/* examine state machine */
	spin_lock_bh(&sta->lock);

	if (sta->ampdu_mlme.tid_active_rx[tid]) {
#ifdef CONFIG_MAC80211_HT_DEBUG
		if (net_ratelimit())
			printk(KERN_DEBUG "unexpected AddBA Req from "
				"%pM on tid %u\n",
				mgmt->sa, tid);
#endif /* CONFIG_MAC80211_HT_DEBUG */
		goto end;
	}

	/* prepare A-MPDU MLME for Rx aggregation */
	sta->ampdu_mlme.tid_rx[tid] =
			kmalloc(sizeof(struct tid_ampdu_rx), GFP_ATOMIC);
	if (!sta->ampdu_mlme.tid_rx[tid]) {
#ifdef CONFIG_MAC80211_HT_DEBUG
		if (net_ratelimit())
			printk(KERN_ERR "allocate rx mlme to tid %d failed\n",
					tid);
#endif
		goto end;
	}
	/* rx timer */
	sta->ampdu_mlme.tid_rx[tid]->session_timer.function =
				sta_rx_agg_session_timer_expired;
	sta->ampdu_mlme.tid_rx[tid]->session_timer.data =
				(unsigned long)&sta->timer_to_tid[tid];
	init_timer(&sta->ampdu_mlme.tid_rx[tid]->session_timer);

	tid_agg_rx = sta->ampdu_mlme.tid_rx[tid];

	/* prepare reordering buffer */
	tid_agg_rx->reorder_buf =
		kcalloc(buf_size, sizeof(struct sk_buff *), GFP_ATOMIC);
	tid_agg_rx->reorder_time =
		kcalloc(buf_size, sizeof(unsigned long), GFP_ATOMIC);
	if (!tid_agg_rx->reorder_buf || !tid_agg_rx->reorder_time) {
#ifdef CONFIG_MAC80211_HT_DEBUG
		if (net_ratelimit())
			printk(KERN_ERR "can not allocate reordering buffer "
			       "to tid %d\n", tid);
#endif
		kfree(tid_agg_rx->reorder_buf);
		kfree(tid_agg_rx->reorder_time);
		kfree(sta->ampdu_mlme.tid_rx[tid]);
		sta->ampdu_mlme.tid_rx[tid] = NULL;
		goto end;
	}

	ret = drv_ampdu_action(local, sta->sdata, IEEE80211_AMPDU_RX_START,
			       &sta->sta, tid, &start_seq_num);
#ifdef CONFIG_MAC80211_HT_DEBUG
	printk(KERN_DEBUG "Rx A-MPDU request on tid %d result %d\n", tid, ret);
#endif /* CONFIG_MAC80211_HT_DEBUG */

	if (ret) {
		kfree(tid_agg_rx->reorder_buf);
		kfree(tid_agg_rx);
		sta->ampdu_mlme.tid_rx[tid] = NULL;
		goto end;
	}

	/* change state and send addba resp */
	sta->ampdu_mlme.tid_active_rx[tid] = true;
	tid_agg_rx->dialog_token = dialog_token;
	tid_agg_rx->ssn = start_seq_num;
	tid_agg_rx->head_seq_num = start_seq_num;
	tid_agg_rx->buf_size = buf_size;
	tid_agg_rx->timeout = timeout;
	tid_agg_rx->stored_mpdu_num = 0;
	status = WLAN_STATUS_SUCCESS;
end:
	spin_unlock_bh(&sta->lock);

end_no_lock:
	ieee80211_send_addba_resp(sta->sdata, sta->sta.addr, tid,
				  dialog_token, status, 1, buf_size, timeout);
}
