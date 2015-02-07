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

/* Slow work private definitions
 *
 * Copyright (C) 2009 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#define SLOW_WORK_CULL_TIMEOUT (5 * HZ)	/* cull threads 5s after running out of
					 * things to do */
#define SLOW_WORK_OOM_TIMEOUT (5 * HZ)	/* can't start new threads for 5s after
					 * OOM */

#define SLOW_WORK_THREAD_LIMIT	255	/* abs maximum number of slow-work threads */

/*
 * slow-work.c
 */
#ifdef CONFIG_SLOW_WORK_DEBUG
extern struct slow_work *slow_work_execs[];
extern pid_t slow_work_pids[];
extern rwlock_t slow_work_execs_lock;
#endif

extern struct list_head slow_work_queue;
extern struct list_head vslow_work_queue;
extern spinlock_t slow_work_queue_lock;

/*
 * slow-work-debugfs.c
 */
#ifdef CONFIG_SLOW_WORK_DEBUG
extern const struct file_operations slow_work_runqueue_fops;

extern void slow_work_new_thread_desc(struct slow_work *, struct seq_file *);
#endif

/*
 * Helper functions
 */
static inline void slow_work_set_thread_pid(int id, pid_t pid)
{
#ifdef CONFIG_SLOW_WORK_DEBUG
	slow_work_pids[id] = pid;
#endif
}

static inline void slow_work_mark_time(struct slow_work *work)
{
#ifdef CONFIG_SLOW_WORK_DEBUG
	work->mark = CURRENT_TIME;
#endif
}

static inline void slow_work_begin_exec(int id, struct slow_work *work)
{
#ifdef CONFIG_SLOW_WORK_DEBUG
	slow_work_execs[id] = work;
#endif
}

static inline void slow_work_end_exec(int id, struct slow_work *work)
{
#ifdef CONFIG_SLOW_WORK_DEBUG
	write_lock(&slow_work_execs_lock);
	slow_work_execs[id] = NULL;
	write_unlock(&slow_work_execs_lock);
#endif
}
