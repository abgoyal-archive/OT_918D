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
 * Line6 Linux USB driver - 0.8.0
 *
 * Copyright (C) 2004-2009 Markus Grabner (grabner@icg.tugraz.at)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */

#include "driver.h"

#include <linux/slab.h>

#include "dumprequest.h"


/*
	Set "dump in progress" flag.
*/
void line6_dump_started(struct line6_dump_request *l6dr, int dest)
{
	l6dr->in_progress = dest;
}

/*
	Invalidate current channel, i.e., set "dump in progress" flag.
	Reading from the "dump" special file blocks until dump is completed.
*/
void line6_invalidate_current(struct line6_dump_request *l6dr)
{
	line6_dump_started(l6dr, LINE6_DUMP_CURRENT);
}

/*
	Clear "dump in progress" flag and notify waiting processes.
*/
void line6_dump_finished(struct line6_dump_request *l6dr)
{
	l6dr->in_progress = LINE6_DUMP_NONE;
	wake_up_interruptible(&l6dr->wait);
}

/*
	Send an asynchronous channel dump request.
*/
int line6_dump_request_async(struct line6_dump_request *l6dr,
			     struct usb_line6 *line6, int num)
{
	int ret;
	line6_invalidate_current(l6dr);
	ret = line6_send_raw_message_async(line6, l6dr->reqbufs[num].buffer,
					   l6dr->reqbufs[num].length);

	if (ret < 0)
		line6_dump_finished(l6dr);

	return ret;
}

/*
	Send an asynchronous dump request after a given interval.
*/
void line6_startup_delayed(struct line6_dump_request *l6dr, int seconds,
			   void (*function)(unsigned long), void *data)
{
	l6dr->timer.expires = jiffies + seconds * HZ;
	l6dr->timer.function = function;
	l6dr->timer.data = (unsigned long)data;
	add_timer(&l6dr->timer);
}

/*
	Wait for completion.
*/
int line6_wait_dump(struct line6_dump_request *l6dr, int nonblock)
{
	int retval = 0;
	DECLARE_WAITQUEUE(wait, current);
	add_wait_queue(&l6dr->wait, &wait);
	current->state = TASK_INTERRUPTIBLE;

	while (l6dr->in_progress) {
		if (nonblock) {
			retval = -EAGAIN;
			break;
		}

		if (signal_pending(current)) {
			retval = -ERESTARTSYS;
			break;
		} else
			schedule();
	}

	current->state = TASK_RUNNING;
	remove_wait_queue(&l6dr->wait, &wait);
	return retval;
}

/*
	Initialize dump request buffer.
*/
int line6_dumpreq_initbuf(struct line6_dump_request *l6dr, const void *buf,
			  size_t len, int num)
{
	l6dr->reqbufs[num].buffer = kmemdup(buf, len, GFP_KERNEL);
	if (l6dr->reqbufs[num].buffer == NULL)
		return -ENOMEM;
	l6dr->reqbufs[num].length = len;
	return 0;
}

/*
	Initialize dump request data structure (including one buffer).
*/
int line6_dumpreq_init(struct line6_dump_request *l6dr, const void *buf,
		       size_t len)
{
	int ret;
	ret = line6_dumpreq_initbuf(l6dr, buf, len, 0);
	if (ret < 0)
		return ret;
	init_waitqueue_head(&l6dr->wait);
	init_timer(&l6dr->timer);
	return 0;
}

/*
	Destruct dump request data structure.
*/
void line6_dumpreq_destructbuf(struct line6_dump_request *l6dr, int num)
{
	if (l6dr == NULL)
		return;
	if (l6dr->reqbufs[num].buffer == NULL)
		return;
	kfree(l6dr->reqbufs[num].buffer);
	l6dr->reqbufs[num].buffer = NULL;
}

/*
	Destruct dump request data structure.
*/
void line6_dumpreq_destruct(struct line6_dump_request *l6dr)
{
	if (l6dr->reqbufs[0].buffer == NULL)
		return;
	line6_dumpreq_destructbuf(l6dr, 0);
	l6dr->ok = 1;
	del_timer_sync(&l6dr->timer);
}
