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

#ifndef __ASM_KGDB_H_
#define __ASM_KGDB_H_

#ifdef __KERNEL__

#include <asm/sgidefs.h>

#if (_MIPS_ISA == _MIPS_ISA_MIPS1) || (_MIPS_ISA == _MIPS_ISA_MIPS2) || \
	(_MIPS_ISA == _MIPS_ISA_MIPS32)

#define KGDB_GDB_REG_SIZE 32

#elif (_MIPS_ISA == _MIPS_ISA_MIPS3) || (_MIPS_ISA == _MIPS_ISA_MIPS4) || \
	(_MIPS_ISA == _MIPS_ISA_MIPS64)

#ifdef CONFIG_32BIT
#define KGDB_GDB_REG_SIZE 32
#else /* CONFIG_CPU_32BIT */
#define KGDB_GDB_REG_SIZE 64
#endif
#else
#error "Need to set KGDB_GDB_REG_SIZE for MIPS ISA"
#endif /* _MIPS_ISA */

#define BUFMAX			2048
#if (KGDB_GDB_REG_SIZE == 32)
#define NUMREGBYTES		(90*sizeof(u32))
#define NUMCRITREGBYTES		(12*sizeof(u32))
#else
#define NUMREGBYTES		(90*sizeof(u64))
#define NUMCRITREGBYTES		(12*sizeof(u64))
#endif
#define BREAK_INSTR_SIZE	4
#define CACHE_FLUSH_IS_SAFE	0

extern void arch_kgdb_breakpoint(void);
extern int kgdb_early_setup;
extern void *saved_vectors[32];
extern void handle_exception(struct pt_regs *regs);
extern void breakinst(void);
extern int kgdb_ll_trap(int cmd, const char *str,
			struct pt_regs *regs, long err, int trap, int sig);

#endif				/* __KERNEL__ */

#endif /* __ASM_KGDB_H_ */
