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
 *	xt_iprange - Netfilter module to match IP address ranges
 *
 *	(C) 2003 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *	(C) CC Computer Consultants GmbH, 2008
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_iprange.h>

static bool
iprange_mt4(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_iprange_mtinfo *info = par->matchinfo;
	const struct iphdr *iph = ip_hdr(skb);
	bool m;

	if (info->flags & IPRANGE_SRC) {
		m  = ntohl(iph->saddr) < ntohl(info->src_min.ip);
		m |= ntohl(iph->saddr) > ntohl(info->src_max.ip);
		m ^= !!(info->flags & IPRANGE_SRC_INV);
		if (m) {
			pr_debug("src IP %pI4 NOT in range %s%pI4-%pI4\n",
			         &iph->saddr,
			         (info->flags & IPRANGE_SRC_INV) ? "(INV) " : "",
			         &info->src_max.ip,
			         &info->src_max.ip);
			return false;
		}
	}
	if (info->flags & IPRANGE_DST) {
		m  = ntohl(iph->daddr) < ntohl(info->dst_min.ip);
		m |= ntohl(iph->daddr) > ntohl(info->dst_max.ip);
		m ^= !!(info->flags & IPRANGE_DST_INV);
		if (m) {
			pr_debug("dst IP %pI4 NOT in range %s%pI4-%pI4\n",
			         &iph->daddr,
			         (info->flags & IPRANGE_DST_INV) ? "(INV) " : "",
			         &info->dst_min.ip,
			         &info->dst_max.ip);
			return false;
		}
	}
	return true;
}

static inline int
iprange_ipv6_sub(const struct in6_addr *a, const struct in6_addr *b)
{
	unsigned int i;
	int r;

	for (i = 0; i < 4; ++i) {
		r = ntohl(a->s6_addr32[i]) - ntohl(b->s6_addr32[i]);
		if (r != 0)
			return r;
	}

	return 0;
}

static bool
iprange_mt6(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_iprange_mtinfo *info = par->matchinfo;
	const struct ipv6hdr *iph = ipv6_hdr(skb);
	bool m;

	if (info->flags & IPRANGE_SRC) {
		m  = iprange_ipv6_sub(&iph->saddr, &info->src_min.in6) < 0;
		m |= iprange_ipv6_sub(&iph->saddr, &info->src_max.in6) > 0;
		m ^= !!(info->flags & IPRANGE_SRC_INV);
		if (m)
			return false;
	}
	if (info->flags & IPRANGE_DST) {
		m  = iprange_ipv6_sub(&iph->daddr, &info->dst_min.in6) < 0;
		m |= iprange_ipv6_sub(&iph->daddr, &info->dst_max.in6) > 0;
		m ^= !!(info->flags & IPRANGE_DST_INV);
		if (m)
			return false;
	}
	return true;
}

static struct xt_match iprange_mt_reg[] __read_mostly = {
	{
		.name      = "iprange",
		.revision  = 1,
		.family    = NFPROTO_IPV4,
		.match     = iprange_mt4,
		.matchsize = sizeof(struct xt_iprange_mtinfo),
		.me        = THIS_MODULE,
	},
	{
		.name      = "iprange",
		.revision  = 1,
		.family    = NFPROTO_IPV6,
		.match     = iprange_mt6,
		.matchsize = sizeof(struct xt_iprange_mtinfo),
		.me        = THIS_MODULE,
	},
};

static int __init iprange_mt_init(void)
{
	return xt_register_matches(iprange_mt_reg, ARRAY_SIZE(iprange_mt_reg));
}

static void __exit iprange_mt_exit(void)
{
	xt_unregister_matches(iprange_mt_reg, ARRAY_SIZE(iprange_mt_reg));
}

module_init(iprange_mt_init);
module_exit(iprange_mt_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_AUTHOR("Jan Engelhardt <jengelh@medozas.de>");
MODULE_DESCRIPTION("Xtables: arbitrary IPv4 range matching");
MODULE_ALIAS("ipt_iprange");
MODULE_ALIAS("ip6t_iprange");
