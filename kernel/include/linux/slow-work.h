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

/* Worker thread pool for slow items, such as filesystem lookups or mkdirs
 *
 * Copyright (C) 2008 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 *
 * See Documentation/slow-work.txt
 */

#ifndef _LINUX_SLOW_WORK_H
#define _LINUX_SLOW_WORK_H

#ifdef CONFIG_SLOW_WORK

#include <linux/sysctl.h>
#include <linux/timer.h>

struct slow_work;
#ifdef CONFIG_SLOW_WORK_DEBUG
struct seq_file;
#endif

/*
 * The operations used to support slow work items
 */
struct slow_work_ops {
	/* owner */
	struct module *owner;

	/* get a ref on a work item
	 * - return 0 if successful, -ve if not
	 */
	int (*get_ref)(struct slow_work *work);

	/* discard a ref to a work item */
	void (*put_ref)(struct slow_work *work);

	/* execute a work item */
	void (*execute)(struct slow_work *work);

#ifdef CONFIG_SLOW_WORK_DEBUG
	/* describe a work item for debugfs */
	void (*desc)(struct slow_work *work, struct seq_file *m);
#endif
};

/*
 * A slow work item
 * - A reference is held on the parent object by the thread pool when it is
 *   queued
 */
struct slow_work {
	struct module		*owner;	/* the owning module */
	unsigned long		flags;
#define SLOW_WORK_PENDING	0	/* item pending (further) execution */
#define SLOW_WORK_EXECUTING	1	/* item currently executing */
#define SLOW_WORK_ENQ_DEFERRED	2	/* item enqueue deferred */
#define SLOW_WORK_VERY_SLOW	3	/* item is very slow */
#define SLOW_WORK_CANCELLING	4	/* item is being cancelled, don't enqueue */
#define SLOW_WORK_DELAYED	5	/* item is struct delayed_slow_work with active timer */
	const struct slow_work_ops *ops; /* operations table for this item */
	struct list_head	link;	/* link in queue */
#ifdef CONFIG_SLOW_WORK_DEBUG
	struct timespec		mark;	/* jiffies at which queued or exec begun */
#endif
};

struct delayed_slow_work {
	struct slow_work	work;
	struct timer_list	timer;
};

/**
 * slow_work_init - Initialise a slow work item
 * @work: The work item to initialise
 * @ops: The operations to use to handle the slow work item
 *
 * Initialise a slow work item.
 */
static inline void slow_work_init(struct slow_work *work,
				  const struct slow_work_ops *ops)
{
	work->flags = 0;
	work->ops = ops;
	INIT_LIST_HEAD(&work->link);
}

/**
 * slow_work_init - Initialise a delayed slow work item
 * @work: The work item to initialise
 * @ops: The operations to use to handle the slow work item
 *
 * Initialise a delayed slow work item.
 */
static inline void delayed_slow_work_init(struct delayed_slow_work *dwork,
					  const struct slow_work_ops *ops)
{
	init_timer(&dwork->timer);
	slow_work_init(&dwork->work, ops);
}

/**
 * vslow_work_init - Initialise a very slow work item
 * @work: The work item to initialise
 * @ops: The operations to use to handle the slow work item
 *
 * Initialise a very slow work item.  This item will be restricted such that
 * only a certain number of the pool threads will be able to execute items of
 * this type.
 */
static inline void vslow_work_init(struct slow_work *work,
				   const struct slow_work_ops *ops)
{
	work->flags = 1 << SLOW_WORK_VERY_SLOW;
	work->ops = ops;
	INIT_LIST_HEAD(&work->link);
}

/**
 * slow_work_is_queued - Determine if a slow work item is on the work queue
 * work: The work item to test
 *
 * Determine if the specified slow-work item is on the work queue.  This
 * returns true if it is actually on the queue.
 *
 * If the item is executing and has been marked for requeue when execution
 * finishes, then false will be returned.
 *
 * Anyone wishing to wait for completion of execution can wait on the
 * SLOW_WORK_EXECUTING bit.
 */
static inline bool slow_work_is_queued(struct slow_work *work)
{
	unsigned long flags = work->flags;
	return flags & SLOW_WORK_PENDING && !(flags & SLOW_WORK_EXECUTING);
}

extern int slow_work_enqueue(struct slow_work *work);
extern void slow_work_cancel(struct slow_work *work);
extern int slow_work_register_user(struct module *owner);
extern void slow_work_unregister_user(struct module *owner);

extern int delayed_slow_work_enqueue(struct delayed_slow_work *dwork,
				     unsigned long delay);

static inline void delayed_slow_work_cancel(struct delayed_slow_work *dwork)
{
	slow_work_cancel(&dwork->work);
}

extern bool slow_work_sleep_till_thread_needed(struct slow_work *work,
					       signed long *_timeout);

#ifdef CONFIG_SYSCTL
extern ctl_table slow_work_sysctls[];
#endif

#endif /* CONFIG_SLOW_WORK */
#endif /* _LINUX_SLOW_WORK_H */
