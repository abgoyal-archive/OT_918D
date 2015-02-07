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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef _TRACE_NET_H
#define _TRACE_NET_H

#include <linux/tracepoint.h>

struct sk_buff;
DECLARE_TRACE(net_dev_xmit,
	TP_PROTO(struct sk_buff *skb),
	TP_ARGS(skb));
DECLARE_TRACE(net_dev_receive,
	TP_PROTO(struct sk_buff *skb),
	TP_ARGS(skb));
DECLARE_TRACE(net_tcpv4_rcv,
	TP_PROTO(struct sk_buff *skb),
	TP_ARGS(skb));
DECLARE_TRACE(net_udpv4_rcv,
	TP_PROTO(struct sk_buff *skb),
	TP_ARGS(skb));

/*
 * Note these first 2 traces are actually in __napi_schedule and net_rx_action
 * respectively.  The former is in __napi_schedule because it uses at-most-once
 * logic and placing it in the calling routine (napi_schedule) would produce
 * countless trace events that were effectively  no-ops.  napi_poll is
 * implemented in net_rx_action, because thats where we do our polling on
 * devices.  The last trace point is in napi_complete, right where you would
 * think it would be.
 */
struct napi_struct;
DECLARE_TRACE(net_napi_schedule,
	TP_PROTO(struct napi_struct *n),
	TP_ARGS(n));
DECLARE_TRACE(net_napi_poll,
	TP_PROTO(struct napi_struct *n),
	TP_ARGS(n));
DECLARE_TRACE(net_napi_complete,
	TP_PROTO(struct napi_struct *n),
	TP_ARGS(n));

#endif
