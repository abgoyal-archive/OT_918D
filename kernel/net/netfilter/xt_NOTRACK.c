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

/* This is a module which is used for setting up fake conntracks
 * on packets so that they are not seen by the conntrack/NAT code.
 */
#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter/x_tables.h>
#include <net/netfilter/nf_conntrack.h>

MODULE_DESCRIPTION("Xtables: Disabling connection tracking for packets");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_NOTRACK");
MODULE_ALIAS("ip6t_NOTRACK");

static unsigned int
notrack_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	/* Previously seen (loopback)? Ignore. */
	if (skb->nfct != NULL)
		return XT_CONTINUE;

	/* Attach fake conntrack entry.
	   If there is a real ct entry correspondig to this packet,
	   it'll hang aroun till timing out. We don't deal with it
	   for performance reasons. JK */
	skb->nfct = &nf_conntrack_untracked.ct_general;
	skb->nfctinfo = IP_CT_NEW;
	nf_conntrack_get(skb->nfct);

	return XT_CONTINUE;
}

static struct xt_target notrack_tg_reg __read_mostly = {
	.name     = "NOTRACK",
	.revision = 0,
	.family   = NFPROTO_UNSPEC,
	.target   = notrack_tg,
	.table    = "raw",
	.me       = THIS_MODULE,
};

static int __init notrack_tg_init(void)
{
	return xt_register_target(&notrack_tg_reg);
}

static void __exit notrack_tg_exit(void)
{
	xt_unregister_target(&notrack_tg_reg);
}

module_init(notrack_tg_init);
module_exit(notrack_tg_exit);
