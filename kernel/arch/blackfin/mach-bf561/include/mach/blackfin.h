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
 * Copyright 2005-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _MACH_BLACKFIN_H_
#define _MACH_BLACKFIN_H_

#define BF561_FAMILY

#include "bf561.h"
#include "defBF561.h"
#include "anomaly.h"

#if !defined(__ASSEMBLY__)
#include "cdefBF561.h"
#endif

#define bfin_read_FIO_FLAG_D() bfin_read_FIO0_FLAG_D()
#define bfin_write_FIO_FLAG_D(val) bfin_write_FIO0_FLAG_D(val)
#define bfin_read_FIO_DIR() bfin_read_FIO0_DIR()
#define bfin_write_FIO_DIR(val) bfin_write_FIO0_DIR(val)
#define bfin_read_FIO_INEN() bfin_read_FIO0_INEN()
#define bfin_write_FIO_INEN(val) bfin_write_FIO0_INEN(val)

#define SIC_IWR0 SICA_IWR0
#define SIC_IWR1 SICA_IWR1
#define SIC_IAR0 SICA_IAR0
#define bfin_write_SIC_IMASK0 bfin_write_SICA_IMASK0
#define bfin_write_SIC_IMASK1 bfin_write_SICA_IMASK1
#define bfin_write_SIC_IWR0   bfin_write_SICA_IWR0
#define bfin_write_SIC_IWR1   bfin_write_SICA_IWR1

#define bfin_read_SIC_IMASK0 bfin_read_SICA_IMASK0
#define bfin_read_SIC_IMASK1 bfin_read_SICA_IMASK1
#define bfin_read_SIC_IWR0   bfin_read_SICA_IWR0
#define bfin_read_SIC_IWR1   bfin_read_SICA_IWR1
#define bfin_read_SIC_ISR0   bfin_read_SICA_ISR0
#define bfin_read_SIC_ISR1   bfin_read_SICA_ISR1

#define bfin_read_SIC_IMASK(x)		bfin_read32(SICA_IMASK0 + (x << 2))
#define bfin_write_SIC_IMASK(x, val)	bfin_write32((SICA_IMASK0 + (x << 2)), val)
#define bfin_read_SICB_IMASK(x)		bfin_read32(SICB_IMASK0 + (x << 2))
#define bfin_write_SICB_IMASK(x, val)	bfin_write32((SICB_IMASK0 + (x << 2)), val)
#define bfin_read_SIC_ISR(x)		bfin_read32(SICA_ISR0 + (x << 2))
#define bfin_write_SIC_ISR(x, val)	bfin_write32((SICA_ISR0 + (x << 2)), val)
#define bfin_read_SICB_ISR(x)		bfin_read32(SICB_ISR0 + (x << 2))
#define bfin_write_SICB_ISR(x, val)	bfin_write32((SICB_ISR0 + (x << 2)), val)

#define BFIN_UART_NR_PORTS      1

#define OFFSET_THR              0x00	/* Transmit Holding register            */
#define OFFSET_RBR              0x00	/* Receive Buffer register              */
#define OFFSET_DLL              0x00	/* Divisor Latch (Low-Byte)             */
#define OFFSET_IER              0x04	/* Interrupt Enable Register            */
#define OFFSET_DLH              0x04	/* Divisor Latch (High-Byte)            */
#define OFFSET_IIR              0x08	/* Interrupt Identification Register    */
#define OFFSET_LCR              0x0C	/* Line Control Register                */
#define OFFSET_MCR              0x10	/* Modem Control Register               */
#define OFFSET_LSR              0x14	/* Line Status Register                 */
#define OFFSET_MSR              0x18	/* Modem Status Register                */
#define OFFSET_SCR              0x1C	/* SCR Scratch Register                 */
#define OFFSET_GCTL             0x24	/* Global Control Register              */

#endif				/* _MACH_BLACKFIN_H_ */
