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
 * Copyright 2005-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later
 */

#ifndef _CDEF_BF532_H
#define _CDEF_BF532_H

#include <asm/blackfin.h>

/*include all Core registers and bit definitions*/
#include "defBF532.h"

/*include core specific register pointer definitions*/
#include <asm/cdef_LPBlackfin.h>

/* Clock and System Control (0xFFC0 0400-0xFFC0 07FF) */
#define bfin_read_PLL_CTL()                  bfin_read16(PLL_CTL)
#define bfin_read_PLL_STAT()                 bfin_read16(PLL_STAT)
#define bfin_write_PLL_STAT(val)             bfin_write16(PLL_STAT,val)
#define bfin_read_PLL_LOCKCNT()              bfin_read16(PLL_LOCKCNT)
#define bfin_write_PLL_LOCKCNT(val)          bfin_write16(PLL_LOCKCNT,val)
#define bfin_read_CHIPID()                   bfin_read32(CHIPID)
#define bfin_read_PLL_DIV()                  bfin_read16(PLL_DIV)
#define bfin_write_PLL_DIV(val)              bfin_write16(PLL_DIV,val)
#define bfin_read_VR_CTL()                   bfin_read16(VR_CTL)

/* System Interrupt Controller (0xFFC0 0C00-0xFFC0 0FFF) */
#define bfin_read_SWRST()                    bfin_read16(SWRST)
#define bfin_write_SWRST(val)                bfin_write16(SWRST,val)
#define bfin_read_SYSCR()                    bfin_read16(SYSCR)
#define bfin_write_SYSCR(val)                bfin_write16(SYSCR,val)
#define bfin_read_SIC_IAR0()                 bfin_read32(SIC_IAR0)
#define bfin_write_SIC_IAR0(val)             bfin_write32(SIC_IAR0,val)
#define bfin_read_SIC_IAR1()                 bfin_read32(SIC_IAR1)
#define bfin_write_SIC_IAR1(val)             bfin_write32(SIC_IAR1,val)
#define bfin_read_SIC_IAR2()                 bfin_read32(SIC_IAR2)
#define bfin_write_SIC_IAR2(val)             bfin_write32(SIC_IAR2,val)
#define bfin_read_SIC_IAR3()                 bfin_read32(SIC_IAR3)
#define bfin_write_SIC_IAR3(val)             bfin_write32(SIC_IAR3,val)
#define bfin_read_SIC_IMASK()                bfin_read32(SIC_IMASK)
#define bfin_write_SIC_IMASK(val)            bfin_write32(SIC_IMASK,val)
#define bfin_read_SIC_ISR()                  bfin_read32(SIC_ISR)
#define bfin_write_SIC_ISR(val)              bfin_write32(SIC_ISR,val)
#define bfin_read_SIC_IWR()                  bfin_read32(SIC_IWR)
#define bfin_write_SIC_IWR(val)              bfin_write32(SIC_IWR,val)

/* Watchdog Timer (0xFFC0 1000-0xFFC0 13FF) */
#define bfin_read_WDOG_CTL()                 bfin_read16(WDOG_CTL)
#define bfin_write_WDOG_CTL(val)             bfin_write16(WDOG_CTL,val)
#define bfin_read_WDOG_CNT()                 bfin_read32(WDOG_CNT)
#define bfin_write_WDOG_CNT(val)             bfin_write32(WDOG_CNT,val)
#define bfin_read_WDOG_STAT()                bfin_read32(WDOG_STAT)
#define bfin_write_WDOG_STAT(val)            bfin_write32(WDOG_STAT,val)

/* Real Time Clock (0xFFC0 1400-0xFFC0 17FF) */
#define bfin_read_RTC_STAT()                 bfin_read32(RTC_STAT)
#define bfin_write_RTC_STAT(val)             bfin_write32(RTC_STAT,val)
#define bfin_read_RTC_ICTL()                 bfin_read16(RTC_ICTL)
#define bfin_write_RTC_ICTL(val)             bfin_write16(RTC_ICTL,val)
#define bfin_read_RTC_ISTAT()                bfin_read16(RTC_ISTAT)
#define bfin_write_RTC_ISTAT(val)            bfin_write16(RTC_ISTAT,val)
#define bfin_read_RTC_SWCNT()                bfin_read16(RTC_SWCNT)
#define bfin_write_RTC_SWCNT(val)            bfin_write16(RTC_SWCNT,val)
#define bfin_read_RTC_ALARM()                bfin_read32(RTC_ALARM)
#define bfin_write_RTC_ALARM(val)            bfin_write32(RTC_ALARM,val)
#define bfin_read_RTC_FAST()                 bfin_read16(RTC_FAST)
#define bfin_write_RTC_FAST(val)             bfin_write16(RTC_FAST,val)
#define bfin_read_RTC_PREN()                 bfin_read16(RTC_PREN)
#define bfin_write_RTC_PREN(val)             bfin_write16(RTC_PREN,val)

/* DMA Traffic controls */
#define bfin_read_DMA_TCPER()                bfin_read16(DMA_TCPER)
#define bfin_write_DMA_TCPER(val)            bfin_write16(DMA_TCPER,val)
#define bfin_read_DMA_TCCNT()                bfin_read16(DMA_TCCNT)
#define bfin_write_DMA_TCCNT(val)            bfin_write16(DMA_TCCNT,val)

/* Alternate deprecated register names (below) provided for backwards code compatibility */
#define bfin_read_DMA_TC_PER()               bfin_read16(DMA_TC_PER)
#define bfin_write_DMA_TC_PER(val)           bfin_write16(DMA_TC_PER,val)
#define bfin_read_DMA_TC_CNT()               bfin_read16(DMA_TC_CNT)
#define bfin_write_DMA_TC_CNT(val)           bfin_write16(DMA_TC_CNT,val)

/* General Purpose IO (0xFFC0 2400-0xFFC0 27FF) */
#define bfin_read_FIO_DIR()                  bfin_read16(FIO_DIR)
#define bfin_write_FIO_DIR(val)              bfin_write16(FIO_DIR,val)
#define bfin_read_FIO_MASKA_C()              bfin_read16(FIO_MASKA_C)
#define bfin_write_FIO_MASKA_C(val)          bfin_write16(FIO_MASKA_C,val)
#define bfin_read_FIO_MASKA_S()              bfin_read16(FIO_MASKA_S)
#define bfin_write_FIO_MASKA_S(val)          bfin_write16(FIO_MASKA_S,val)
#define bfin_read_FIO_MASKB_C()              bfin_read16(FIO_MASKB_C)
#define bfin_write_FIO_MASKB_C(val)          bfin_write16(FIO_MASKB_C,val)
#define bfin_read_FIO_MASKB_S()              bfin_read16(FIO_MASKB_S)
#define bfin_write_FIO_MASKB_S(val)          bfin_write16(FIO_MASKB_S,val)
#define bfin_read_FIO_POLAR()                bfin_read16(FIO_POLAR)
#define bfin_write_FIO_POLAR(val)            bfin_write16(FIO_POLAR,val)
#define bfin_read_FIO_EDGE()                 bfin_read16(FIO_EDGE)
#define bfin_write_FIO_EDGE(val)             bfin_write16(FIO_EDGE,val)
#define bfin_read_FIO_BOTH()                 bfin_read16(FIO_BOTH)
#define bfin_write_FIO_BOTH(val)             bfin_write16(FIO_BOTH,val)
#define bfin_read_FIO_INEN()                 bfin_read16(FIO_INEN)
#define bfin_write_FIO_INEN(val)             bfin_write16(FIO_INEN,val)
#define bfin_read_FIO_MASKA_D()              bfin_read16(FIO_MASKA_D)
#define bfin_write_FIO_MASKA_D(val)          bfin_write16(FIO_MASKA_D,val)
#define bfin_read_FIO_MASKA_T()              bfin_read16(FIO_MASKA_T)
#define bfin_write_FIO_MASKA_T(val)          bfin_write16(FIO_MASKA_T,val)
#define bfin_read_FIO_MASKB_D()              bfin_read16(FIO_MASKB_D)
#define bfin_write_FIO_MASKB_D(val)          bfin_write16(FIO_MASKB_D,val)
#define bfin_read_FIO_MASKB_T()              bfin_read16(FIO_MASKB_T)
#define bfin_write_FIO_MASKB_T(val)          bfin_write16(FIO_MASKB_T,val)

/* DMA Controller */
#define bfin_read_DMA0_CONFIG()              bfin_read16(DMA0_CONFIG)
#define bfin_write_DMA0_CONFIG(val)          bfin_write16(DMA0_CONFIG,val)
#define bfin_read_DMA0_NEXT_DESC_PTR()       bfin_read32(DMA0_NEXT_DESC_PTR)
#define bfin_write_DMA0_NEXT_DESC_PTR(val)   bfin_write32(DMA0_NEXT_DESC_PTR,val)
#define bfin_read_DMA0_START_ADDR()          bfin_read32(DMA0_START_ADDR)
#define bfin_write_DMA0_START_ADDR(val)      bfin_write32(DMA0_START_ADDR,val)
#define bfin_read_DMA0_X_COUNT()             bfin_read16(DMA0_X_COUNT)
#define bfin_write_DMA0_X_COUNT(val)         bfin_write16(DMA0_X_COUNT,val)
#define bfin_read_DMA0_Y_COUNT()             bfin_read16(DMA0_Y_COUNT)
#define bfin_write_DMA0_Y_COUNT(val)         bfin_write16(DMA0_Y_COUNT,val)
#define bfin_read_DMA0_X_MODIFY()            bfin_read16(DMA0_X_MODIFY)
#define bfin_write_DMA0_X_MODIFY(val)        bfin_write16(DMA0_X_MODIFY,val)
#define bfin_read_DMA0_Y_MODIFY()            bfin_read16(DMA0_Y_MODIFY)
#define bfin_write_DMA0_Y_MODIFY(val)        bfin_write16(DMA0_Y_MODIFY,val)
#define bfin_read_DMA0_CURR_DESC_PTR()       bfin_read32(DMA0_CURR_DESC_PTR)
#define bfin_write_DMA0_CURR_DESC_PTR(val)   bfin_write32(DMA0_CURR_DESC_PTR,val)
#define bfin_read_DMA0_CURR_ADDR()           bfin_read32(DMA0_CURR_ADDR)
#define bfin_write_DMA0_CURR_ADDR(val)       bfin_write32(DMA0_CURR_ADDR,val)
#define bfin_read_DMA0_CURR_X_COUNT()        bfin_read16(DMA0_CURR_X_COUNT)
#define bfin_write_DMA0_CURR_X_COUNT(val)    bfin_write16(DMA0_CURR_X_COUNT,val)
#define bfin_read_DMA0_CURR_Y_COUNT()        bfin_read16(DMA0_CURR_Y_COUNT)
#define bfin_write_DMA0_CURR_Y_COUNT(val)    bfin_write16(DMA0_CURR_Y_COUNT,val)
#define bfin_read_DMA0_IRQ_STATUS()          bfin_read16(DMA0_IRQ_STATUS)
#define bfin_write_DMA0_IRQ_STATUS(val)      bfin_write16(DMA0_IRQ_STATUS,val)
#define bfin_read_DMA0_PERIPHERAL_MAP()      bfin_read16(DMA0_PERIPHERAL_MAP)
#define bfin_write_DMA0_PERIPHERAL_MAP(val)  bfin_write16(DMA0_PERIPHERAL_MAP,val)

#define bfin_read_DMA1_CONFIG()              bfin_read16(DMA1_CONFIG)
#define bfin_write_DMA1_CONFIG(val)          bfin_write16(DMA1_CONFIG,val)
#define bfin_read_DMA1_NEXT_DESC_PTR()       bfin_read32(DMA1_NEXT_DESC_PTR)
#define bfin_write_DMA1_NEXT_DESC_PTR(val)   bfin_write32(DMA1_NEXT_DESC_PTR,val)
#define bfin_read_DMA1_START_ADDR()          bfin_read32(DMA1_START_ADDR)
#define bfin_write_DMA1_START_ADDR(val)      bfin_write32(DMA1_START_ADDR,val)
#define bfin_read_DMA1_X_COUNT()             bfin_read16(DMA1_X_COUNT)
#define bfin_write_DMA1_X_COUNT(val)         bfin_write16(DMA1_X_COUNT,val)
#define bfin_read_DMA1_Y_COUNT()             bfin_read16(DMA1_Y_COUNT)
#define bfin_write_DMA1_Y_COUNT(val)         bfin_write16(DMA1_Y_COUNT,val)
#define bfin_read_DMA1_X_MODIFY()            bfin_read16(DMA1_X_MODIFY)
#define bfin_write_DMA1_X_MODIFY(val)        bfin_write16(DMA1_X_MODIFY,val)
#define bfin_read_DMA1_Y_MODIFY()            bfin_read16(DMA1_Y_MODIFY)
#define bfin_write_DMA1_Y_MODIFY(val)        bfin_write16(DMA1_Y_MODIFY,val)
#define bfin_read_DMA1_CURR_DESC_PTR()       bfin_read32(DMA1_CURR_DESC_PTR)
#define bfin_write_DMA1_CURR_DESC_PTR(val)   bfin_write32(DMA1_CURR_DESC_PTR,val)
#define bfin_read_DMA1_CURR_ADDR()           bfin_read32(DMA1_CURR_ADDR)
#define bfin_write_DMA1_CURR_ADDR(val)       bfin_write32(DMA1_CURR_ADDR,val)
#define bfin_read_DMA1_CURR_X_COUNT()        bfin_read16(DMA1_CURR_X_COUNT)
#define bfin_write_DMA1_CURR_X_COUNT(val)    bfin_write16(DMA1_CURR_X_COUNT,val)
#define bfin_read_DMA1_CURR_Y_COUNT()        bfin_read16(DMA1_CURR_Y_COUNT)
#define bfin_write_DMA1_CURR_Y_COUNT(val)    bfin_write16(DMA1_CURR_Y_COUNT,val)
#define bfin_read_DMA1_IRQ_STATUS()          bfin_read16(DMA1_IRQ_STATUS)
#define bfin_write_DMA1_IRQ_STATUS(val)      bfin_write16(DMA1_IRQ_STATUS,val)
#define bfin_read_DMA1_PERIPHERAL_MAP()      bfin_read16(DMA1_PERIPHERAL_MAP)
#define bfin_write_DMA1_PERIPHERAL_MAP(val)  bfin_write16(DMA1_PERIPHERAL_MAP,val)

#define bfin_read_DMA2_CONFIG()              bfin_read16(DMA2_CONFIG)
#define bfin_write_DMA2_CONFIG(val)          bfin_write16(DMA2_CONFIG,val)
#define bfin_read_DMA2_NEXT_DESC_PTR()       bfin_read32(DMA2_NEXT_DESC_PTR)
#define bfin_write_DMA2_NEXT_DESC_PTR(val)   bfin_write32(DMA2_NEXT_DESC_PTR,val)
#define bfin_read_DMA2_START_ADDR()          bfin_read32(DMA2_START_ADDR)
#define bfin_write_DMA2_START_ADDR(val)      bfin_write32(DMA2_START_ADDR,val)
#define bfin_read_DMA2_X_COUNT()             bfin_read16(DMA2_X_COUNT)
#define bfin_write_DMA2_X_COUNT(val)         bfin_write16(DMA2_X_COUNT,val)
#define bfin_read_DMA2_Y_COUNT()             bfin_read16(DMA2_Y_COUNT)
#define bfin_write_DMA2_Y_COUNT(val)         bfin_write16(DMA2_Y_COUNT,val)
#define bfin_read_DMA2_X_MODIFY()            bfin_read16(DMA2_X_MODIFY)
#define bfin_write_DMA2_X_MODIFY(val)        bfin_write16(DMA2_X_MODIFY,val)
#define bfin_read_DMA2_Y_MODIFY()            bfin_read16(DMA2_Y_MODIFY)
#define bfin_write_DMA2_Y_MODIFY(val)        bfin_write16(DMA2_Y_MODIFY,val)
#define bfin_read_DMA2_CURR_DESC_PTR()       bfin_read32(DMA2_CURR_DESC_PTR)
#define bfin_write_DMA2_CURR_DESC_PTR(val)   bfin_write32(DMA2_CURR_DESC_PTR,val)
#define bfin_read_DMA2_CURR_ADDR()           bfin_read32(DMA2_CURR_ADDR)
#define bfin_write_DMA2_CURR_ADDR(val)       bfin_write32(DMA2_CURR_ADDR,val)
#define bfin_read_DMA2_CURR_X_COUNT()        bfin_read16(DMA2_CURR_X_COUNT)
#define bfin_write_DMA2_CURR_X_COUNT(val)    bfin_write16(DMA2_CURR_X_COUNT,val)
#define bfin_read_DMA2_CURR_Y_COUNT()        bfin_read16(DMA2_CURR_Y_COUNT)
#define bfin_write_DMA2_CURR_Y_COUNT(val)    bfin_write16(DMA2_CURR_Y_COUNT,val)
#define bfin_read_DMA2_IRQ_STATUS()          bfin_read16(DMA2_IRQ_STATUS)
#define bfin_write_DMA2_IRQ_STATUS(val)      bfin_write16(DMA2_IRQ_STATUS,val)
#define bfin_read_DMA2_PERIPHERAL_MAP()      bfin_read16(DMA2_PERIPHERAL_MAP)
#define bfin_write_DMA2_PERIPHERAL_MAP(val)  bfin_write16(DMA2_PERIPHERAL_MAP,val)

#define bfin_read_DMA3_CONFIG()              bfin_read16(DMA3_CONFIG)
#define bfin_write_DMA3_CONFIG(val)          bfin_write16(DMA3_CONFIG,val)
#define bfin_read_DMA3_NEXT_DESC_PTR()       bfin_read32(DMA3_NEXT_DESC_PTR)
#define bfin_write_DMA3_NEXT_DESC_PTR(val)   bfin_write32(DMA3_NEXT_DESC_PTR,val)
#define bfin_read_DMA3_START_ADDR()          bfin_read32(DMA3_START_ADDR)
#define bfin_write_DMA3_START_ADDR(val)      bfin_write32(DMA3_START_ADDR,val)
#define bfin_read_DMA3_X_COUNT()             bfin_read16(DMA3_X_COUNT)
#define bfin_write_DMA3_X_COUNT(val)         bfin_write16(DMA3_X_COUNT,val)
#define bfin_read_DMA3_Y_COUNT()             bfin_read16(DMA3_Y_COUNT)
#define bfin_write_DMA3_Y_COUNT(val)         bfin_write16(DMA3_Y_COUNT,val)
#define bfin_read_DMA3_X_MODIFY()            bfin_read16(DMA3_X_MODIFY)
#define bfin_write_DMA3_X_MODIFY(val)        bfin_write16(DMA3_X_MODIFY,val)
#define bfin_read_DMA3_Y_MODIFY()            bfin_read16(DMA3_Y_MODIFY)
#define bfin_write_DMA3_Y_MODIFY(val)        bfin_write16(DMA3_Y_MODIFY,val)
#define bfin_read_DMA3_CURR_DESC_PTR()       bfin_read32(DMA3_CURR_DESC_PTR)
#define bfin_write_DMA3_CURR_DESC_PTR(val)   bfin_write32(DMA3_CURR_DESC_PTR,val)
#define bfin_read_DMA3_CURR_ADDR()           bfin_read32(DMA3_CURR_ADDR)
#define bfin_write_DMA3_CURR_ADDR(val)       bfin_write32(DMA3_CURR_ADDR,val)
#define bfin_read_DMA3_CURR_X_COUNT()        bfin_read16(DMA3_CURR_X_COUNT)
#define bfin_write_DMA3_CURR_X_COUNT(val)    bfin_write16(DMA3_CURR_X_COUNT,val)
#define bfin_read_DMA3_CURR_Y_COUNT()        bfin_read16(DMA3_CURR_Y_COUNT)
#define bfin_write_DMA3_CURR_Y_COUNT(val)    bfin_write16(DMA3_CURR_Y_COUNT,val)
#define bfin_read_DMA3_IRQ_STATUS()          bfin_read16(DMA3_IRQ_STATUS)
#define bfin_write_DMA3_IRQ_STATUS(val)      bfin_write16(DMA3_IRQ_STATUS,val)
#define bfin_read_DMA3_PERIPHERAL_MAP()      bfin_read16(DMA3_PERIPHERAL_MAP)
#define bfin_write_DMA3_PERIPHERAL_MAP(val)  bfin_write16(DMA3_PERIPHERAL_MAP,val)

#define bfin_read_DMA4_CONFIG()              bfin_read16(DMA4_CONFIG)
#define bfin_write_DMA4_CONFIG(val)          bfin_write16(DMA4_CONFIG,val)
#define bfin_read_DMA4_NEXT_DESC_PTR()       bfin_read32(DMA4_NEXT_DESC_PTR)
#define bfin_write_DMA4_NEXT_DESC_PTR(val)   bfin_write32(DMA4_NEXT_DESC_PTR,val)
#define bfin_read_DMA4_START_ADDR()          bfin_read32(DMA4_START_ADDR)
#define bfin_write_DMA4_START_ADDR(val)      bfin_write32(DMA4_START_ADDR,val)
#define bfin_read_DMA4_X_COUNT()             bfin_read16(DMA4_X_COUNT)
#define bfin_write_DMA4_X_COUNT(val)         bfin_write16(DMA4_X_COUNT,val)
#define bfin_read_DMA4_Y_COUNT()             bfin_read16(DMA4_Y_COUNT)
#define bfin_write_DMA4_Y_COUNT(val)         bfin_write16(DMA4_Y_COUNT,val)
#define bfin_read_DMA4_X_MODIFY()            bfin_read16(DMA4_X_MODIFY)
#define bfin_write_DMA4_X_MODIFY(val)        bfin_write16(DMA4_X_MODIFY,val)
#define bfin_read_DMA4_Y_MODIFY()            bfin_read16(DMA4_Y_MODIFY)
#define bfin_write_DMA4_Y_MODIFY(val)        bfin_write16(DMA4_Y_MODIFY,val)
#define bfin_read_DMA4_CURR_DESC_PTR()       bfin_read32(DMA4_CURR_DESC_PTR)
#define bfin_write_DMA4_CURR_DESC_PTR(val)   bfin_write32(DMA4_CURR_DESC_PTR,val)
#define bfin_read_DMA4_CURR_ADDR()           bfin_read32(DMA4_CURR_ADDR)
#define bfin_write_DMA4_CURR_ADDR(val)       bfin_write32(DMA4_CURR_ADDR,val)
#define bfin_read_DMA4_CURR_X_COUNT()        bfin_read16(DMA4_CURR_X_COUNT)
#define bfin_write_DMA4_CURR_X_COUNT(val)    bfin_write16(DMA4_CURR_X_COUNT,val)
#define bfin_read_DMA4_CURR_Y_COUNT()        bfin_read16(DMA4_CURR_Y_COUNT)
#define bfin_write_DMA4_CURR_Y_COUNT(val)    bfin_write16(DMA4_CURR_Y_COUNT,val)
#define bfin_read_DMA4_IRQ_STATUS()          bfin_read16(DMA4_IRQ_STATUS)
#define bfin_write_DMA4_IRQ_STATUS(val)      bfin_write16(DMA4_IRQ_STATUS,val)
#define bfin_read_DMA4_PERIPHERAL_MAP()      bfin_read16(DMA4_PERIPHERAL_MAP)
#define bfin_write_DMA4_PERIPHERAL_MAP(val)  bfin_write16(DMA4_PERIPHERAL_MAP,val)

#define bfin_read_DMA5_CONFIG()              bfin_read16(DMA5_CONFIG)
#define bfin_write_DMA5_CONFIG(val)          bfin_write16(DMA5_CONFIG,val)
#define bfin_read_DMA5_NEXT_DESC_PTR()       bfin_read32(DMA5_NEXT_DESC_PTR)
#define bfin_write_DMA5_NEXT_DESC_PTR(val)   bfin_write32(DMA5_NEXT_DESC_PTR,val)
#define bfin_read_DMA5_START_ADDR()          bfin_read32(DMA5_START_ADDR)
#define bfin_write_DMA5_START_ADDR(val)      bfin_write32(DMA5_START_ADDR,val)
#define bfin_read_DMA5_X_COUNT()             bfin_read16(DMA5_X_COUNT)
#define bfin_write_DMA5_X_COUNT(val)         bfin_write16(DMA5_X_COUNT,val)
#define bfin_read_DMA5_Y_COUNT()             bfin_read16(DMA5_Y_COUNT)
#define bfin_write_DMA5_Y_COUNT(val)         bfin_write16(DMA5_Y_COUNT,val)
#define bfin_read_DMA5_X_MODIFY()            bfin_read16(DMA5_X_MODIFY)
#define bfin_write_DMA5_X_MODIFY(val)        bfin_write16(DMA5_X_MODIFY,val)
#define bfin_read_DMA5_Y_MODIFY()            bfin_read16(DMA5_Y_MODIFY)
#define bfin_write_DMA5_Y_MODIFY(val)        bfin_write16(DMA5_Y_MODIFY,val)
#define bfin_read_DMA5_CURR_DESC_PTR()       bfin_read32(DMA5_CURR_DESC_PTR)
#define bfin_write_DMA5_CURR_DESC_PTR(val)   bfin_write32(DMA5_CURR_DESC_PTR,val)
#define bfin_read_DMA5_CURR_ADDR()           bfin_read32(DMA5_CURR_ADDR)
#define bfin_write_DMA5_CURR_ADDR(val)       bfin_write32(DMA5_CURR_ADDR,val)
#define bfin_read_DMA5_CURR_X_COUNT()        bfin_read16(DMA5_CURR_X_COUNT)
#define bfin_write_DMA5_CURR_X_COUNT(val)    bfin_write16(DMA5_CURR_X_COUNT,val)
#define bfin_read_DMA5_CURR_Y_COUNT()        bfin_read16(DMA5_CURR_Y_COUNT)
#define bfin_write_DMA5_CURR_Y_COUNT(val)    bfin_write16(DMA5_CURR_Y_COUNT,val)
#define bfin_read_DMA5_IRQ_STATUS()          bfin_read16(DMA5_IRQ_STATUS)
#define bfin_write_DMA5_IRQ_STATUS(val)      bfin_write16(DMA5_IRQ_STATUS,val)
#define bfin_read_DMA5_PERIPHERAL_MAP()      bfin_read16(DMA5_PERIPHERAL_MAP)
#define bfin_write_DMA5_PERIPHERAL_MAP(val)  bfin_write16(DMA5_PERIPHERAL_MAP,val)

#define bfin_read_DMA6_CONFIG()              bfin_read16(DMA6_CONFIG)
#define bfin_write_DMA6_CONFIG(val)          bfin_write16(DMA6_CONFIG,val)
#define bfin_read_DMA6_NEXT_DESC_PTR()       bfin_read32(DMA6_NEXT_DESC_PTR)
#define bfin_write_DMA6_NEXT_DESC_PTR(val)   bfin_write32(DMA6_NEXT_DESC_PTR,val)
#define bfin_read_DMA6_START_ADDR()          bfin_read32(DMA6_START_ADDR)
#define bfin_write_DMA6_START_ADDR(val)      bfin_write32(DMA6_START_ADDR,val)
#define bfin_read_DMA6_X_COUNT()             bfin_read16(DMA6_X_COUNT)
#define bfin_write_DMA6_X_COUNT(val)         bfin_write16(DMA6_X_COUNT,val)
#define bfin_read_DMA6_Y_COUNT()             bfin_read16(DMA6_Y_COUNT)
#define bfin_write_DMA6_Y_COUNT(val)         bfin_write16(DMA6_Y_COUNT,val)
#define bfin_read_DMA6_X_MODIFY()            bfin_read16(DMA6_X_MODIFY)
#define bfin_write_DMA6_X_MODIFY(val)        bfin_write16(DMA6_X_MODIFY,val)
#define bfin_read_DMA6_Y_MODIFY()            bfin_read16(DMA6_Y_MODIFY)
#define bfin_write_DMA6_Y_MODIFY(val)        bfin_write16(DMA6_Y_MODIFY,val)
#define bfin_read_DMA6_CURR_DESC_PTR()       bfin_read32(DMA6_CURR_DESC_PTR)
#define bfin_write_DMA6_CURR_DESC_PTR(val)   bfin_write32(DMA6_CURR_DESC_PTR,val)
#define bfin_read_DMA6_CURR_ADDR()           bfin_read32(DMA6_CURR_ADDR)
#define bfin_write_DMA6_CURR_ADDR(val)       bfin_write32(DMA6_CURR_ADDR,val)
#define bfin_read_DMA6_CURR_X_COUNT()        bfin_read16(DMA6_CURR_X_COUNT)
#define bfin_write_DMA6_CURR_X_COUNT(val)    bfin_write16(DMA6_CURR_X_COUNT,val)
#define bfin_read_DMA6_CURR_Y_COUNT()        bfin_read16(DMA6_CURR_Y_COUNT)
#define bfin_write_DMA6_CURR_Y_COUNT(val)    bfin_write16(DMA6_CURR_Y_COUNT,val)
#define bfin_read_DMA6_IRQ_STATUS()          bfin_read16(DMA6_IRQ_STATUS)
#define bfin_write_DMA6_IRQ_STATUS(val)      bfin_write16(DMA6_IRQ_STATUS,val)
#define bfin_read_DMA6_PERIPHERAL_MAP()      bfin_read16(DMA6_PERIPHERAL_MAP)
#define bfin_write_DMA6_PERIPHERAL_MAP(val)  bfin_write16(DMA6_PERIPHERAL_MAP,val)

#define bfin_read_DMA7_CONFIG()              bfin_read16(DMA7_CONFIG)
#define bfin_write_DMA7_CONFIG(val)          bfin_write16(DMA7_CONFIG,val)
#define bfin_read_DMA7_NEXT_DESC_PTR()       bfin_read32(DMA7_NEXT_DESC_PTR)
#define bfin_write_DMA7_NEXT_DESC_PTR(val)   bfin_write32(DMA7_NEXT_DESC_PTR,val)
#define bfin_read_DMA7_START_ADDR()          bfin_read32(DMA7_START_ADDR)
#define bfin_write_DMA7_START_ADDR(val)      bfin_write32(DMA7_START_ADDR,val)
#define bfin_read_DMA7_X_COUNT()             bfin_read16(DMA7_X_COUNT)
#define bfin_write_DMA7_X_COUNT(val)         bfin_write16(DMA7_X_COUNT,val)
#define bfin_read_DMA7_Y_COUNT()             bfin_read16(DMA7_Y_COUNT)
#define bfin_write_DMA7_Y_COUNT(val)         bfin_write16(DMA7_Y_COUNT,val)
#define bfin_read_DMA7_X_MODIFY()            bfin_read16(DMA7_X_MODIFY)
#define bfin_write_DMA7_X_MODIFY(val)        bfin_write16(DMA7_X_MODIFY,val)
#define bfin_read_DMA7_Y_MODIFY()            bfin_read16(DMA7_Y_MODIFY)
#define bfin_write_DMA7_Y_MODIFY(val)        bfin_write16(DMA7_Y_MODIFY,val)
#define bfin_read_DMA7_CURR_DESC_PTR()       bfin_read32(DMA7_CURR_DESC_PTR)
#define bfin_write_DMA7_CURR_DESC_PTR(val)   bfin_write32(DMA7_CURR_DESC_PTR,val)
#define bfin_read_DMA7_CURR_ADDR()           bfin_read32(DMA7_CURR_ADDR)
#define bfin_write_DMA7_CURR_ADDR(val)       bfin_write32(DMA7_CURR_ADDR,val)
#define bfin_read_DMA7_CURR_X_COUNT()        bfin_read16(DMA7_CURR_X_COUNT)
#define bfin_write_DMA7_CURR_X_COUNT(val)    bfin_write16(DMA7_CURR_X_COUNT,val)
#define bfin_read_DMA7_CURR_Y_COUNT()        bfin_read16(DMA7_CURR_Y_COUNT)
#define bfin_write_DMA7_CURR_Y_COUNT(val)    bfin_write16(DMA7_CURR_Y_COUNT,val)
#define bfin_read_DMA7_IRQ_STATUS()          bfin_read16(DMA7_IRQ_STATUS)
#define bfin_write_DMA7_IRQ_STATUS(val)      bfin_write16(DMA7_IRQ_STATUS,val)
#define bfin_read_DMA7_PERIPHERAL_MAP()      bfin_read16(DMA7_PERIPHERAL_MAP)
#define bfin_write_DMA7_PERIPHERAL_MAP(val)  bfin_write16(DMA7_PERIPHERAL_MAP,val)

#define bfin_read_MDMA_D1_CONFIG()           bfin_read16(MDMA_D1_CONFIG)
#define bfin_write_MDMA_D1_CONFIG(val)       bfin_write16(MDMA_D1_CONFIG,val)
#define bfin_read_MDMA_D1_NEXT_DESC_PTR()    bfin_read32(MDMA_D1_NEXT_DESC_PTR)
#define bfin_write_MDMA_D1_NEXT_DESC_PTR(val) bfin_write32(MDMA_D1_NEXT_DESC_PTR,val)
#define bfin_read_MDMA_D1_START_ADDR()       bfin_read32(MDMA_D1_START_ADDR)
#define bfin_write_MDMA_D1_START_ADDR(val)   bfin_write32(MDMA_D1_START_ADDR,val)
#define bfin_read_MDMA_D1_X_COUNT()          bfin_read16(MDMA_D1_X_COUNT)
#define bfin_write_MDMA_D1_X_COUNT(val)      bfin_write16(MDMA_D1_X_COUNT,val)
#define bfin_read_MDMA_D1_Y_COUNT()          bfin_read16(MDMA_D1_Y_COUNT)
#define bfin_write_MDMA_D1_Y_COUNT(val)      bfin_write16(MDMA_D1_Y_COUNT,val)
#define bfin_read_MDMA_D1_X_MODIFY()         bfin_read16(MDMA_D1_X_MODIFY)
#define bfin_write_MDMA_D1_X_MODIFY(val)     bfin_write16(MDMA_D1_X_MODIFY,val)
#define bfin_read_MDMA_D1_Y_MODIFY()         bfin_read16(MDMA_D1_Y_MODIFY)
#define bfin_write_MDMA_D1_Y_MODIFY(val)     bfin_write16(MDMA_D1_Y_MODIFY,val)
#define bfin_read_MDMA_D1_CURR_DESC_PTR()    bfin_read32(MDMA_D1_CURR_DESC_PTR)
#define bfin_write_MDMA_D1_CURR_DESC_PTR(val) bfin_write32(MDMA_D1_CURR_DESC_PTR,val)
#define bfin_read_MDMA_D1_CURR_ADDR()        bfin_read32(MDMA_D1_CURR_ADDR)
#define bfin_write_MDMA_D1_CURR_ADDR(val)    bfin_write32(MDMA_D1_CURR_ADDR,val)
#define bfin_read_MDMA_D1_CURR_X_COUNT()     bfin_read16(MDMA_D1_CURR_X_COUNT)
#define bfin_write_MDMA_D1_CURR_X_COUNT(val) bfin_write16(MDMA_D1_CURR_X_COUNT,val)
#define bfin_read_MDMA_D1_CURR_Y_COUNT()     bfin_read16(MDMA_D1_CURR_Y_COUNT)
#define bfin_write_MDMA_D1_CURR_Y_COUNT(val) bfin_write16(MDMA_D1_CURR_Y_COUNT,val)
#define bfin_read_MDMA_D1_IRQ_STATUS()       bfin_read16(MDMA_D1_IRQ_STATUS)
#define bfin_write_MDMA_D1_IRQ_STATUS(val)   bfin_write16(MDMA_D1_IRQ_STATUS,val)
#define bfin_read_MDMA_D1_PERIPHERAL_MAP()   bfin_read16(MDMA_D1_PERIPHERAL_MAP)
#define bfin_write_MDMA_D1_PERIPHERAL_MAP(val) bfin_write16(MDMA_D1_PERIPHERAL_MAP,val)

#define bfin_read_MDMA_S1_CONFIG()           bfin_read16(MDMA_S1_CONFIG)
#define bfin_write_MDMA_S1_CONFIG(val)       bfin_write16(MDMA_S1_CONFIG,val)
#define bfin_read_MDMA_S1_NEXT_DESC_PTR()    bfin_read32(MDMA_S1_NEXT_DESC_PTR)
#define bfin_write_MDMA_S1_NEXT_DESC_PTR(val) bfin_write32(MDMA_S1_NEXT_DESC_PTR,val)
#define bfin_read_MDMA_S1_START_ADDR()       bfin_read32(MDMA_S1_START_ADDR)
#define bfin_write_MDMA_S1_START_ADDR(val)   bfin_write32(MDMA_S1_START_ADDR,val)
#define bfin_read_MDMA_S1_X_COUNT()          bfin_read16(MDMA_S1_X_COUNT)
#define bfin_write_MDMA_S1_X_COUNT(val)      bfin_write16(MDMA_S1_X_COUNT,val)
#define bfin_read_MDMA_S1_Y_COUNT()          bfin_read16(MDMA_S1_Y_COUNT)
#define bfin_write_MDMA_S1_Y_COUNT(val)      bfin_write16(MDMA_S1_Y_COUNT,val)
#define bfin_read_MDMA_S1_X_MODIFY()         bfin_read16(MDMA_S1_X_MODIFY)
#define bfin_write_MDMA_S1_X_MODIFY(val)     bfin_write16(MDMA_S1_X_MODIFY,val)
#define bfin_read_MDMA_S1_Y_MODIFY()         bfin_read16(MDMA_S1_Y_MODIFY)
#define bfin_write_MDMA_S1_Y_MODIFY(val)     bfin_write16(MDMA_S1_Y_MODIFY,val)
#define bfin_read_MDMA_S1_CURR_DESC_PTR()    bfin_read32(MDMA_S1_CURR_DESC_PTR)
#define bfin_write_MDMA_S1_CURR_DESC_PTR(val) bfin_write32(MDMA_S1_CURR_DESC_PTR,val)
#define bfin_read_MDMA_S1_CURR_ADDR()        bfin_read32(MDMA_S1_CURR_ADDR)
#define bfin_write_MDMA_S1_CURR_ADDR(val)    bfin_write32(MDMA_S1_CURR_ADDR,val)
#define bfin_read_MDMA_S1_CURR_X_COUNT()     bfin_read16(MDMA_S1_CURR_X_COUNT)
#define bfin_write_MDMA_S1_CURR_X_COUNT(val) bfin_write16(MDMA_S1_CURR_X_COUNT,val)
#define bfin_read_MDMA_S1_CURR_Y_COUNT()     bfin_read16(MDMA_S1_CURR_Y_COUNT)
#define bfin_write_MDMA_S1_CURR_Y_COUNT(val) bfin_write16(MDMA_S1_CURR_Y_COUNT,val)
#define bfin_read_MDMA_S1_IRQ_STATUS()       bfin_read16(MDMA_S1_IRQ_STATUS)
#define bfin_write_MDMA_S1_IRQ_STATUS(val)   bfin_write16(MDMA_S1_IRQ_STATUS,val)
#define bfin_read_MDMA_S1_PERIPHERAL_MAP()   bfin_read16(MDMA_S1_PERIPHERAL_MAP)
#define bfin_write_MDMA_S1_PERIPHERAL_MAP(val) bfin_write16(MDMA_S1_PERIPHERAL_MAP,val)

#define bfin_read_MDMA_D0_CONFIG()           bfin_read16(MDMA_D0_CONFIG)
#define bfin_write_MDMA_D0_CONFIG(val)       bfin_write16(MDMA_D0_CONFIG,val)
#define bfin_read_MDMA_D0_NEXT_DESC_PTR()    bfin_read32(MDMA_D0_NEXT_DESC_PTR)
#define bfin_write_MDMA_D0_NEXT_DESC_PTR(val) bfin_write32(MDMA_D0_NEXT_DESC_PTR,val)
#define bfin_read_MDMA_D0_START_ADDR()       bfin_read32(MDMA_D0_START_ADDR)
#define bfin_write_MDMA_D0_START_ADDR(val)   bfin_write32(MDMA_D0_START_ADDR,val)
#define bfin_read_MDMA_D0_X_COUNT()          bfin_read16(MDMA_D0_X_COUNT)
#define bfin_write_MDMA_D0_X_COUNT(val)      bfin_write16(MDMA_D0_X_COUNT,val)
#define bfin_read_MDMA_D0_Y_COUNT()          bfin_read16(MDMA_D0_Y_COUNT)
#define bfin_write_MDMA_D0_Y_COUNT(val)      bfin_write16(MDMA_D0_Y_COUNT,val)
#define bfin_read_MDMA_D0_X_MODIFY()         bfin_read16(MDMA_D0_X_MODIFY)
#define bfin_write_MDMA_D0_X_MODIFY(val)     bfin_write16(MDMA_D0_X_MODIFY,val)
#define bfin_read_MDMA_D0_Y_MODIFY()         bfin_read16(MDMA_D0_Y_MODIFY)
#define bfin_write_MDMA_D0_Y_MODIFY(val)     bfin_write16(MDMA_D0_Y_MODIFY,val)
#define bfin_read_MDMA_D0_CURR_DESC_PTR()    bfin_read32(MDMA_D0_CURR_DESC_PTR)
#define bfin_write_MDMA_D0_CURR_DESC_PTR(val) bfin_write32(MDMA_D0_CURR_DESC_PTR,val)
#define bfin_read_MDMA_D0_CURR_ADDR()        bfin_read32(MDMA_D0_CURR_ADDR)
#define bfin_write_MDMA_D0_CURR_ADDR(val)    bfin_write32(MDMA_D0_CURR_ADDR,val)
#define bfin_read_MDMA_D0_CURR_X_COUNT()     bfin_read16(MDMA_D0_CURR_X_COUNT)
#define bfin_write_MDMA_D0_CURR_X_COUNT(val) bfin_write16(MDMA_D0_CURR_X_COUNT,val)
#define bfin_read_MDMA_D0_CURR_Y_COUNT()     bfin_read16(MDMA_D0_CURR_Y_COUNT)
#define bfin_write_MDMA_D0_CURR_Y_COUNT(val) bfin_write16(MDMA_D0_CURR_Y_COUNT,val)
#define bfin_read_MDMA_D0_IRQ_STATUS()       bfin_read16(MDMA_D0_IRQ_STATUS)
#define bfin_write_MDMA_D0_IRQ_STATUS(val)   bfin_write16(MDMA_D0_IRQ_STATUS,val)
#define bfin_read_MDMA_D0_PERIPHERAL_MAP()   bfin_read16(MDMA_D0_PERIPHERAL_MAP)
#define bfin_write_MDMA_D0_PERIPHERAL_MAP(val) bfin_write16(MDMA_D0_PERIPHERAL_MAP,val)

#define bfin_read_MDMA_S0_CONFIG()           bfin_read16(MDMA_S0_CONFIG)
#define bfin_write_MDMA_S0_CONFIG(val)       bfin_write16(MDMA_S0_CONFIG,val)
#define bfin_read_MDMA_S0_NEXT_DESC_PTR()    bfin_read32(MDMA_S0_NEXT_DESC_PTR)
#define bfin_write_MDMA_S0_NEXT_DESC_PTR(val) bfin_write32(MDMA_S0_NEXT_DESC_PTR,val)
#define bfin_read_MDMA_S0_START_ADDR()       bfin_read32(MDMA_S0_START_ADDR)
#define bfin_write_MDMA_S0_START_ADDR(val)   bfin_write32(MDMA_S0_START_ADDR,val)
#define bfin_read_MDMA_S0_X_COUNT()          bfin_read16(MDMA_S0_X_COUNT)
#define bfin_write_MDMA_S0_X_COUNT(val)      bfin_write16(MDMA_S0_X_COUNT,val)
#define bfin_read_MDMA_S0_Y_COUNT()          bfin_read16(MDMA_S0_Y_COUNT)
#define bfin_write_MDMA_S0_Y_COUNT(val)      bfin_write16(MDMA_S0_Y_COUNT,val)
#define bfin_read_MDMA_S0_X_MODIFY()         bfin_read16(MDMA_S0_X_MODIFY)
#define bfin_write_MDMA_S0_X_MODIFY(val)     bfin_write16(MDMA_S0_X_MODIFY,val)
#define bfin_read_MDMA_S0_Y_MODIFY()         bfin_read16(MDMA_S0_Y_MODIFY)
#define bfin_write_MDMA_S0_Y_MODIFY(val)     bfin_write16(MDMA_S0_Y_MODIFY,val)
#define bfin_read_MDMA_S0_CURR_DESC_PTR()    bfin_read32(MDMA_S0_CURR_DESC_PTR)
#define bfin_write_MDMA_S0_CURR_DESC_PTR(val) bfin_write32(MDMA_S0_CURR_DESC_PTR,val)
#define bfin_read_MDMA_S0_CURR_ADDR()        bfin_read32(MDMA_S0_CURR_ADDR)
#define bfin_write_MDMA_S0_CURR_ADDR(val)    bfin_write32(MDMA_S0_CURR_ADDR,val)
#define bfin_read_MDMA_S0_CURR_X_COUNT()     bfin_read16(MDMA_S0_CURR_X_COUNT)
#define bfin_write_MDMA_S0_CURR_X_COUNT(val) bfin_write16(MDMA_S0_CURR_X_COUNT,val)
#define bfin_read_MDMA_S0_CURR_Y_COUNT()     bfin_read16(MDMA_S0_CURR_Y_COUNT)
#define bfin_write_MDMA_S0_CURR_Y_COUNT(val) bfin_write16(MDMA_S0_CURR_Y_COUNT,val)
#define bfin_read_MDMA_S0_IRQ_STATUS()       bfin_read16(MDMA_S0_IRQ_STATUS)
#define bfin_write_MDMA_S0_IRQ_STATUS(val)   bfin_write16(MDMA_S0_IRQ_STATUS,val)
#define bfin_read_MDMA_S0_PERIPHERAL_MAP()   bfin_read16(MDMA_S0_PERIPHERAL_MAP)
#define bfin_write_MDMA_S0_PERIPHERAL_MAP(val) bfin_write16(MDMA_S0_PERIPHERAL_MAP,val)

/* Aysnchronous Memory Controller - External Bus Interface Unit (0xFFC0 3C00-0xFFC0 3FFF) */
#define bfin_read_EBIU_AMGCTL()              bfin_read16(EBIU_AMGCTL)
#define bfin_write_EBIU_AMGCTL(val)          bfin_write16(EBIU_AMGCTL,val)
#define bfin_read_EBIU_AMBCTL0()             bfin_read32(EBIU_AMBCTL0)
#define bfin_write_EBIU_AMBCTL0(val)         bfin_write32(EBIU_AMBCTL0,val)
#define bfin_read_EBIU_AMBCTL1()             bfin_read32(EBIU_AMBCTL1)
#define bfin_write_EBIU_AMBCTL1(val)         bfin_write32(EBIU_AMBCTL1,val)

/* SDRAM Controller External Bus Interface Unit (0xFFC0 4C00-0xFFC0 4FFF) */
#define bfin_read_EBIU_SDGCTL()              bfin_read32(EBIU_SDGCTL)
#define bfin_write_EBIU_SDGCTL(val)          bfin_write32(EBIU_SDGCTL,val)
#define bfin_read_EBIU_SDRRC()               bfin_read16(EBIU_SDRRC)
#define bfin_write_EBIU_SDRRC(val)           bfin_write16(EBIU_SDRRC,val)
#define bfin_read_EBIU_SDSTAT()              bfin_read16(EBIU_SDSTAT)
#define bfin_write_EBIU_SDSTAT(val)          bfin_write16(EBIU_SDSTAT,val)
#define bfin_read_EBIU_SDBCTL()              bfin_read16(EBIU_SDBCTL)
#define bfin_write_EBIU_SDBCTL(val)          bfin_write16(EBIU_SDBCTL,val)

/* UART Controller */
#define bfin_read_UART_THR()                 bfin_read16(UART_THR)
#define bfin_write_UART_THR(val)             bfin_write16(UART_THR,val)
#define bfin_read_UART_RBR()                 bfin_read16(UART_RBR)
#define bfin_write_UART_RBR(val)             bfin_write16(UART_RBR,val)
#define bfin_read_UART_DLL()                 bfin_read16(UART_DLL)
#define bfin_write_UART_DLL(val)             bfin_write16(UART_DLL,val)
#define bfin_read_UART_IER()                 bfin_read16(UART_IER)
#define bfin_write_UART_IER(val)             bfin_write16(UART_IER,val)
#define bfin_read_UART_DLH()                 bfin_read16(UART_DLH)
#define bfin_write_UART_DLH(val)             bfin_write16(UART_DLH,val)
#define bfin_read_UART_IIR()                 bfin_read16(UART_IIR)
#define bfin_write_UART_IIR(val)             bfin_write16(UART_IIR,val)
#define bfin_read_UART_LCR()                 bfin_read16(UART_LCR)
#define bfin_write_UART_LCR(val)             bfin_write16(UART_LCR,val)
#define bfin_read_UART_MCR()                 bfin_read16(UART_MCR)
#define bfin_write_UART_MCR(val)             bfin_write16(UART_MCR,val)
#define bfin_read_UART_LSR()                 bfin_read16(UART_LSR)
#define bfin_write_UART_LSR(val)             bfin_write16(UART_LSR,val)
/*
#define UART_MSR
*/
#define bfin_read_UART_SCR()                 bfin_read16(UART_SCR)
#define bfin_write_UART_SCR(val)             bfin_write16(UART_SCR,val)
#define bfin_read_UART_GCTL()                bfin_read16(UART_GCTL)
#define bfin_write_UART_GCTL(val)            bfin_write16(UART_GCTL,val)

/* SPI Controller */
#define bfin_read_SPI_CTL()                  bfin_read16(SPI_CTL)
#define bfin_write_SPI_CTL(val)              bfin_write16(SPI_CTL,val)
#define bfin_read_SPI_FLG()                  bfin_read16(SPI_FLG)
#define bfin_write_SPI_FLG(val)              bfin_write16(SPI_FLG,val)
#define bfin_read_SPI_STAT()                 bfin_read16(SPI_STAT)
#define bfin_write_SPI_STAT(val)             bfin_write16(SPI_STAT,val)
#define bfin_read_SPI_TDBR()                 bfin_read16(SPI_TDBR)
#define bfin_write_SPI_TDBR(val)             bfin_write16(SPI_TDBR,val)
#define bfin_read_SPI_RDBR()                 bfin_read16(SPI_RDBR)
#define bfin_write_SPI_RDBR(val)             bfin_write16(SPI_RDBR,val)
#define bfin_read_SPI_BAUD()                 bfin_read16(SPI_BAUD)
#define bfin_write_SPI_BAUD(val)             bfin_write16(SPI_BAUD,val)
#define bfin_read_SPI_SHADOW()               bfin_read16(SPI_SHADOW)
#define bfin_write_SPI_SHADOW(val)           bfin_write16(SPI_SHADOW,val)

/* TIMER 0, 1, 2 Registers */
#define bfin_read_TIMER0_CONFIG()            bfin_read16(TIMER0_CONFIG)
#define bfin_write_TIMER0_CONFIG(val)        bfin_write16(TIMER0_CONFIG,val)
#define bfin_read_TIMER0_COUNTER()           bfin_read32(TIMER0_COUNTER)
#define bfin_write_TIMER0_COUNTER(val)       bfin_write32(TIMER0_COUNTER,val)
#define bfin_read_TIMER0_PERIOD()            bfin_read32(TIMER0_PERIOD)
#define bfin_write_TIMER0_PERIOD(val)        bfin_write32(TIMER0_PERIOD,val)
#define bfin_read_TIMER0_WIDTH()             bfin_read32(TIMER0_WIDTH)
#define bfin_write_TIMER0_WIDTH(val)         bfin_write32(TIMER0_WIDTH,val)

#define bfin_read_TIMER1_CONFIG()            bfin_read16(TIMER1_CONFIG)
#define bfin_write_TIMER1_CONFIG(val)        bfin_write16(TIMER1_CONFIG,val)
#define bfin_read_TIMER1_COUNTER()           bfin_read32(TIMER1_COUNTER)
#define bfin_write_TIMER1_COUNTER(val)       bfin_write32(TIMER1_COUNTER,val)
#define bfin_read_TIMER1_PERIOD()            bfin_read32(TIMER1_PERIOD)
#define bfin_write_TIMER1_PERIOD(val)        bfin_write32(TIMER1_PERIOD,val)
#define bfin_read_TIMER1_WIDTH()             bfin_read32(TIMER1_WIDTH)
#define bfin_write_TIMER1_WIDTH(val)         bfin_write32(TIMER1_WIDTH,val)

#define bfin_read_TIMER2_CONFIG()            bfin_read16(TIMER2_CONFIG)
#define bfin_write_TIMER2_CONFIG(val)        bfin_write16(TIMER2_CONFIG,val)
#define bfin_read_TIMER2_COUNTER()           bfin_read32(TIMER2_COUNTER)
#define bfin_write_TIMER2_COUNTER(val)       bfin_write32(TIMER2_COUNTER,val)
#define bfin_read_TIMER2_PERIOD()            bfin_read32(TIMER2_PERIOD)
#define bfin_write_TIMER2_PERIOD(val)        bfin_write32(TIMER2_PERIOD,val)
#define bfin_read_TIMER2_WIDTH()             bfin_read32(TIMER2_WIDTH)
#define bfin_write_TIMER2_WIDTH(val)         bfin_write32(TIMER2_WIDTH,val)

#define bfin_read_TIMER_ENABLE()             bfin_read16(TIMER_ENABLE)
#define bfin_write_TIMER_ENABLE(val)         bfin_write16(TIMER_ENABLE,val)
#define bfin_read_TIMER_DISABLE()            bfin_read16(TIMER_DISABLE)
#define bfin_write_TIMER_DISABLE(val)        bfin_write16(TIMER_DISABLE,val)
#define bfin_read_TIMER_STATUS()             bfin_read16(TIMER_STATUS)
#define bfin_write_TIMER_STATUS(val)         bfin_write16(TIMER_STATUS,val)

/* SPORT0 Controller */
#define bfin_read_SPORT0_TCR1()              bfin_read16(SPORT0_TCR1)
#define bfin_write_SPORT0_TCR1(val)          bfin_write16(SPORT0_TCR1,val)
#define bfin_read_SPORT0_TCR2()              bfin_read16(SPORT0_TCR2)
#define bfin_write_SPORT0_TCR2(val)          bfin_write16(SPORT0_TCR2,val)
#define bfin_read_SPORT0_TCLKDIV()           bfin_read16(SPORT0_TCLKDIV)
#define bfin_write_SPORT0_TCLKDIV(val)       bfin_write16(SPORT0_TCLKDIV,val)
#define bfin_read_SPORT0_TFSDIV()            bfin_read16(SPORT0_TFSDIV)
#define bfin_write_SPORT0_TFSDIV(val)        bfin_write16(SPORT0_TFSDIV,val)
#define bfin_read_SPORT0_TX()                bfin_read32(SPORT0_TX)
#define bfin_write_SPORT0_TX(val)            bfin_write32(SPORT0_TX,val)
#define bfin_read_SPORT0_RX()                bfin_read32(SPORT0_RX)
#define bfin_write_SPORT0_RX(val)            bfin_write32(SPORT0_RX,val)
#define bfin_read_SPORT0_TX32()              bfin_read32(SPORT0_TX)
#define bfin_write_SPORT0_TX32(val)          bfin_write32(SPORT0_TX,val)
#define bfin_read_SPORT0_RX32()              bfin_read32(SPORT0_RX)
#define bfin_write_SPORT0_RX32(val)          bfin_write32(SPORT0_RX,val)
#define bfin_read_SPORT0_TX16()              bfin_read16(SPORT0_TX)
#define bfin_write_SPORT0_TX16(val)          bfin_write16(SPORT0_TX,val)
#define bfin_read_SPORT0_RX16()              bfin_read16(SPORT0_RX)
#define bfin_write_SPORT0_RX16(val)          bfin_write16(SPORT0_RX,val)
#define bfin_read_SPORT0_RCR1()              bfin_read16(SPORT0_RCR1)
#define bfin_write_SPORT0_RCR1(val)          bfin_write16(SPORT0_RCR1,val)
#define bfin_read_SPORT0_RCR2()              bfin_read16(SPORT0_RCR2)
#define bfin_write_SPORT0_RCR2(val)          bfin_write16(SPORT0_RCR2,val)
#define bfin_read_SPORT0_RCLKDIV()           bfin_read16(SPORT0_RCLKDIV)
#define bfin_write_SPORT0_RCLKDIV(val)       bfin_write16(SPORT0_RCLKDIV,val)
#define bfin_read_SPORT0_RFSDIV()            bfin_read16(SPORT0_RFSDIV)
#define bfin_write_SPORT0_RFSDIV(val)        bfin_write16(SPORT0_RFSDIV,val)
#define bfin_read_SPORT0_STAT()              bfin_read16(SPORT0_STAT)
#define bfin_write_SPORT0_STAT(val)          bfin_write16(SPORT0_STAT,val)
#define bfin_read_SPORT0_CHNL()              bfin_read16(SPORT0_CHNL)
#define bfin_write_SPORT0_CHNL(val)          bfin_write16(SPORT0_CHNL,val)
#define bfin_read_SPORT0_MCMC1()             bfin_read16(SPORT0_MCMC1)
#define bfin_write_SPORT0_MCMC1(val)         bfin_write16(SPORT0_MCMC1,val)
#define bfin_read_SPORT0_MCMC2()             bfin_read16(SPORT0_MCMC2)
#define bfin_write_SPORT0_MCMC2(val)         bfin_write16(SPORT0_MCMC2,val)
#define bfin_read_SPORT0_MTCS0()             bfin_read32(SPORT0_MTCS0)
#define bfin_write_SPORT0_MTCS0(val)         bfin_write32(SPORT0_MTCS0,val)
#define bfin_read_SPORT0_MTCS1()             bfin_read32(SPORT0_MTCS1)
#define bfin_write_SPORT0_MTCS1(val)         bfin_write32(SPORT0_MTCS1,val)
#define bfin_read_SPORT0_MTCS2()             bfin_read32(SPORT0_MTCS2)
#define bfin_write_SPORT0_MTCS2(val)         bfin_write32(SPORT0_MTCS2,val)
#define bfin_read_SPORT0_MTCS3()             bfin_read32(SPORT0_MTCS3)
#define bfin_write_SPORT0_MTCS3(val)         bfin_write32(SPORT0_MTCS3,val)
#define bfin_read_SPORT0_MRCS0()             bfin_read32(SPORT0_MRCS0)
#define bfin_write_SPORT0_MRCS0(val)         bfin_write32(SPORT0_MRCS0,val)
#define bfin_read_SPORT0_MRCS1()             bfin_read32(SPORT0_MRCS1)
#define bfin_write_SPORT0_MRCS1(val)         bfin_write32(SPORT0_MRCS1,val)
#define bfin_read_SPORT0_MRCS2()             bfin_read32(SPORT0_MRCS2)
#define bfin_write_SPORT0_MRCS2(val)         bfin_write32(SPORT0_MRCS2,val)
#define bfin_read_SPORT0_MRCS3()             bfin_read32(SPORT0_MRCS3)
#define bfin_write_SPORT0_MRCS3(val)         bfin_write32(SPORT0_MRCS3,val)

/* SPORT1 Controller */
#define bfin_read_SPORT1_TCR1()              bfin_read16(SPORT1_TCR1)
#define bfin_write_SPORT1_TCR1(val)          bfin_write16(SPORT1_TCR1,val)
#define bfin_read_SPORT1_TCR2()              bfin_read16(SPORT1_TCR2)
#define bfin_write_SPORT1_TCR2(val)          bfin_write16(SPORT1_TCR2,val)
#define bfin_read_SPORT1_TCLKDIV()           bfin_read16(SPORT1_TCLKDIV)
#define bfin_write_SPORT1_TCLKDIV(val)       bfin_write16(SPORT1_TCLKDIV,val)
#define bfin_read_SPORT1_TFSDIV()            bfin_read16(SPORT1_TFSDIV)
#define bfin_write_SPORT1_TFSDIV(val)        bfin_write16(SPORT1_TFSDIV,val)
#define bfin_read_SPORT1_TX()                bfin_read32(SPORT1_TX)
#define bfin_write_SPORT1_TX(val)            bfin_write32(SPORT1_TX,val)
#define bfin_read_SPORT1_RX()                bfin_read32(SPORT1_RX)
#define bfin_write_SPORT1_RX(val)            bfin_write32(SPORT1_RX,val)
#define bfin_read_SPORT1_TX32()              bfin_read32(SPORT1_TX)
#define bfin_write_SPORT1_TX32(val)          bfin_write32(SPORT1_TX,val)
#define bfin_read_SPORT1_RX32()              bfin_read32(SPORT1_RX)
#define bfin_write_SPORT1_RX32(val)          bfin_write32(SPORT1_RX,val)
#define bfin_read_SPORT1_TX16()              bfin_read16(SPORT1_TX)
#define bfin_write_SPORT1_TX16(val)          bfin_write16(SPORT1_TX,val)
#define bfin_read_SPORT1_RX16()              bfin_read16(SPORT1_RX)
#define bfin_write_SPORT1_RX16(val)          bfin_write16(SPORT1_RX,val)
#define bfin_read_SPORT1_RCR1()              bfin_read16(SPORT1_RCR1)
#define bfin_write_SPORT1_RCR1(val)          bfin_write16(SPORT1_RCR1,val)
#define bfin_read_SPORT1_RCR2()              bfin_read16(SPORT1_RCR2)
#define bfin_write_SPORT1_RCR2(val)          bfin_write16(SPORT1_RCR2,val)
#define bfin_read_SPORT1_RCLKDIV()           bfin_read16(SPORT1_RCLKDIV)
#define bfin_write_SPORT1_RCLKDIV(val)       bfin_write16(SPORT1_RCLKDIV,val)
#define bfin_read_SPORT1_RFSDIV()            bfin_read16(SPORT1_RFSDIV)
#define bfin_write_SPORT1_RFSDIV(val)        bfin_write16(SPORT1_RFSDIV,val)
#define bfin_read_SPORT1_STAT()              bfin_read16(SPORT1_STAT)
#define bfin_write_SPORT1_STAT(val)          bfin_write16(SPORT1_STAT,val)
#define bfin_read_SPORT1_CHNL()              bfin_read16(SPORT1_CHNL)
#define bfin_write_SPORT1_CHNL(val)          bfin_write16(SPORT1_CHNL,val)
#define bfin_read_SPORT1_MCMC1()             bfin_read16(SPORT1_MCMC1)
#define bfin_write_SPORT1_MCMC1(val)         bfin_write16(SPORT1_MCMC1,val)
#define bfin_read_SPORT1_MCMC2()             bfin_read16(SPORT1_MCMC2)
#define bfin_write_SPORT1_MCMC2(val)         bfin_write16(SPORT1_MCMC2,val)
#define bfin_read_SPORT1_MTCS0()             bfin_read32(SPORT1_MTCS0)
#define bfin_write_SPORT1_MTCS0(val)         bfin_write32(SPORT1_MTCS0,val)
#define bfin_read_SPORT1_MTCS1()             bfin_read32(SPORT1_MTCS1)
#define bfin_write_SPORT1_MTCS1(val)         bfin_write32(SPORT1_MTCS1,val)
#define bfin_read_SPORT1_MTCS2()             bfin_read32(SPORT1_MTCS2)
#define bfin_write_SPORT1_MTCS2(val)         bfin_write32(SPORT1_MTCS2,val)
#define bfin_read_SPORT1_MTCS3()             bfin_read32(SPORT1_MTCS3)
#define bfin_write_SPORT1_MTCS3(val)         bfin_write32(SPORT1_MTCS3,val)
#define bfin_read_SPORT1_MRCS0()             bfin_read32(SPORT1_MRCS0)
#define bfin_write_SPORT1_MRCS0(val)         bfin_write32(SPORT1_MRCS0,val)
#define bfin_read_SPORT1_MRCS1()             bfin_read32(SPORT1_MRCS1)
#define bfin_write_SPORT1_MRCS1(val)         bfin_write32(SPORT1_MRCS1,val)
#define bfin_read_SPORT1_MRCS2()             bfin_read32(SPORT1_MRCS2)
#define bfin_write_SPORT1_MRCS2(val)         bfin_write32(SPORT1_MRCS2,val)
#define bfin_read_SPORT1_MRCS3()             bfin_read32(SPORT1_MRCS3)
#define bfin_write_SPORT1_MRCS3(val)         bfin_write32(SPORT1_MRCS3,val)

/* Parallel Peripheral Interface (PPI) */
#define bfin_read_PPI_CONTROL()              bfin_read16(PPI_CONTROL)
#define bfin_write_PPI_CONTROL(val)          bfin_write16(PPI_CONTROL,val)
#define bfin_read_PPI_STATUS()               bfin_read16(PPI_STATUS)
#define bfin_write_PPI_STATUS(val)           bfin_write16(PPI_STATUS,val)
#define bfin_clear_PPI_STATUS()              bfin_read_PPI_STATUS()
#define bfin_read_PPI_DELAY()                bfin_read16(PPI_DELAY)
#define bfin_write_PPI_DELAY(val)            bfin_write16(PPI_DELAY,val)
#define bfin_read_PPI_COUNT()                bfin_read16(PPI_COUNT)
#define bfin_write_PPI_COUNT(val)            bfin_write16(PPI_COUNT,val)
#define bfin_read_PPI_FRAME()                bfin_read16(PPI_FRAME)
#define bfin_write_PPI_FRAME(val)            bfin_write16(PPI_FRAME,val)

/* These need to be last due to the cdef/linux inter-dependencies */
#include <asm/irq.h>

#if ANOMALY_05000311
#define BFIN_WRITE_FIO_FLAG(name) \
static inline void bfin_write_FIO_FLAG_##name(unsigned short val) \
{ \
	unsigned long flags; \
	local_irq_save_hw(flags); \
	bfin_write16(FIO_FLAG_##name, val); \
	bfin_read_CHIPID(); \
	local_irq_restore_hw(flags); \
}
BFIN_WRITE_FIO_FLAG(D)
BFIN_WRITE_FIO_FLAG(C)
BFIN_WRITE_FIO_FLAG(S)
BFIN_WRITE_FIO_FLAG(T)

#define BFIN_READ_FIO_FLAG(name) \
static inline u16 bfin_read_FIO_FLAG_##name(void) \
{ \
	unsigned long flags; \
	u16 ret; \
	local_irq_save_hw(flags); \
	ret = bfin_read16(FIO_FLAG_##name); \
	bfin_read_CHIPID(); \
	local_irq_restore_hw(flags); \
	return ret; \
}
BFIN_READ_FIO_FLAG(D)
BFIN_READ_FIO_FLAG(C)
BFIN_READ_FIO_FLAG(S)
BFIN_READ_FIO_FLAG(T)

#else
#define bfin_write_FIO_FLAG_D(val)           bfin_write16(FIO_FLAG_D, val)
#define bfin_write_FIO_FLAG_C(val)           bfin_write16(FIO_FLAG_C, val)
#define bfin_write_FIO_FLAG_S(val)           bfin_write16(FIO_FLAG_S, val)
#define bfin_write_FIO_FLAG_T(val)           bfin_write16(FIO_FLAG_T, val)
#define bfin_read_FIO_FLAG_T()               bfin_read16(FIO_FLAG_T)
#define bfin_read_FIO_FLAG_C()               bfin_read16(FIO_FLAG_C)
#define bfin_read_FIO_FLAG_S()               bfin_read16(FIO_FLAG_S)
#define bfin_read_FIO_FLAG_D()               bfin_read16(FIO_FLAG_D)
#endif

/* Writing to PLL_CTL initiates a PLL relock sequence. */
static __inline__ void bfin_write_PLL_CTL(unsigned int val)
{
	unsigned long flags, iwr;

	if (val == bfin_read_PLL_CTL())
		return;

	local_irq_save_hw(flags);
	/* Enable the PLL Wakeup bit in SIC IWR */
	iwr = bfin_read32(SIC_IWR);
	/* Only allow PPL Wakeup) */
	bfin_write32(SIC_IWR, IWR_ENABLE(0));

	bfin_write16(PLL_CTL, val);
	SSYNC();
	asm("IDLE;");

	bfin_write32(SIC_IWR, iwr);
	local_irq_restore_hw(flags);
}

/* Writing to VR_CTL initiates a PLL relock sequence. */
static __inline__ void bfin_write_VR_CTL(unsigned int val)
{
	unsigned long flags, iwr;

	if (val == bfin_read_VR_CTL())
		return;

	local_irq_save_hw(flags);
	/* Enable the PLL Wakeup bit in SIC IWR */
	iwr = bfin_read32(SIC_IWR);
	/* Only allow PPL Wakeup) */
	bfin_write32(SIC_IWR, IWR_ENABLE(0));

	bfin_write16(VR_CTL, val);
	SSYNC();
	asm("IDLE;");

	bfin_write32(SIC_IWR, iwr);
	local_irq_restore_hw(flags);
}

#endif				/* _CDEF_BF532_H */
