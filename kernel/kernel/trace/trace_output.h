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

#ifndef __TRACE_EVENTS_H
#define __TRACE_EVENTS_H

#include <linux/trace_seq.h>
#include "trace.h"

extern enum print_line_t
trace_print_bprintk_msg_only(struct trace_iterator *iter);
extern enum print_line_t
trace_print_printk_msg_only(struct trace_iterator *iter);

extern int
seq_print_ip_sym(struct trace_seq *s, unsigned long ip,
		unsigned long sym_flags);
extern int seq_print_userip_objs(const struct userstack_entry *entry,
				 struct trace_seq *s, unsigned long sym_flags);
extern int seq_print_user_ip(struct trace_seq *s, struct mm_struct *mm,
			     unsigned long ip, unsigned long sym_flags);

extern int trace_print_context(struct trace_iterator *iter);
extern int trace_print_lat_context(struct trace_iterator *iter);

extern void trace_event_read_lock(void);
extern void trace_event_read_unlock(void);
extern struct trace_event *ftrace_find_event(int type);

extern enum print_line_t trace_nop_print(struct trace_iterator *iter,
					 int flags, struct trace_event *event);
extern int
trace_print_lat_fmt(struct trace_seq *s, struct trace_entry *entry);

/* used by module unregistering */
extern int __unregister_ftrace_event(struct trace_event *event);
extern struct rw_semaphore trace_event_mutex;

#define MAX_MEMHEX_BYTES	8
#define HEX_CHARS		(MAX_MEMHEX_BYTES*2 + 1)

#define SEQ_PUT_FIELD_RET(s, x)				\
do {							\
	if (!trace_seq_putmem(s, &(x), sizeof(x)))	\
		return TRACE_TYPE_PARTIAL_LINE;		\
} while (0)

#define SEQ_PUT_HEX_FIELD_RET(s, x)			\
do {							\
	BUILD_BUG_ON(sizeof(x) > MAX_MEMHEX_BYTES);	\
	if (!trace_seq_putmem_hex(s, &(x), sizeof(x)))	\
		return TRACE_TYPE_PARTIAL_LINE;		\
} while (0)

#endif

