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
 * Dummy LL interface for the Gigaset driver
 *
 * Copyright (c) 2009 by Tilman Schmidt <tilman@imap.cc>.
 *
 * =====================================================================
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation; either version 2 of
 *	the License, or (at your option) any later version.
 * =====================================================================
 */

#include "gigaset.h"

void gigaset_skb_sent(struct bc_state *bcs, struct sk_buff *skb)
{
}
EXPORT_SYMBOL_GPL(gigaset_skb_sent);

void gigaset_skb_rcvd(struct bc_state *bcs, struct sk_buff *skb)
{
}
EXPORT_SYMBOL_GPL(gigaset_skb_rcvd);

void gigaset_isdn_rcv_err(struct bc_state *bcs)
{
}
EXPORT_SYMBOL_GPL(gigaset_isdn_rcv_err);

int gigaset_isdn_icall(struct at_state_t *at_state)
{
	return ICALL_IGNORE;
}

void gigaset_isdn_connD(struct bc_state *bcs)
{
}

void gigaset_isdn_hupD(struct bc_state *bcs)
{
}

void gigaset_isdn_connB(struct bc_state *bcs)
{
}

void gigaset_isdn_hupB(struct bc_state *bcs)
{
}

void gigaset_isdn_start(struct cardstate *cs)
{
}

void gigaset_isdn_stop(struct cardstate *cs)
{
}

int gigaset_isdn_regdev(struct cardstate *cs, const char *isdnid)
{
	return 1;
}

void gigaset_isdn_unregdev(struct cardstate *cs)
{
}

void gigaset_isdn_regdrv(void)
{
	pr_info("no ISDN subsystem interface\n");
}

void gigaset_isdn_unregdrv(void)
{
}
