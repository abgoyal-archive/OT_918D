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

/***********************license start***************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
 ***********************license end**************************************/

/*
 *
 * This is file defines ASM primitives for the executive.
 */
#ifndef __CVMX_ASM_H__
#define __CVMX_ASM_H__

#include "octeon-model.h"

/* other useful stuff */
#define CVMX_SYNC asm volatile ("sync" : : : "memory")
/* String version of SYNCW macro for using in inline asm constructs */
#define CVMX_SYNCW_STR "syncw\nsyncw\n"
#ifdef __OCTEON__

/* Deprecated, will be removed in future release */
#define CVMX_SYNCIO asm volatile ("nop")

#define CVMX_SYNCIOBDMA asm volatile ("synciobdma" : : : "memory")

/* Deprecated, will be removed in future release */
#define CVMX_SYNCIOALL asm volatile ("nop")

/*
 * We actually use two syncw instructions in a row when we need a write
 * memory barrier. This is because the CN3XXX series of Octeons have
 * errata Core-401. This can cause a single syncw to not enforce
 * ordering under very rare conditions. Even if it is rare, better safe
 * than sorry.
 */
#define CVMX_SYNCW asm volatile ("syncw\n\tsyncw" : : : "memory")

/*
 * Define new sync instructions to be normal SYNC instructions for
 * operating systems that use threads.
 */
#define CVMX_SYNCWS CVMX_SYNCW
#define CVMX_SYNCS  CVMX_SYNC
#define CVMX_SYNCWS_STR CVMX_SYNCW_STR
#else
/*
 * Not using a Cavium compiler, always use the slower sync so the
 * assembler stays happy.
 */
/* Deprecated, will be removed in future release */
#define CVMX_SYNCIO asm volatile ("nop")

#define CVMX_SYNCIOBDMA asm volatile ("sync" : : : "memory")

/* Deprecated, will be removed in future release */
#define CVMX_SYNCIOALL asm volatile ("nop")

#define CVMX_SYNCW asm volatile ("sync" : : : "memory")
#define CVMX_SYNCWS CVMX_SYNCW
#define CVMX_SYNCS  CVMX_SYNC
#define CVMX_SYNCWS_STR CVMX_SYNCW_STR
#endif

/*
 * CVMX_PREPARE_FOR_STORE makes each byte of the block unpredictable
 * (actually old value or zero) until that byte is stored to (by this or
 * another processor. Note that the value of each byte is not only
 * unpredictable, but may also change again - up until the point when one
 * of the cores stores to the byte.
 */
#define CVMX_PREPARE_FOR_STORE(address, offset) \
	asm volatile ("pref 30, " CVMX_TMP_STR(offset) "(%[rbase])" : : \
	[rbase] "d" (address))
/*
 * This is a command headed to the L2 controller to tell it to clear
 * its dirty bit for a block. Basically, SW is telling HW that the
 * current version of the block will not be used.
 */
#define CVMX_DONT_WRITE_BACK(address, offset) \
	asm volatile ("pref 29, " CVMX_TMP_STR(offset) "(%[rbase])" : : \
	[rbase] "d" (address))

/* flush stores, invalidate entire icache */
#define CVMX_ICACHE_INVALIDATE \
	{ CVMX_SYNC; asm volatile ("synci 0($0)" : : ); }

/* flush stores, invalidate entire icache */
#define CVMX_ICACHE_INVALIDATE2 \
	{ CVMX_SYNC; asm volatile ("cache 0, 0($0)" : : ); }

/* complete prefetches, invalidate entire dcache */
#define CVMX_DCACHE_INVALIDATE \
	{ CVMX_SYNC; asm volatile ("cache 9, 0($0)" : : ); }


#define CVMX_POP(result, input) \
	asm ("pop %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))
#define CVMX_DPOP(result, input) \
	asm ("dpop %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))

/* some new cop0-like stuff */
#define CVMX_RDHWR(result, regstr) \
	asm volatile ("rdhwr %[rt],$" CVMX_TMP_STR(regstr) : [rt] "=d" (result))
#define CVMX_RDHWRNV(result, regstr) \
	asm ("rdhwr %[rt],$" CVMX_TMP_STR(regstr) : [rt] "=d" (result))
#endif /* __CVMX_ASM_H__ */
