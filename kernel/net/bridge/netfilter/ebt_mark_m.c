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
 *  ebt_mark_m
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  July, 2002
 *
 */
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_mark_m.h>

static bool
ebt_mark_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct ebt_mark_m_info *info = par->matchinfo;

	if (info->bitmask & EBT_MARK_OR)
		return !!(skb->mark & info->mask) ^ info->invert;
	return ((skb->mark & info->mask) == info->mark) ^ info->invert;
}

static int ebt_mark_mt_check(const struct xt_mtchk_param *par)
{
	const struct ebt_mark_m_info *info = par->matchinfo;

	if (info->bitmask & ~EBT_MARK_MASK)
		return -EINVAL;
	if ((info->bitmask & EBT_MARK_OR) && (info->bitmask & EBT_MARK_AND))
		return -EINVAL;
	if (!info->bitmask)
		return -EINVAL;
	return 0;
}


#ifdef CONFIG_COMPAT
struct compat_ebt_mark_m_info {
	compat_ulong_t mark, mask;
	uint8_t invert, bitmask;
};

static void mark_mt_compat_from_user(void *dst, const void *src)
{
	const struct compat_ebt_mark_m_info *user = src;
	struct ebt_mark_m_info *kern = dst;

	kern->mark = user->mark;
	kern->mask = user->mask;
	kern->invert = user->invert;
	kern->bitmask = user->bitmask;
}

static int mark_mt_compat_to_user(void __user *dst, const void *src)
{
	struct compat_ebt_mark_m_info __user *user = dst;
	const struct ebt_mark_m_info *kern = src;

	if (put_user(kern->mark, &user->mark) ||
	    put_user(kern->mask, &user->mask) ||
	    put_user(kern->invert, &user->invert) ||
	    put_user(kern->bitmask, &user->bitmask))
		return -EFAULT;
	return 0;
}
#endif

static struct xt_match ebt_mark_mt_reg __read_mostly = {
	.name		= "mark_m",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_mark_mt,
	.checkentry	= ebt_mark_mt_check,
	.matchsize	= sizeof(struct ebt_mark_m_info),
#ifdef CONFIG_COMPAT
	.compatsize	= sizeof(struct compat_ebt_mark_m_info),
	.compat_from_user = mark_mt_compat_from_user,
	.compat_to_user	= mark_mt_compat_to_user,
#endif
	.me		= THIS_MODULE,
};

static int __init ebt_mark_m_init(void)
{
	return xt_register_match(&ebt_mark_mt_reg);
}

static void __exit ebt_mark_m_fini(void)
{
	xt_unregister_match(&ebt_mark_mt_reg);
}

module_init(ebt_mark_m_init);
module_exit(ebt_mark_m_fini);
MODULE_DESCRIPTION("Ebtables: Packet mark match");
MODULE_LICENSE("GPL");
