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

#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/hrtimer.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/spinlock.h>


static int ss_get(void *data, u64 *val)
{
	ktime_t start, finish;
	int loops;
	int cont;
	DEFINE_RAW_SPINLOCK(ss_spin);

	loops = 1000000;
	cont = 1;

	start = ktime_get();

	while (cont) {
		raw_spin_lock(&ss_spin);
		loops--;
		if (loops == 0)
			cont = 0;
		raw_spin_unlock(&ss_spin);
	}

	finish = ktime_get();

	*val = ktime_us_delta(finish, start);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_ss, ss_get, NULL, "%llu\n");



struct spin_multi_state {
	raw_spinlock_t lock;
	atomic_t start_wait;
	atomic_t enter_wait;
	atomic_t exit_wait;
	int loops;
};

struct spin_multi_per_thread {
	struct spin_multi_state *state;
	ktime_t start;
};

static int multi_other(void *data)
{
	int loops;
	int cont;
	struct spin_multi_per_thread *pt = data;
	struct spin_multi_state *s = pt->state;

	loops = s->loops;
	cont = 1;

	atomic_dec(&s->enter_wait);

	while (atomic_read(&s->enter_wait))
		; /* spin */

	pt->start = ktime_get();

	atomic_dec(&s->start_wait);

	while (atomic_read(&s->start_wait))
		; /* spin */

	while (cont) {
		raw_spin_lock(&s->lock);
		loops--;
		if (loops == 0)
			cont = 0;
		raw_spin_unlock(&s->lock);
	}

	atomic_dec(&s->exit_wait);
	while (atomic_read(&s->exit_wait))
		; /* spin */
	return 0;
}

static int multi_get(void *data, u64 *val)
{
	ktime_t finish;
	struct spin_multi_state ms;
	struct spin_multi_per_thread t1, t2;

	ms.lock = __RAW_SPIN_LOCK_UNLOCKED("multi_get");
	ms.loops = 1000000;

	atomic_set(&ms.start_wait, 2);
	atomic_set(&ms.enter_wait, 2);
	atomic_set(&ms.exit_wait, 2);
	t1.state = &ms;
	t2.state = &ms;

	kthread_run(multi_other, &t2, "multi_get");

	multi_other(&t1);

	finish = ktime_get();

	*val = ktime_us_delta(finish, t1.start);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_multi, multi_get, NULL, "%llu\n");


extern struct dentry *mips_debugfs_dir;
static int __init spinlock_test(void)
{
	struct dentry *d;

	if (!mips_debugfs_dir)
		return -ENODEV;

	d = debugfs_create_file("spin_single", S_IRUGO,
				mips_debugfs_dir, NULL,
				&fops_ss);
	if (!d)
		return -ENOMEM;

	d = debugfs_create_file("spin_multi", S_IRUGO,
				mips_debugfs_dir, NULL,
				&fops_multi);
	if (!d)
		return -ENOMEM;

	return 0;
}
device_initcall(spinlock_test);
