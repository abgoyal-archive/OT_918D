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
 *
  Copyright (c) Eicon Networks, 2002.
 *
  This source file is supplied for the use with
  Eicon Networks range of DIVA Server Adapters.
 *
  Eicon File Revision :    2.1
 *
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.
 *
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND WHATSOEVER INCLUDING ANY
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU General Public License for more details.
 *
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifdef PLATFORM_GT_32BIT
/* #define POINTER_32BIT byte * __ptr32 */
#define POINTER_32BIT dword 
#else
#define POINTER_32BIT byte *
#endif
#if !defined(MIPS_SCOM)
#define BUFFER_SZ  48
#define MAINT_OFFS 0x380
#else
#define BUFFER_SZ  128
#if defined(PRI)
#define MAINT_OFFS 0xef00
#else
#define MAINT_OFFS 0xff00
#endif
#endif
#define MIPS_BUFFER_SZ  128
#if defined(PRI)
#define MIPS_MAINT_OFFS 0xef00
#else
#define MIPS_MAINT_OFFS 0xff00
#endif
#define LOG                     1
#define MEMR                    2
#define MEMW                    3
#define IOR                     4
#define IOW                     5
#define B1TEST                  6
#define B2TEST                  7
#define BTESTOFF                8
#define DSIG_STATS              9
#define B_CH_STATS              10
#define D_CH_STATS              11
#define BL1_STATS               12
#define BL1_STATS_C             13
#define GET_VERSION             14
#define OS_STATS                15
#define XLOG_SET_MASK           16
#define XLOG_GET_MASK           17
#define DSP_READ                20
#define DSP_WRITE               21
#define OK 0xff
#define MORE_EVENTS 0xfe
#define NO_EVENT 1
struct DSigStruc
{
  byte Id;
  byte u;
  byte listen;
  byte active;
  byte sin[3];
  byte bc[6];
  byte llc[6];
  byte hlc[6];
  byte oad[20];
};
struct BL1Struc {
  dword cx_b1;
  dword cx_b2;
  dword cr_b1;
  dword cr_b2;
  dword px_b1;
  dword px_b2;
  dword pr_b1;
  dword pr_b2;
  word er_b1;
  word er_b2;
};
struct L2Struc {
  dword XTotal;
  dword RTotal;
  word XError;
  word RError;
};
struct OSStruc {
  dword free_n;
};
typedef union
{
  struct DSigStruc DSigStats;
  struct BL1Struc BL1Stats;
  struct L2Struc L2Stats;
  struct OSStruc OSStats;
  byte   b[BUFFER_SZ];
  word   w[BUFFER_SZ>>1];
  word   l[BUFFER_SZ>>2]; /* word is wrong, do not use! Use 'd' instead. */
  dword  d[BUFFER_SZ>>2];
} BUFFER;
typedef union
{
  struct DSigStruc DSigStats;
  struct BL1Struc BL1Stats;
  struct L2Struc L2Stats;
  struct OSStruc OSStats;
  byte   b[MIPS_BUFFER_SZ];
  word   w[MIPS_BUFFER_SZ>>1];
  word   l[BUFFER_SZ>>2]; /* word is wrong, do not use! Use 'd' instead. */
  dword  d[MIPS_BUFFER_SZ>>2];
} MIPS_BUFFER;
#if !defined(MIPS_SCOM)
struct pc_maint
{
  byte req;
  byte rc;
  POINTER_32BIT mem;
  short length;
  word port;
  byte fill[6];
  BUFFER data;
};
#else
struct pc_maint
{
  byte req;
  byte rc;
  byte reserved[2];     /* R3000 alignment ... */
  POINTER_32BIT mem;
  short length;
  word port;
  byte fill[4];         /* data at offset 16   */
  BUFFER data;
};
#endif
struct mi_pc_maint
{
  byte req;
  byte rc;
  byte reserved[2];     /* R3000 alignment ... */
  POINTER_32BIT mem;
  short length;
  word port;
  byte fill[4];         /* data at offset 16   */
  MIPS_BUFFER data;
};
