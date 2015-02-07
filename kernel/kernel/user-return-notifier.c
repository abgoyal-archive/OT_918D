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


#include <linux/user-return-notifier.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/module.h>

static DEFINE_PER_CPU(struct hlist_head, return_notifier_list);

/*
 * Request a notification when the current cpu returns to userspace.  Must be
 * called in atomic context.  The notifier will also be called in atomic
 * context.
 */
void user_return_notifier_register(struct user_return_notifier *urn)
{
	set_tsk_thread_flag(current, TIF_USER_RETURN_NOTIFY);
	hlist_add_head(&urn->link, &__get_cpu_var(return_notifier_list));
}
EXPORT_SYMBOL_GPL(user_return_notifier_register);

/*
 * Removes a registered user return notifier.  Must be called from atomic
 * context, and from the same cpu registration occured in.
 */
void user_return_notifier_unregister(struct user_return_notifier *urn)
{
	hlist_del(&urn->link);
	if (hlist_empty(&__get_cpu_var(return_notifier_list)))
		clear_tsk_thread_flag(current, TIF_USER_RETURN_NOTIFY);
}
EXPORT_SYMBOL_GPL(user_return_notifier_unregister);

/* Calls registered user return notifiers */
void fire_user_return_notifiers(void)
{
	struct user_return_notifier *urn;
	struct hlist_node *tmp1, *tmp2;
	struct hlist_head *head;

	head = &get_cpu_var(return_notifier_list);
	hlist_for_each_entry_safe(urn, tmp1, tmp2, head, link)
		urn->on_user_return(urn);
	put_cpu_var(return_notifier_list);
}
