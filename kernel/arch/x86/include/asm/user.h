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

#ifndef _ASM_X86_USER_H
#define _ASM_X86_USER_H

#ifdef CONFIG_X86_32
# include "user_32.h"
#else
# include "user_64.h"
#endif

#include <asm/types.h>

struct user_ymmh_regs {
	/* 16 * 16 bytes for each YMMH-reg */
	__u32 ymmh_space[64];
};

struct user_xsave_hdr {
	__u64 xstate_bv;
	__u64 reserved1[2];
	__u64 reserved2[5];
};

/*
 * The structure layout of user_xstateregs, used for exporting the
 * extended register state through ptrace and core-dump (NT_X86_XSTATE note)
 * interfaces will be same as the memory layout of xsave used by the processor
 * (except for the bytes 464..511, which can be used by the software) and hence
 * the size of this structure varies depending on the features supported by the
 * processor and OS. The size of the structure that users need to use can be
 * obtained by doing:
 *     cpuid_count(0xd, 0, &eax, &ptrace_xstateregs_struct_size, &ecx, &edx);
 * i.e., cpuid.(eax=0xd,ecx=0).ebx will be the size that user (debuggers, etc.)
 * need to use.
 *
 * For now, only the first 8 bytes of the software usable bytes[464..471] will
 * be used and will be set to OS enabled xstate mask (which is same as the
 * 64bit mask returned by the xgetbv's xCR0).  Users (analyzing core dump
 * remotely, etc.) can use this mask as well as the mask saved in the
 * xstate_hdr bytes and interpret what states the processor/OS supports
 * and what states are in modified/initialized conditions for the
 * particular process/thread.
 *
 * Also when the user modifies certain state FP/SSE/etc through the
 * ptrace interface, they must ensure that the xsave_hdr.xstate_bv
 * bytes[512..519] of the memory layout are updated correspondingly.
 * i.e., for example when FP state is modified to a non-init state,
 * xsave_hdr.xstate_bv's bit 0 must be set to '1', when SSE is modified to
 * non-init state, xsave_hdr.xstate_bv's bit 1 must to be set to '1', etc.
 */
#define USER_XSTATE_FX_SW_WORDS 6
#define USER_XSTATE_XCR0_WORD	0

struct user_xstateregs {
	struct {
		__u64 fpx_space[58];
		__u64 xstate_fx_sw[USER_XSTATE_FX_SW_WORDS];
	} i387;
	struct user_xsave_hdr xsave_hdr;
	struct user_ymmh_regs ymmh;
	/* further processor state extensions go here */
};

#endif /* _ASM_X86_USER_H */
