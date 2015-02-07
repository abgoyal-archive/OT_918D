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
 * IBM ASM Service Processor Device Driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) IBM Corporation, 2004
 *
 * Author: Max Asb�ck <amax@us.ibm.com>
 *
 */

#include <linux/sched.h>
#include <linux/slab.h>
#include "ibmasm.h"
#include "lowlevel.h"

static void exec_next_command(struct service_processor *sp);

static atomic_t command_count = ATOMIC_INIT(0);

struct command *ibmasm_new_command(struct service_processor *sp, size_t buffer_size)
{
	struct command *cmd;

	if (buffer_size > IBMASM_CMD_MAX_BUFFER_SIZE)
		return NULL;

	cmd = kzalloc(sizeof(struct command), GFP_KERNEL);
	if (cmd == NULL)
		return NULL;


	cmd->buffer = kzalloc(buffer_size, GFP_KERNEL);
	if (cmd->buffer == NULL) {
		kfree(cmd);
		return NULL;
	}
	cmd->buffer_size = buffer_size;

	kref_init(&cmd->kref);
	cmd->lock = &sp->lock;

	cmd->status = IBMASM_CMD_PENDING;
	init_waitqueue_head(&cmd->wait);
	INIT_LIST_HEAD(&cmd->queue_node);

	atomic_inc(&command_count);
	dbg("command count: %d\n", atomic_read(&command_count));

	return cmd;
}

void ibmasm_free_command(struct kref *kref)
{
	struct command *cmd = to_command(kref);

	list_del(&cmd->queue_node);
	atomic_dec(&command_count);
	dbg("command count: %d\n", atomic_read(&command_count));
	kfree(cmd->buffer);
	kfree(cmd);
}

static void enqueue_command(struct service_processor *sp, struct command *cmd)
{
	list_add_tail(&cmd->queue_node, &sp->command_queue);
}

static struct command *dequeue_command(struct service_processor *sp)
{
	struct command *cmd;
	struct list_head *next;

	if (list_empty(&sp->command_queue))
		return NULL;

	next = sp->command_queue.next;
	list_del_init(next);
	cmd = list_entry(next, struct command, queue_node);

	return cmd;
}

static inline void do_exec_command(struct service_processor *sp)
{
	char tsbuf[32];

	dbg("%s:%d at %s\n", __func__, __LINE__, get_timestamp(tsbuf));

	if (ibmasm_send_i2o_message(sp)) {
		sp->current_command->status = IBMASM_CMD_FAILED;
		wake_up(&sp->current_command->wait);
		command_put(sp->current_command);
		exec_next_command(sp);
	}
}

/**
 * exec_command
 * send a command to a service processor
 * Commands are executed sequentially. One command (sp->current_command)
 * is sent to the service processor. Once the interrupt handler gets a
 * message of type command_response, the message is copied into
 * the current commands buffer,
 */
void ibmasm_exec_command(struct service_processor *sp, struct command *cmd)
{
	unsigned long flags;
	char tsbuf[32];

	dbg("%s:%d at %s\n", __func__, __LINE__, get_timestamp(tsbuf));

	spin_lock_irqsave(&sp->lock, flags);

	if (!sp->current_command) {
		sp->current_command = cmd;
		command_get(sp->current_command);
		spin_unlock_irqrestore(&sp->lock, flags);
		do_exec_command(sp);
	} else {
		enqueue_command(sp, cmd);
		spin_unlock_irqrestore(&sp->lock, flags);
	}
}

static void exec_next_command(struct service_processor *sp)
{
	unsigned long flags;
	char tsbuf[32];

	dbg("%s:%d at %s\n", __func__, __LINE__, get_timestamp(tsbuf));

	spin_lock_irqsave(&sp->lock, flags);
	sp->current_command = dequeue_command(sp);
	if (sp->current_command) {
		command_get(sp->current_command);
		spin_unlock_irqrestore(&sp->lock, flags);
		do_exec_command(sp);
	} else {
		spin_unlock_irqrestore(&sp->lock, flags);
	}
}

/**
 * Sleep until a command has failed or a response has been received
 * and the command status been updated by the interrupt handler.
 * (see receive_response).
 */
void ibmasm_wait_for_response(struct command *cmd, int timeout)
{
	wait_event_interruptible_timeout(cmd->wait,
				cmd->status == IBMASM_CMD_COMPLETE ||
				cmd->status == IBMASM_CMD_FAILED,
				timeout * HZ);
}

/**
 * receive_command_response
 * called by the interrupt handler when a dot command of type command_response
 * was received.
 */
void ibmasm_receive_command_response(struct service_processor *sp, void *response, size_t size)
{
	struct command *cmd = sp->current_command;

	if (!sp->current_command)
		return;

	memcpy_fromio(cmd->buffer, response, min(size, cmd->buffer_size));
	cmd->status = IBMASM_CMD_COMPLETE;
	wake_up(&sp->current_command->wait);
	command_put(sp->current_command);
	exec_next_command(sp);
}
