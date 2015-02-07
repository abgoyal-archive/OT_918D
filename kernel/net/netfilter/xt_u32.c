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
 *	xt_u32 - kernel module to match u32 packet content
 *
 *	Original author: Don Cohen <don@isis.cs3-inc.com>
 *	(C) CC Computer Consultants GmbH, 2007
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_u32.h>

static bool u32_match_it(const struct xt_u32 *data,
			 const struct sk_buff *skb)
{
	const struct xt_u32_test *ct;
	unsigned int testind;
	unsigned int nnums;
	unsigned int nvals;
	unsigned int i;
	__be32 n;
	u_int32_t pos;
	u_int32_t val;
	u_int32_t at;

	/*
	 * Small example: "0 >> 28 == 4 && 8 & 0xFF0000 >> 16 = 6, 17"
	 * (=IPv4 and (TCP or UDP)). Outer loop runs over the "&&" operands.
	 */
	for (testind = 0; testind < data->ntests; ++testind) {
		ct  = &data->tests[testind];
		at  = 0;
		pos = ct->location[0].number;

		if (skb->len < 4 || pos > skb->len - 4)
			return false;

		if (skb_copy_bits(skb, pos, &n, sizeof(n)) < 0)
			BUG();
		val   = ntohl(n);
		nnums = ct->nnums;

		/* Inner loop runs over "&", "<<", ">>" and "@" operands */
		for (i = 1; i < nnums; ++i) {
			u_int32_t number = ct->location[i].number;
			switch (ct->location[i].nextop) {
			case XT_U32_AND:
				val &= number;
				break;
			case XT_U32_LEFTSH:
				val <<= number;
				break;
			case XT_U32_RIGHTSH:
				val >>= number;
				break;
			case XT_U32_AT:
				if (at + val < at)
					return false;
				at += val;
				pos = number;
				if (at + 4 < at || skb->len < at + 4 ||
				    pos > skb->len - at - 4)
					return false;

				if (skb_copy_bits(skb, at + pos, &n,
						    sizeof(n)) < 0)
					BUG();
				val = ntohl(n);
				break;
			}
		}

		/* Run over the "," and ":" operands */
		nvals = ct->nvalues;
		for (i = 0; i < nvals; ++i)
			if (ct->value[i].min <= val && val <= ct->value[i].max)
				break;

		if (i >= ct->nvalues)
			return false;
	}

	return true;
}

static bool u32_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_u32 *data = par->matchinfo;
	bool ret;

	ret = u32_match_it(data, skb);
	return ret ^ data->invert;
}

static struct xt_match xt_u32_mt_reg __read_mostly = {
	.name       = "u32",
	.revision   = 0,
	.family     = NFPROTO_UNSPEC,
	.match      = u32_mt,
	.matchsize  = sizeof(struct xt_u32),
	.me         = THIS_MODULE,
};

static int __init u32_mt_init(void)
{
	return xt_register_match(&xt_u32_mt_reg);
}

static void __exit u32_mt_exit(void)
{
	xt_unregister_match(&xt_u32_mt_reg);
}

module_init(u32_mt_init);
module_exit(u32_mt_exit);
MODULE_AUTHOR("Jan Engelhardt <jengelh@medozas.de>");
MODULE_DESCRIPTION("Xtables: arbitrary byte matching");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_u32");
MODULE_ALIAS("ip6t_u32");
