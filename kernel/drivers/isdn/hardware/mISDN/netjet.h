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
 * NETjet common header file
 *
 * Author	Karsten Keil
 *              based on work of Matt Henderson and Daniel Potts,
 *              Traverse Technologies P/L www.traverse.com.au
 *
 * Copyright 2009  by Karsten Keil <keil@isdn4linux.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#define NJ_CTRL			0x00
#define NJ_DMACTRL		0x01
#define NJ_AUXCTRL		0x02
#define NJ_AUXDATA		0x03
#define NJ_IRQMASK0		0x04
#define NJ_IRQMASK1		0x05
#define NJ_IRQSTAT0		0x06
#define NJ_IRQSTAT1		0x07
#define NJ_DMA_READ_START	0x08
#define NJ_DMA_READ_IRQ		0x0c
#define NJ_DMA_READ_END		0x10
#define NJ_DMA_READ_ADR		0x14
#define NJ_DMA_WRITE_START	0x18
#define NJ_DMA_WRITE_IRQ	0x1c
#define NJ_DMA_WRITE_END	0x20
#define NJ_DMA_WRITE_ADR	0x24
#define NJ_PULSE_CNT		0x28

#define NJ_ISAC_OFF		0xc0
#define NJ_ISACIRQ		0x10

#define NJ_IRQM0_RD_MASK	0x03
#define NJ_IRQM0_RD_IRQ		0x01
#define NJ_IRQM0_RD_END		0x02
#define NJ_IRQM0_WR_MASK	0x0c
#define NJ_IRQM0_WR_IRQ		0x04
#define NJ_IRQM0_WR_END		0x08

/* one page here is no need to be smaller */
#define NJ_DMA_SIZE		4096
/* 2 * 64 byte is a compromise between IRQ count and latency */
#define NJ_DMA_RXSIZE		128  /* 2 * 64 */
#define NJ_DMA_TXSIZE		128  /* 2 * 64 */

