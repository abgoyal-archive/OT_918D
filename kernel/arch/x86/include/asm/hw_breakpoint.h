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

#ifndef	_I386_HW_BREAKPOINT_H
#define	_I386_HW_BREAKPOINT_H

#ifdef	__KERNEL__
#define	__ARCH_HW_BREAKPOINT_H

/*
 * The name should probably be something dealt in
 * a higher level. While dealing with the user
 * (display/resolving)
 */
struct arch_hw_breakpoint {
	unsigned long	address;
	u8		len;
	u8		type;
};

#include <linux/kdebug.h>
#include <linux/percpu.h>
#include <linux/list.h>

/* Available HW breakpoint length encodings */
#define X86_BREAKPOINT_LEN_1		0x40
#define X86_BREAKPOINT_LEN_2		0x44
#define X86_BREAKPOINT_LEN_4		0x4c
#define X86_BREAKPOINT_LEN_EXECUTE	0x40

#ifdef CONFIG_X86_64
#define X86_BREAKPOINT_LEN_8		0x48
#endif

/* Available HW breakpoint type encodings */

/* trigger on instruction execute */
#define X86_BREAKPOINT_EXECUTE	0x80
/* trigger on memory write */
#define X86_BREAKPOINT_WRITE	0x81
/* trigger on memory read or write */
#define X86_BREAKPOINT_RW	0x83

/* Total number of available HW breakpoint registers */
#define HBP_NUM 4

static inline int hw_breakpoint_slots(int type)
{
	return HBP_NUM;
}

struct perf_event;
struct pmu;

extern int arch_check_bp_in_kernelspace(struct perf_event *bp);
extern int arch_validate_hwbkpt_settings(struct perf_event *bp);
extern int hw_breakpoint_exceptions_notify(struct notifier_block *unused,
					   unsigned long val, void *data);


int arch_install_hw_breakpoint(struct perf_event *bp);
void arch_uninstall_hw_breakpoint(struct perf_event *bp);
void hw_breakpoint_pmu_read(struct perf_event *bp);
void hw_breakpoint_pmu_unthrottle(struct perf_event *bp);

extern void
arch_fill_perf_breakpoint(struct perf_event *bp);

unsigned long encode_dr7(int drnum, unsigned int len, unsigned int type);
int decode_dr7(unsigned long dr7, int bpnum, unsigned *len, unsigned *type);

extern int arch_bp_generic_fields(int x86_len, int x86_type,
				  int *gen_len, int *gen_type);

extern struct pmu perf_ops_bp;

#endif	/* __KERNEL__ */
#endif	/* _I386_HW_BREAKPOINT_H */

