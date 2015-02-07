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

#undef TRACE_SYSTEM
#define TRACE_SYSTEM workqueue

#if !defined(_TRACE_WORKQUEUE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_WORKQUEUE_H

#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/tracepoint.h>

DECLARE_EVENT_CLASS(workqueue,

	TP_PROTO(struct task_struct *wq_thread, struct work_struct *work),

	TP_ARGS(wq_thread, work),

	TP_STRUCT__entry(
		__array(char,		thread_comm,	TASK_COMM_LEN)
		__field(pid_t,		thread_pid)
		__field(work_func_t,	func)
	),

	TP_fast_assign(
		memcpy(__entry->thread_comm, wq_thread->comm, TASK_COMM_LEN);
		__entry->thread_pid	= wq_thread->pid;
		__entry->func		= work->func;
	),

	TP_printk("thread=%s:%d func=%pf", __entry->thread_comm,
		__entry->thread_pid, __entry->func)
);

DEFINE_EVENT(workqueue, workqueue_insertion,

	TP_PROTO(struct task_struct *wq_thread, struct work_struct *work),

	TP_ARGS(wq_thread, work)
);

DEFINE_EVENT(workqueue, workqueue_execution,

	TP_PROTO(struct task_struct *wq_thread, struct work_struct *work),

	TP_ARGS(wq_thread, work)
);

/* Trace the creation of one workqueue thread on a cpu */
TRACE_EVENT(workqueue_creation,

	TP_PROTO(struct task_struct *wq_thread, int cpu),

	TP_ARGS(wq_thread, cpu),

	TP_STRUCT__entry(
		__array(char,	thread_comm,	TASK_COMM_LEN)
		__field(pid_t,	thread_pid)
		__field(int,	cpu)
	),

	TP_fast_assign(
		memcpy(__entry->thread_comm, wq_thread->comm, TASK_COMM_LEN);
		__entry->thread_pid	= wq_thread->pid;
		__entry->cpu		= cpu;
	),

	TP_printk("thread=%s:%d cpu=%d", __entry->thread_comm,
		__entry->thread_pid, __entry->cpu)
);

TRACE_EVENT(workqueue_destruction,

	TP_PROTO(struct task_struct *wq_thread),

	TP_ARGS(wq_thread),

	TP_STRUCT__entry(
		__array(char,	thread_comm,	TASK_COMM_LEN)
		__field(pid_t,	thread_pid)
	),

	TP_fast_assign(
		memcpy(__entry->thread_comm, wq_thread->comm, TASK_COMM_LEN);
		__entry->thread_pid	= wq_thread->pid;
	),

	TP_printk("thread=%s:%d", __entry->thread_comm, __entry->thread_pid)
);

#endif /* _TRACE_WORKQUEUE_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
