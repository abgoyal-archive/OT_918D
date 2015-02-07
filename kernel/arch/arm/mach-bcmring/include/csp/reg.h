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

/*****************************************************************************
* Copyright 2003 - 2008 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

/****************************************************************************/
/**
*  @file    reg.h
*
*  @brief   Generic register definitions used in CSP
*/
/****************************************************************************/

#ifndef CSP_REG_H
#define CSP_REG_H

/* ---- Include Files ---------------------------------------------------- */

#include <csp/stdint.h>

/* ---- Public Constants and Types --------------------------------------- */

#define __REG32(x)      (*((volatile uint32_t *)(x)))
#define __REG16(x)      (*((volatile uint16_t *)(x)))
#define __REG8(x)       (*((volatile uint8_t *) (x)))

/* Macros used to define a sequence of reserved registers. The start / end */
/* are byte offsets in the particular register definition, with the "end" */
/* being the offset of the next un-reserved register. E.g. if offsets */
/* 0x10 through to 0x1f are reserved, then this reserved area could be */
/* specified as follows. */
/*  typedef struct */
/*  { */
/*      uint32_t reg1;           offset 0x00 */
/*      uint32_t reg2;           offset 0x04 */
/*      uint32_t reg3;           offset 0x08 */
/*      uint32_t reg4;           offset 0x0c */
/*      REG32_RSVD(0x10, 0x20); */
/*      uint32_t reg5;           offset 0x20 */
/*      ... */
/*  } EXAMPLE_REG_t; */
#define REG8_RSVD(start, end)   uint8_t rsvd_##start[(end - start) / sizeof(uint8_t)]
#define REG16_RSVD(start, end)  uint16_t rsvd_##start[(end - start) / sizeof(uint16_t)]
#define REG32_RSVD(start, end)  uint32_t rsvd_##start[(end - start) / sizeof(uint32_t)]

/* ---- Public Variable Externs ------------------------------------------ */
/* ---- Public Function Prototypes --------------------------------------- */

/* Note: When protecting multiple statements, the REG_LOCAL_IRQ_SAVE and */
/* REG_LOCAL_IRQ_RESTORE must be enclosed in { } to allow the  */
/* flags variable to be declared locally. */
/* e.g. */
/*    statement1; */
/*    { */
/*       REG_LOCAL_IRQ_SAVE; */
/*       <multiple statements here> */
/*       REG_LOCAL_IRQ_RESTORE; */
/*    } */
/*    statement2; */
/*  */

#if defined(__KERNEL__) && !defined(STANDALONE)
#include <mach/hardware.h>
#include <linux/interrupt.h>

#define REG_LOCAL_IRQ_SAVE      HW_DECLARE_SPINLOCK(reg32) \
	unsigned long flags; HW_IRQ_SAVE(reg32, flags)

#define REG_LOCAL_IRQ_RESTORE   HW_IRQ_RESTORE(reg32, flags)

#else

#define REG_LOCAL_IRQ_SAVE
#define REG_LOCAL_IRQ_RESTORE

#endif

static inline void reg32_modify_and(volatile uint32_t *reg, uint32_t value)
{
	REG_LOCAL_IRQ_SAVE;
	*reg &= value;
	REG_LOCAL_IRQ_RESTORE;
}

static inline void reg32_modify_or(volatile uint32_t *reg, uint32_t value)
{
	REG_LOCAL_IRQ_SAVE;
	*reg |= value;
	REG_LOCAL_IRQ_RESTORE;
}

static inline void reg32_modify_mask(volatile uint32_t *reg, uint32_t mask,
				     uint32_t value)
{
	REG_LOCAL_IRQ_SAVE;
	*reg = (*reg & mask) | value;
	REG_LOCAL_IRQ_RESTORE;
}

static inline void reg32_write(volatile uint32_t *reg, uint32_t value)
{
	*reg = value;
}

#endif /* CSP_REG_H */
