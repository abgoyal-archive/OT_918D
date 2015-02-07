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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM powerpc

#if !defined(_TRACE_POWERPC_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_POWERPC_H

#include <linux/tracepoint.h>

struct pt_regs;
#if 0 /* disabled by Mathieu Desnoyers. Belongs to generic IRQS. */
TRACE_EVENT(irq_entry,

	TP_PROTO(struct pt_regs *regs),

	TP_ARGS(regs),

	TP_STRUCT__entry(
		__field(struct pt_regs *, regs)
	),

	TP_fast_assign(
		__entry->regs = regs;
	),

	TP_printk("pt_regs=%p", __entry->regs)
);

TRACE_EVENT(irq_exit,

	TP_PROTO(struct pt_regs *regs),

	TP_ARGS(regs),

	TP_STRUCT__entry(
		__field(struct pt_regs *, regs)
	),

	TP_fast_assign(
		__entry->regs = regs;
	),

	TP_printk("pt_regs=%p", __entry->regs)
);
#endif //0

TRACE_EVENT(timer_interrupt_entry,

	TP_PROTO(struct pt_regs *regs),

	TP_ARGS(regs),

	TP_STRUCT__entry(
		__field(struct pt_regs *, regs)
	),

	TP_fast_assign(
		__entry->regs = regs;
	),

	TP_printk("pt_regs=%p", __entry->regs)
);

TRACE_EVENT(timer_interrupt_exit,

	TP_PROTO(struct pt_regs *regs),

	TP_ARGS(regs),

	TP_STRUCT__entry(
		__field(struct pt_regs *, regs)
	),

	TP_fast_assign(
		__entry->regs = regs;
	),

	TP_printk("pt_regs=%p", __entry->regs)
);

#ifdef CONFIG_PPC_PSERIES
extern void hcall_tracepoint_regfunc(void);
extern void hcall_tracepoint_unregfunc(void);

TRACE_EVENT_FN(hcall_entry,

	TP_PROTO(unsigned long opcode, unsigned long *args),

	TP_ARGS(opcode, args),

	TP_STRUCT__entry(
		__field(unsigned long, opcode)
	),

	TP_fast_assign(
		__entry->opcode = opcode;
	),

	TP_printk("opcode=%lu", __entry->opcode),

	hcall_tracepoint_regfunc, hcall_tracepoint_unregfunc
);

TRACE_EVENT_FN(hcall_exit,

	TP_PROTO(unsigned long opcode, unsigned long retval,
		unsigned long *retbuf),

	TP_ARGS(opcode, retval, retbuf),

	TP_STRUCT__entry(
		__field(unsigned long, opcode)
		__field(unsigned long, retval)
	),

	TP_fast_assign(
		__entry->opcode = opcode;
		__entry->retval = retval;
	),

	TP_printk("opcode=%lu retval=%lu", __entry->opcode, __entry->retval),

	hcall_tracepoint_regfunc, hcall_tracepoint_unregfunc
);
#endif

#endif /* _TRACE_POWERPC_H */

#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE

#define TRACE_INCLUDE_PATH asm
#define TRACE_INCLUDE_FILE trace

#include <trace/define_trace.h>
