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

#ifndef __iop_mpu_defs_h
#define __iop_mpu_defs_h

/*
 * This file is autogenerated from
 *   file:           ../../inst/io_proc/rtl/iop_mpu.r
 *     id:           iop_mpu.r,v 1.30 2005/02/17 08:12:33 niklaspa Exp
 *     last modfied: Mon Apr 11 16:08:45 2005
 *
 *   by /n/asic/design/tools/rdesc/src/rdes2c --outfile iop_mpu_defs.h ../../inst/io_proc/rtl/iop_mpu.r
 *      id: $Id: iop_mpu_defs.h,v 1.5 2005/04/24 18:31:05 starvik Exp $
 * Any changes here will be lost.
 *
 * -*- buffer-read-only: t -*-
 */
/* Main access macros */
#ifndef REG_RD
#define REG_RD( scope, inst, reg ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR
#define REG_WR( scope, inst, reg, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_VECT
#define REG_RD_VECT( scope, inst, reg, index ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_VECT
#define REG_WR_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT
#define REG_RD_INT( scope, inst, reg ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR_INT
#define REG_WR_INT( scope, inst, reg, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT_VECT
#define REG_RD_INT_VECT( scope, inst, reg, index ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_INT_VECT
#define REG_WR_INT_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_TYPE_CONV
#define REG_TYPE_CONV( type, orgtype, val ) \
  ( { union { orgtype o; type n; } r; r.o = val; r.n; } )
#endif

#ifndef reg_page_size
#define reg_page_size 8192
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg + \
    (index) * STRIDE_##scope##_##reg )
#endif

/* C-code for register scope iop_mpu */

#define STRIDE_iop_mpu_rw_r 4
/* Register rw_r, scope iop_mpu, type rw */
typedef unsigned int reg_iop_mpu_rw_r;
#define REG_RD_ADDR_iop_mpu_rw_r 0
#define REG_WR_ADDR_iop_mpu_rw_r 0

/* Register rw_ctrl, scope iop_mpu, type rw */
typedef struct {
  unsigned int en : 1;
  unsigned int dummy1 : 31;
} reg_iop_mpu_rw_ctrl;
#define REG_RD_ADDR_iop_mpu_rw_ctrl 128
#define REG_WR_ADDR_iop_mpu_rw_ctrl 128

/* Register r_pc, scope iop_mpu, type r */
typedef struct {
  unsigned int addr : 12;
  unsigned int dummy1 : 20;
} reg_iop_mpu_r_pc;
#define REG_RD_ADDR_iop_mpu_r_pc 132

/* Register r_stat, scope iop_mpu, type r */
typedef struct {
  unsigned int instr_reg_busy : 1;
  unsigned int intr_busy      : 1;
  unsigned int intr_vect      : 16;
  unsigned int dummy1         : 14;
} reg_iop_mpu_r_stat;
#define REG_RD_ADDR_iop_mpu_r_stat 136

/* Register rw_instr, scope iop_mpu, type rw */
typedef unsigned int reg_iop_mpu_rw_instr;
#define REG_RD_ADDR_iop_mpu_rw_instr 140
#define REG_WR_ADDR_iop_mpu_rw_instr 140

/* Register rw_immediate, scope iop_mpu, type rw */
typedef unsigned int reg_iop_mpu_rw_immediate;
#define REG_RD_ADDR_iop_mpu_rw_immediate 144
#define REG_WR_ADDR_iop_mpu_rw_immediate 144

/* Register r_trace, scope iop_mpu, type r */
typedef struct {
  unsigned int intr_vect      : 16;
  unsigned int pc             : 12;
  unsigned int en             : 1;
  unsigned int instr_reg_busy : 1;
  unsigned int intr_busy      : 1;
  unsigned int dummy1         : 1;
} reg_iop_mpu_r_trace;
#define REG_RD_ADDR_iop_mpu_r_trace 148

/* Register r_wr_stat, scope iop_mpu, type r */
typedef struct {
  unsigned int r0  : 1;
  unsigned int r1  : 1;
  unsigned int r2  : 1;
  unsigned int r3  : 1;
  unsigned int r4  : 1;
  unsigned int r5  : 1;
  unsigned int r6  : 1;
  unsigned int r7  : 1;
  unsigned int r8  : 1;
  unsigned int r9  : 1;
  unsigned int r10 : 1;
  unsigned int r11 : 1;
  unsigned int r12 : 1;
  unsigned int r13 : 1;
  unsigned int r14 : 1;
  unsigned int r15 : 1;
  unsigned int dummy1 : 16;
} reg_iop_mpu_r_wr_stat;
#define REG_RD_ADDR_iop_mpu_r_wr_stat 152

#define STRIDE_iop_mpu_rw_thread 4
/* Register rw_thread, scope iop_mpu, type rw */
typedef struct {
  unsigned int addr : 12;
  unsigned int dummy1 : 20;
} reg_iop_mpu_rw_thread;
#define REG_RD_ADDR_iop_mpu_rw_thread 156
#define REG_WR_ADDR_iop_mpu_rw_thread 156

#define STRIDE_iop_mpu_rw_intr 4
/* Register rw_intr, scope iop_mpu, type rw */
typedef struct {
  unsigned int addr : 12;
  unsigned int dummy1 : 20;
} reg_iop_mpu_rw_intr;
#define REG_RD_ADDR_iop_mpu_rw_intr 196
#define REG_WR_ADDR_iop_mpu_rw_intr 196


/* Constants */
enum {
  regk_iop_mpu_no                          = 0x00000000,
  regk_iop_mpu_r_pc_default                = 0x00000000,
  regk_iop_mpu_rw_ctrl_default             = 0x00000000,
  regk_iop_mpu_rw_intr_size                = 0x00000010,
  regk_iop_mpu_rw_r_size                   = 0x00000010,
  regk_iop_mpu_rw_thread_default           = 0x00000000,
  regk_iop_mpu_rw_thread_size              = 0x00000004,
  regk_iop_mpu_yes                         = 0x00000001
};
#endif /* __iop_mpu_defs_h */