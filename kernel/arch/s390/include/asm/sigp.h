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
 *  Routines and structures for signalling other processors.
 *
 *    Copyright IBM Corp. 1999,2010
 *    Author(s): Denis Joseph Barrow,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
 */

#ifndef __ASM_SIGP_H
#define __ASM_SIGP_H

#include <asm/system.h>

/* Get real cpu address from logical cpu number. */
extern unsigned short __cpu_logical_map[];

static inline int cpu_logical_map(int cpu)
{
#ifdef CONFIG_SMP
	return __cpu_logical_map[cpu];
#else
	return stap();
#endif
}

enum {
	sigp_sense = 1,
	sigp_external_call = 2,
	sigp_emergency_signal = 3,
	sigp_start = 4,
	sigp_stop = 5,
	sigp_restart = 6,
	sigp_stop_and_store_status = 9,
	sigp_initial_cpu_reset = 11,
	sigp_cpu_reset = 12,
	sigp_set_prefix = 13,
	sigp_store_status_at_address = 14,
	sigp_store_extended_status_at_address = 15,
	sigp_set_architecture = 18,
	sigp_conditional_emergency_signal = 19,
	sigp_sense_running = 21,
};

enum {
	sigp_order_code_accepted = 0,
	sigp_status_stored = 1,
	sigp_busy = 2,
	sigp_not_operational = 3,
};

/*
 * Definitions for external call.
 */
enum {
	ec_schedule = 0,
	ec_call_function,
	ec_call_function_single,
};

/*
 * Signal processor.
 */
static inline int raw_sigp(u16 cpu, int order)
{
	register unsigned long reg1 asm ("1") = 0;
	int ccode;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		:	"=d"	(ccode)
		: "d" (reg1), "d" (cpu),
		  "a" (order) : "cc" , "memory");
	return ccode;
}

/*
 * Signal processor with parameter.
 */
static inline int raw_sigp_p(u32 parameter, u16 cpu, int order)
{
	register unsigned int reg1 asm ("1") = parameter;
	int ccode;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (ccode)
		: "d" (reg1), "d" (cpu),
		  "a" (order) : "cc" , "memory");
	return ccode;
}

/*
 * Signal processor with parameter and return status.
 */
static inline int raw_sigp_ps(u32 *status, u32 parm, u16 cpu, int order)
{
	register unsigned int reg1 asm ("1") = parm;
	int ccode;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (ccode), "+d" (reg1)
		: "d" (cpu), "a" (order)
		: "cc" , "memory");
	*status = reg1;
	return ccode;
}

static inline int sigp(int cpu, int order)
{
	return raw_sigp(cpu_logical_map(cpu), order);
}

static inline int sigp_p(u32 parameter, int cpu, int order)
{
	return raw_sigp_p(parameter, cpu_logical_map(cpu), order);
}

static inline int sigp_ps(u32 *status, u32 parm, int cpu, int order)
{
	return raw_sigp_ps(status, parm, cpu_logical_map(cpu), order);
}

#endif /* __ASM_SIGP_H */
