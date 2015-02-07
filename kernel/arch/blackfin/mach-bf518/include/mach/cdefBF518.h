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
 * Copyright 2008-2009 Analog Devices Inc.
 *
 * Licensed under the ADI BSD license or the GPL-2 (or later)
 */

#ifndef _CDEF_BF518_H
#define _CDEF_BF518_H

/* include all Core registers and bit definitions */
#include "defBF518.h"

/* BF518 is BF516 + IEEE-1588 */
#include "cdefBF516.h"

/* PTP TSYNC Registers */

#define bfin_read_EMAC_PTP_CTL()                bfin_read16(EMAC_PTP_CTL)
#define bfin_write_EMAC_PTP_CTL(val)            bfin_write16(EMAC_PTP_CTL, val)
#define bfin_read_EMAC_PTP_IE()                 bfin_read16(EMAC_PTP_IE)
#define bfin_write_EMAC_PTP_IE(val)             bfin_write16(EMAC_PTP_IE, val)
#define bfin_read_EMAC_PTP_ISTAT()              bfin_read16(EMAC_PTP_ISTAT)
#define bfin_write_EMAC_PTP_ISTAT(val)          bfin_write16(EMAC_PTP_ISTAT, val)
#define bfin_read_EMAC_PTP_FOFF()               bfin_read32(EMAC_PTP_FOFF)
#define bfin_write_EMAC_PTP_FOFF(val)           bfin_write32(EMAC_PTP_FOFF, val)
#define bfin_read_EMAC_PTP_FV1()                bfin_read32(EMAC_PTP_FV1)
#define bfin_write_EMAC_PTP_FV1(val)            bfin_write32(EMAC_PTP_FV1, val)
#define bfin_read_EMAC_PTP_FV2()                bfin_read32(EMAC_PTP_FV2)
#define bfin_write_EMAC_PTP_FV2(val)            bfin_write32(EMAC_PTP_FV2, val)
#define bfin_read_EMAC_PTP_FV3()                bfin_read32(EMAC_PTP_FV3)
#define bfin_write_EMAC_PTP_FV3(val)            bfin_write32(EMAC_PTP_FV3, val)
#define bfin_read_EMAC_PTP_ADDEND()             bfin_read32(EMAC_PTP_ADDEND)
#define bfin_write_EMAC_PTP_ADDEND(val)         bfin_write32(EMAC_PTP_ADDEND, val)
#define bfin_read_EMAC_PTP_ACCR()               bfin_read32(EMAC_PTP_ACCR)
#define bfin_write_EMAC_PTP_ACCR(val)           bfin_write32(EMAC_PTP_ACCR, val)
#define bfin_read_EMAC_PTP_OFFSET()             bfin_read32(EMAC_PTP_OFFSET)
#define bfin_write_EMAC_PTP_OFFSET(val)         bfin_write32(EMAC_PTP_OFFSET, val)
#define bfin_read_EMAC_PTP_TIMELO()             bfin_read32(EMAC_PTP_TIMELO)
#define bfin_write_EMAC_PTP_TIMELO(val)         bfin_write32(EMAC_PTP_TIMELO, val)
#define bfin_read_EMAC_PTP_TIMEHI()             bfin_read32(EMAC_PTP_TIMEHI)
#define bfin_write_EMAC_PTP_TIMEHI(val)         bfin_write32(EMAC_PTP_TIMEHI, val)
#define bfin_read_EMAC_PTP_RXSNAPLO()           bfin_read32(EMAC_PTP_RXSNAPLO)
#define bfin_read_EMAC_PTP_RXSNAPHI()           bfin_read32(EMAC_PTP_RXSNAPHI)
#define bfin_read_EMAC_PTP_TXSNAPLO()           bfin_read32(EMAC_PTP_TXSNAPLO)
#define bfin_read_EMAC_PTP_TXSNAPHI()           bfin_read32(EMAC_PTP_TXSNAPHI)
#define bfin_read_EMAC_PTP_ALARMLO()            bfin_read32(EMAC_PTP_ALARMLO)
#define bfin_write_EMAC_PTP_ALARMLO(val)        bfin_write32(EMAC_PTP_ALARMLO, val)
#define bfin_read_EMAC_PTP_ALARMHI()            bfin_read32(EMAC_PTP_ALARMHI)
#define bfin_write_EMAC_PTP_ALARMHI(val)        bfin_write32(EMAC_PTP_ALARMHI, val)
#define bfin_read_EMAC_PTP_ID_OFF()             bfin_read16(EMAC_PTP_ID_OFF)
#define bfin_write_EMAC_PTP_ID_OFF(val)         bfin_write16(EMAC_PTP_ID_OFF, val)
#define bfin_read_EMAC_PTP_ID_SNAP()            bfin_read32(EMAC_PTP_ID_SNAP)
#define bfin_write_EMAC_PTP_ID_SNAP(val)        bfin_write32(EMAC_PTP_ID_SNAP, val)
#define bfin_read_EMAC_PTP_PPS_STARTHI()        bfin_read32(EMAC_PTP_PPS_STARTHI)
#define bfin_write_EMAC_PTP_PPS_STARTHI(val)    bfin_write32(EMAC_PTP_PPS_STARTHI, val)
#define bfin_read_EMAC_PTP_PPS_PERIOD()         bfin_read32(EMAC_PTP_PPS_PERIOD)
#define bfin_write_EMAC_PTP_PPS_PERIOD(val)     bfin_write32(EMAC_PTP_PPS_PERIOD, val)

#endif /* _CDEF_BF518_H */
