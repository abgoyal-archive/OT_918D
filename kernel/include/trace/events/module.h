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
#define TRACE_SYSTEM module

#if !defined(_TRACE_MODULE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_MODULE_H

#include <linux/tracepoint.h>

#ifdef CONFIG_MODULES

struct module;

#define show_module_flags(flags) __print_flags(flags, "",	\
	{ (1UL << TAINT_PROPRIETARY_MODULE),	"P" },		\
	{ (1UL << TAINT_FORCED_MODULE),		"F" },		\
	{ (1UL << TAINT_CRAP),			"C" })

TRACE_EVENT(module_load,

	TP_PROTO(struct module *mod),

	TP_ARGS(mod),

	TP_STRUCT__entry(
		__field(	unsigned int,	taints		)
		__string(	name,		mod->name	)
	),

	TP_fast_assign(
		__entry->taints = mod->taints;
		__assign_str(name, mod->name);
	),

	TP_printk("%s %s", __get_str(name), show_module_flags(__entry->taints))
);

TRACE_EVENT(module_free,

	TP_PROTO(struct module *mod),

	TP_ARGS(mod),

	TP_STRUCT__entry(
		__string(	name,		mod->name	)
	),

	TP_fast_assign(
		__assign_str(name, mod->name);
	),

	TP_printk("%s", __get_str(name))
);

#ifdef CONFIG_MODULE_UNLOAD
/* trace_module_get/put are only used if CONFIG_MODULE_UNLOAD is defined */

DECLARE_EVENT_CLASS(module_refcnt,

	TP_PROTO(struct module *mod, unsigned long ip),

	TP_ARGS(mod, ip),

	TP_STRUCT__entry(
		__field(	unsigned long,	ip		)
		__field(	int,		refcnt		)
		__string(	name,		mod->name	)
	),

	TP_fast_assign(
		__entry->ip	= ip;
		__entry->refcnt	= __this_cpu_read(mod->refptr->incs) + __this_cpu_read(mod->refptr->decs);
		__assign_str(name, mod->name);
	),

	TP_printk("%s call_site=%pf refcnt=%d",
		  __get_str(name), (void *)__entry->ip, __entry->refcnt)
);

DEFINE_EVENT(module_refcnt, module_get,

	TP_PROTO(struct module *mod, unsigned long ip),

	TP_ARGS(mod, ip)
);

DEFINE_EVENT(module_refcnt, module_put,

	TP_PROTO(struct module *mod, unsigned long ip),

	TP_ARGS(mod, ip)
);
#endif /* CONFIG_MODULE_UNLOAD */

TRACE_EVENT(module_request,

	TP_PROTO(char *name, bool wait, unsigned long ip),

	TP_ARGS(name, wait, ip),

	TP_STRUCT__entry(
		__field(	bool,		wait		)
		__field(	unsigned long,	ip		)
		__string(	name,		name		)
	),

	TP_fast_assign(
		__entry->wait	= wait;
		__entry->ip	= ip;
		__assign_str(name, name);
	),

	TP_printk("%s wait=%d call_site=%pf",
		  __get_str(name), (int)__entry->wait, (void *)__entry->ip)
);

#endif /* CONFIG_MODULES */

#endif /* _TRACE_MODULE_H */

/* This part must be outside protection */
#include <trace/define_trace.h>

