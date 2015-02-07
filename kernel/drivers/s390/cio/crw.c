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
 *   Channel report handling code
 *
 *    Copyright IBM Corp. 2000,2009
 *    Author(s): Ingo Adlung <adlung@de.ibm.com>,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Cornelia Huck <cornelia.huck@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
 */

#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <asm/crw.h>

static DEFINE_MUTEX(crw_handler_mutex);
static crw_handler_t crw_handlers[NR_RSCS];
static atomic_t crw_nr_req = ATOMIC_INIT(0);
static DECLARE_WAIT_QUEUE_HEAD(crw_handler_wait_q);

/**
 * crw_register_handler() - register a channel report word handler
 * @rsc: reporting source code to handle
 * @handler: handler to be registered
 *
 * Returns %0 on success and a negative error value otherwise.
 */
int crw_register_handler(int rsc, crw_handler_t handler)
{
	int rc = 0;

	if ((rsc < 0) || (rsc >= NR_RSCS))
		return -EINVAL;
	mutex_lock(&crw_handler_mutex);
	if (crw_handlers[rsc])
		rc = -EBUSY;
	else
		crw_handlers[rsc] = handler;
	mutex_unlock(&crw_handler_mutex);
	return rc;
}

/**
 * crw_unregister_handler() - unregister a channel report word handler
 * @rsc: reporting source code to handle
 */
void crw_unregister_handler(int rsc)
{
	if ((rsc < 0) || (rsc >= NR_RSCS))
		return;
	mutex_lock(&crw_handler_mutex);
	crw_handlers[rsc] = NULL;
	mutex_unlock(&crw_handler_mutex);
}

/*
 * Retrieve CRWs and call function to handle event.
 */
static int crw_collect_info(void *unused)
{
	struct crw crw[2];
	int ccode, signal;
	unsigned int chain;

repeat:
	signal = wait_event_interruptible(crw_handler_wait_q,
					  atomic_read(&crw_nr_req) > 0);
	if (unlikely(signal))
		atomic_inc(&crw_nr_req);
	chain = 0;
	while (1) {
		crw_handler_t handler;

		if (unlikely(chain > 1)) {
			struct crw tmp_crw;

			printk(KERN_WARNING"%s: Code does not support more "
			       "than two chained crws; please report to "
			       "linux390@de.ibm.com!\n", __func__);
			ccode = stcrw(&tmp_crw);
			printk(KERN_WARNING"%s: crw reports slct=%d, oflw=%d, "
			       "chn=%d, rsc=%X, anc=%d, erc=%X, rsid=%X\n",
			       __func__, tmp_crw.slct, tmp_crw.oflw,
			       tmp_crw.chn, tmp_crw.rsc, tmp_crw.anc,
			       tmp_crw.erc, tmp_crw.rsid);
			printk(KERN_WARNING"%s: This was crw number %x in the "
			       "chain\n", __func__, chain);
			if (ccode != 0)
				break;
			chain = tmp_crw.chn ? chain + 1 : 0;
			continue;
		}
		ccode = stcrw(&crw[chain]);
		if (ccode != 0)
			break;
		printk(KERN_DEBUG "crw_info : CRW reports slct=%d, oflw=%d, "
		       "chn=%d, rsc=%X, anc=%d, erc=%X, rsid=%X\n",
		       crw[chain].slct, crw[chain].oflw, crw[chain].chn,
		       crw[chain].rsc, crw[chain].anc, crw[chain].erc,
		       crw[chain].rsid);
		/* Check for overflows. */
		if (crw[chain].oflw) {
			int i;

			pr_debug("%s: crw overflow detected!\n", __func__);
			mutex_lock(&crw_handler_mutex);
			for (i = 0; i < NR_RSCS; i++) {
				if (crw_handlers[i])
					crw_handlers[i](NULL, NULL, 1);
			}
			mutex_unlock(&crw_handler_mutex);
			chain = 0;
			continue;
		}
		if (crw[0].chn && !chain) {
			chain++;
			continue;
		}
		mutex_lock(&crw_handler_mutex);
		handler = crw_handlers[crw[chain].rsc];
		if (handler)
			handler(&crw[0], chain ? &crw[1] : NULL, 0);
		mutex_unlock(&crw_handler_mutex);
		/* chain is always 0 or 1 here. */
		chain = crw[chain].chn ? chain + 1 : 0;
	}
	if (atomic_dec_and_test(&crw_nr_req))
		wake_up(&crw_handler_wait_q);
	goto repeat;
	return 0;
}

void crw_handle_channel_report(void)
{
	atomic_inc(&crw_nr_req);
	wake_up(&crw_handler_wait_q);
}

void crw_wait_for_channel_report(void)
{
	crw_handle_channel_report();
	wait_event(crw_handler_wait_q, atomic_read(&crw_nr_req) == 0);
}

/*
 * Machine checks for the channel subsystem must be enabled
 * after the channel subsystem is initialized
 */
static int __init crw_machine_check_init(void)
{
	struct task_struct *task;

	task = kthread_run(crw_collect_info, NULL, "kmcheck");
	if (IS_ERR(task))
		return PTR_ERR(task);
	ctl_set_bit(14, 28);	/* enable channel report MCH */
	return 0;
}
device_initcall(crw_machine_check_init);
