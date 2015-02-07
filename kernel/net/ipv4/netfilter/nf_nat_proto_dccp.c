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
 * DCCP NAT protocol helper
 *
 * Copyright (c) 2005, 2006. 2008 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/dccp.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_protocol.h>

static u_int16_t dccp_port_rover;

static bool
dccp_unique_tuple(struct nf_conntrack_tuple *tuple,
		  const struct nf_nat_range *range,
		  enum nf_nat_manip_type maniptype,
		  const struct nf_conn *ct)
{
	return nf_nat_proto_unique_tuple(tuple, range, maniptype, ct,
					 &dccp_port_rover);
}

static bool
dccp_manip_pkt(struct sk_buff *skb,
	       unsigned int iphdroff,
	       const struct nf_conntrack_tuple *tuple,
	       enum nf_nat_manip_type maniptype)
{
	const struct iphdr *iph = (const void *)(skb->data + iphdroff);
	struct dccp_hdr *hdr;
	unsigned int hdroff = iphdroff + iph->ihl * 4;
	__be32 oldip, newip;
	__be16 *portptr, oldport, newport;
	int hdrsize = 8; /* DCCP connection tracking guarantees this much */

	if (skb->len >= hdroff + sizeof(struct dccp_hdr))
		hdrsize = sizeof(struct dccp_hdr);

	if (!skb_make_writable(skb, hdroff + hdrsize))
		return false;

	iph = (struct iphdr *)(skb->data + iphdroff);
	hdr = (struct dccp_hdr *)(skb->data + hdroff);

	if (maniptype == IP_NAT_MANIP_SRC) {
		oldip = iph->saddr;
		newip = tuple->src.u3.ip;
		newport = tuple->src.u.dccp.port;
		portptr = &hdr->dccph_sport;
	} else {
		oldip = iph->daddr;
		newip = tuple->dst.u3.ip;
		newport = tuple->dst.u.dccp.port;
		portptr = &hdr->dccph_dport;
	}

	oldport = *portptr;
	*portptr = newport;

	if (hdrsize < sizeof(*hdr))
		return true;

	inet_proto_csum_replace4(&hdr->dccph_checksum, skb, oldip, newip, 1);
	inet_proto_csum_replace2(&hdr->dccph_checksum, skb, oldport, newport,
				 0);
	return true;
}

static const struct nf_nat_protocol nf_nat_protocol_dccp = {
	.protonum		= IPPROTO_DCCP,
	.me			= THIS_MODULE,
	.manip_pkt		= dccp_manip_pkt,
	.in_range		= nf_nat_proto_in_range,
	.unique_tuple		= dccp_unique_tuple,
#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
	.range_to_nlattr	= nf_nat_proto_range_to_nlattr,
	.nlattr_to_range	= nf_nat_proto_nlattr_to_range,
#endif
};

static int __init nf_nat_proto_dccp_init(void)
{
	return nf_nat_protocol_register(&nf_nat_protocol_dccp);
}

static void __exit nf_nat_proto_dccp_fini(void)
{
	nf_nat_protocol_unregister(&nf_nat_protocol_dccp);
}

module_init(nf_nat_proto_dccp_init);
module_exit(nf_nat_proto_dccp_fini);

MODULE_AUTHOR("Patrick McHardy <kaber@trash.net>");
MODULE_DESCRIPTION("DCCP NAT protocol helper");
MODULE_LICENSE("GPL");
