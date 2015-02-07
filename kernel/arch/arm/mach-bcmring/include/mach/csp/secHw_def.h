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
*  @file    secHw_def.h
*
*  @brief   Definitions for configuring/testing secure blocks
*
*  @note
*     None
*/
/****************************************************************************/

#ifndef SECHW_DEF_H
#define SECHW_DEF_H

#include <mach/csp/mm_io.h>

/* Bit mask for various secure device */
#define secHw_BLK_MASK_CHIP_CONTROL     0x00000001
#define secHw_BLK_MASK_KEY_SCAN         0x00000002
#define secHw_BLK_MASK_TOUCH_SCREEN     0x00000004
#define secHw_BLK_MASK_UART0            0x00000008
#define secHw_BLK_MASK_UART1            0x00000010
#define secHw_BLK_MASK_WATCHDOG         0x00000020
#define secHw_BLK_MASK_SPUM             0x00000040
#define secHw_BLK_MASK_DDR2             0x00000080
#define secHw_BLK_MASK_EXT_MEM          0x00000100
#define secHw_BLK_MASK_ESW              0x00000200
#define secHw_BLK_MASK_SPU              0x00010000
#define secHw_BLK_MASK_PKA              0x00020000
#define secHw_BLK_MASK_RNG              0x00040000
#define secHw_BLK_MASK_RTC              0x00080000
#define secHw_BLK_MASK_OTP              0x00100000
#define secHw_BLK_MASK_BOOT             0x00200000
#define secHw_BLK_MASK_MPU              0x00400000
#define secHw_BLK_MASK_TZCTRL           0x00800000
#define secHw_BLK_MASK_INTR             0x01000000

/* Trustzone register set */
typedef struct {
	volatile uint32_t status;	/* read only - reflects status of writes of 2 write registers */
	volatile uint32_t setUnsecure;	/* write only. reads back as 0 */
	volatile uint32_t setSecure;	/* write only. reads back as 0 */
} secHw_TZREG_t;

/* There are 2 register sets. The first is for the lower 16 bits, the 2nd */
/* is for the higher 16 bits. */

typedef enum {
	secHw_IDX_LS = 0,
	secHw_IDX_MS = 1,
	secHw_IDX_NUM
} secHw_IDX_e;

typedef struct {
	volatile secHw_TZREG_t reg[secHw_IDX_NUM];
} secHw_REGS_t;

/****************************************************************************/
/**
*  @brief  Configures a device as a secure device
*
*/
/****************************************************************************/
static inline void secHw_setSecure(uint32_t mask	/*  mask of type secHw_BLK_MASK_XXXXXX */
    );

/****************************************************************************/
/**
*  @brief  Configures a device as a non-secure device
*
*/
/****************************************************************************/
static inline void secHw_setUnsecure(uint32_t mask	/*  mask of type secHw_BLK_MASK_XXXXXX */
    );

/****************************************************************************/
/**
*  @brief  Get the trustzone status for all components. 1 = non-secure, 0 = secure
*
*/
/****************************************************************************/
static inline uint32_t secHw_getStatus(void);

#include <mach/csp/secHw_inline.h>

#endif /* SECHW_DEF_H */
