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

#ifndef _PROBE_EVENT_H
#define _PROBE_EVENT_H

#include <stdbool.h>
#include "strlist.h"

extern bool probe_event_dry_run;

/* kprobe-tracer tracing point */
struct kprobe_trace_point {
	char		*symbol;	/* Base symbol */
	unsigned long	offset;		/* Offset from symbol */
	bool		retprobe;	/* Return probe flag */
};

/* kprobe-tracer tracing argument referencing offset */
struct kprobe_trace_arg_ref {
	struct kprobe_trace_arg_ref	*next;	/* Next reference */
	long				offset;	/* Offset value */
};

/* kprobe-tracer tracing argument */
struct kprobe_trace_arg {
	char				*name;	/* Argument name */
	char				*value;	/* Base value */
	char				*type;	/* Type name */
	struct kprobe_trace_arg_ref	*ref;	/* Referencing offset */
};

/* kprobe-tracer tracing event (point + arg) */
struct kprobe_trace_event {
	char				*event;	/* Event name */
	char				*group;	/* Group name */
	struct kprobe_trace_point	point;	/* Trace point */
	int				nargs;	/* Number of args */
	struct kprobe_trace_arg		*args;	/* Arguments */
};

/* Perf probe probing point */
struct perf_probe_point {
	char		*file;		/* File path */
	char		*function;	/* Function name */
	int		line;		/* Line number */
	bool		retprobe;	/* Return probe flag */
	char		*lazy_line;	/* Lazy matching pattern */
	unsigned long	offset;		/* Offset from function entry */
};

/* Perf probe probing argument field chain */
struct perf_probe_arg_field {
	struct perf_probe_arg_field	*next;	/* Next field */
	char				*name;	/* Name of the field */
	bool				ref;	/* Referencing flag */
};

/* Perf probe probing argument */
struct perf_probe_arg {
	char				*name;	/* Argument name */
	char				*var;	/* Variable name */
	char				*type;	/* Type name */
	struct perf_probe_arg_field	*field;	/* Structure fields */
};

/* Perf probe probing event (point + arg) */
struct perf_probe_event {
	char			*event;	/* Event name */
	char			*group;	/* Group name */
	struct perf_probe_point	point;	/* Probe point */
	int			nargs;	/* Number of arguments */
	struct perf_probe_arg	*args;	/* Arguments */
};


/* Line number container */
struct line_node {
	struct list_head	list;
	int			line;
};

/* Line range */
struct line_range {
	char			*file;		/* File name */
	char			*function;	/* Function name */
	int			start;		/* Start line number */
	int			end;		/* End line number */
	int			offset;		/* Start line offset */
	char			*path;		/* Real path name */
	struct list_head	line_list;	/* Visible lines */
};

/* Command string to events */
extern int parse_perf_probe_command(const char *cmd,
				    struct perf_probe_event *pev);
extern int parse_kprobe_trace_command(const char *cmd,
				      struct kprobe_trace_event *tev);

/* Events to command string */
extern char *synthesize_perf_probe_command(struct perf_probe_event *pev);
extern char *synthesize_kprobe_trace_command(struct kprobe_trace_event *tev);
extern int synthesize_perf_probe_arg(struct perf_probe_arg *pa, char *buf,
				     size_t len);

/* Check the perf_probe_event needs debuginfo */
extern bool perf_probe_event_need_dwarf(struct perf_probe_event *pev);

/* Convert from kprobe_trace_event to perf_probe_event */
extern int convert_to_perf_probe_event(struct kprobe_trace_event *tev,
				       struct perf_probe_event *pev);

/* Release event contents */
extern void clear_perf_probe_event(struct perf_probe_event *pev);
extern void clear_kprobe_trace_event(struct kprobe_trace_event *tev);

/* Command string to line-range */
extern int parse_line_range_desc(const char *cmd, struct line_range *lr);


extern int add_perf_probe_events(struct perf_probe_event *pevs, int npevs,
				 bool force_add, int max_probe_points);
extern int del_perf_probe_events(struct strlist *dellist);
extern int show_perf_probe_events(void);
extern int show_line_range(struct line_range *lr);


/* Maximum index number of event-name postfix */
#define MAX_EVENT_INDEX	1024

#endif /*_PROBE_EVENT_H */
