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


#ifndef CSP_HW_CFG_H
#define CSP_HW_CFG_H

/* ---- Include Files ---------------------------------------------------- */

#include <cfg_global.h>
#include <mach/csp/cap_inline.h>

#if defined(__KERNEL__)
#include <mach/memory_settings.h>
#else
#include <hw_cfg.h>
#endif

/* Some items that can be defined externally, but will be set to default values */
/* if they are not defined. */
/*      HW_CFG_PLL_SPREAD_SPECTRUM_DISABLE   Default undefined and SS is enabled. */
/*      HW_CFG_SDRAM_CAS_LATENCY        5    Default 5, Values [3..6] */
/*      HW_CFG_SDRAM_CHIP_SELECT_CNT    1    Default 1, Vaules [1..2] */
/*      HW_CFG_SDRAM_SPEED_GRADE        667  Default 667, Values [400,533,667,800] */
/*      HW_CFG_SDRAM_WIDTH_BITS         16   Default 16, Vaules [8,16] */
/*      HW_CFG_SDRAM_ADDR_BRC                Default undefined and Row-Bank-Col (RBC) addressing used. Define to use Bank-Row-Col (BRC). */
/*      HW_CFG_SDRAM_CLK_ASYNC               Default undefined and DDR clock is synchronous with AXI BUS clock. Define for ASYNC mode. */

#if defined(CFG_GLOBAL_CHIP)
  #if (CFG_GLOBAL_CHIP == FPGA11107)
     #define HW_CFG_BUS_CLK_HZ            5000000
     #define HW_CFG_DDR_CTLR_CLK_HZ      10000000
     #define HW_CFG_DDR_PHY_OMIT
     #define HW_CFG_UART_CLK_HZ           7500000
  #else
     #define HW_CFG_PLL_VCO_HZ           2000000000
     #define HW_CFG_PLL2_VCO_HZ          1800000000
     #define HW_CFG_ARM_CLK_HZ            CAP_HW_CFG_ARM_CLK_HZ
     #define HW_CFG_BUS_CLK_HZ            166666666
     #define HW_CFG_DDR_CTLR_CLK_HZ       333333333
     #define HW_CFG_DDR_PHY_CLK_HZ        (2 * HW_CFG_DDR_CTLR_CLK_HZ)
     #define HW_CFG_UART_CLK_HZ           142857142
     #define HW_CFG_VPM_CLK_HZ            CAP_HW_CFG_VPM_CLK_HZ
  #endif
#else
   #define HW_CFG_PLL_VCO_HZ           1800000000
   #define HW_CFG_PLL2_VCO_HZ          1800000000
   #define HW_CFG_ARM_CLK_HZ            450000000
   #define HW_CFG_BUS_CLK_HZ            150000000
   #define HW_CFG_DDR_CTLR_CLK_HZ       300000000
   #define HW_CFG_DDR_PHY_CLK_HZ        (2 * HW_CFG_DDR_CTLR_CLK_HZ)
   #define HW_CFG_UART_CLK_HZ           150000000
   #define HW_CFG_VPM_CLK_HZ            300000000
#endif

/* ---- Public Constants and Types --------------------------------------- */
/* ---- Public Variable Externs ------------------------------------------ */
/* ---- Public Function Prototypes --------------------------------------- */


#endif /* CSP_HW_CFG_H */

