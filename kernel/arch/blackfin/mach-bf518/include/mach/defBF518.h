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

#ifndef _DEF_BF518_H
#define _DEF_BF518_H

/* BF518 is BF516 + IEEE-1588 */
#include "defBF516.h"

/* PTP TSYNC Registers */

#define EMAC_PTP_CTL                   0xFFC030A0 /* PTP Block Control */
#define EMAC_PTP_IE                    0xFFC030A4 /* PTP Block Interrupt Enable */
#define EMAC_PTP_ISTAT                 0xFFC030A8 /* PTP Block Interrupt Status */
#define EMAC_PTP_FOFF                  0xFFC030AC /* PTP Filter offset Register */
#define EMAC_PTP_FV1                   0xFFC030B0 /* PTP Filter Value Register 1 */
#define EMAC_PTP_FV2                   0xFFC030B4 /* PTP Filter Value Register 2 */
#define EMAC_PTP_FV3                   0xFFC030B8 /* PTP Filter Value Register 3 */
#define EMAC_PTP_ADDEND                0xFFC030BC /* PTP Addend for Frequency Compensation */
#define EMAC_PTP_ACCR                  0xFFC030C0 /* PTP Accumulator for Frequency Compensation */
#define EMAC_PTP_OFFSET                0xFFC030C4 /* PTP Time Offset Register */
#define EMAC_PTP_TIMELO                0xFFC030C8 /* PTP Precision Clock Time Low */
#define EMAC_PTP_TIMEHI                0xFFC030CC /* PTP Precision Clock Time High */
#define EMAC_PTP_RXSNAPLO              0xFFC030D0 /* PTP Receive Snapshot Register Low */
#define EMAC_PTP_RXSNAPHI              0xFFC030D4 /* PTP Receive Snapshot Register High */
#define EMAC_PTP_TXSNAPLO              0xFFC030D8 /* PTP Transmit Snapshot Register Low */
#define EMAC_PTP_TXSNAPHI              0xFFC030DC /* PTP Transmit Snapshot Register High */
#define EMAC_PTP_ALARMLO               0xFFC030E0 /* PTP Alarm time Low */
#define EMAC_PTP_ALARMHI               0xFFC030E4 /* PTP Alarm time High */
#define EMAC_PTP_ID_OFF                0xFFC030E8 /* PTP Capture ID offset register */
#define EMAC_PTP_ID_SNAP               0xFFC030EC /* PTP Capture ID register */
#define EMAC_PTP_PPS_STARTLO           0xFFC030F0 /* PPS Start Time Low */
#define EMAC_PTP_PPS_STARTHI           0xFFC030F4 /* PPS Start Time High */
#define EMAC_PTP_PPS_PERIOD            0xFFC030F8 /* PPS Count Register */

/* Bit masks for EMAC_PTP_CTL */

#define                    PTP_EN  0x1        /* Enable the PTP_TSYNC module */
#define                        TL  0x2        /* Timestamp lock control */
#define                      ASEN  0x10       /* Auxiliary snapshot control */
#define                     PPSEN  0x80       /* Pulse-per-second (PPS) control */
#define                     CKOEN  0x2000     /* Clock output control */

/* Bit masks for EMAC_PTP_IE */

#define                      ALIE  0x1        /* Alarm interrupt enable */
#define                     RXEIE  0x2        /* Receive event interrupt enable */
#define                     RXGIE  0x4        /* Receive general interrupt enable */
#define                      TXIE  0x8        /* Transmit interrupt enable */
#define                     RXOVE  0x10       /* Receive overrun error interrupt enable */
#define                     TXOVE  0x20       /* Transmit overrun error interrupt enable */
#define                      ASIE  0x40       /* Auxiliary snapshot interrupt enable */

/* Bit masks for EMAC_PTP_ISTAT */

#define                       ALS  0x1        /* Alarm status */
#define                      RXEL  0x2        /* Receive event interrupt status */
#define                      RXGL  0x4        /* Receive general interrupt status */
#define                      TXTL  0x8        /* Transmit snapshot status */
#define                      RXOV  0x10       /* Receive snapshot overrun status */
#define                      TXOV  0x20       /* Transmit snapshot overrun status */
#define                       ASL  0x40       /* Auxiliary snapshot interrupt status */

#endif /* _DEF_BF518_H */
