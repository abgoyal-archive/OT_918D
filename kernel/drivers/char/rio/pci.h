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
** -----------------------------------------------------------------------------
**
**  Perle Specialix driver for Linux
**  Ported from existing RIO Driver for SCO sources.
 *
 *  (C) 1990 - 2000 Specialix International Ltd., Byfleet, Surrey, UK.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
**	Module		: pci.h
**	SID		: 1.2
**	Last Modified	: 11/6/98 11:34:12
**	Retrieved	: 11/6/98 11:34:21
**
**  ident @(#)pci.h	1.2
**
** -----------------------------------------------------------------------------
*/

#ifndef __rio_pci_h__
#define	__rio_pci_h__

/*
** PCI stuff
*/

#define	PCITpFastClock		0x80
#define	PCITpSlowClock		0x00
#define	PCITpFastLinks	        0x40
#define	PCITpSlowLinks	        0x00
#define	PCITpIntEnable		0x04
#define	PCITpIntDisable		0x00
#define	PCITpBusEnable		0x02
#define	PCITpBusDisable		0x00
#define	PCITpBootFromRam	0x01
#define	PCITpBootFromLink	0x00

#define	RIO_PCI_VENDOR		0x11CB
#define	RIO_PCI_DEVICE		0x8000
#define	RIO_PCI_BASE_CLASS	0x02
#define	RIO_PCI_SUB_CLASS	0x80
#define	RIO_PCI_PROG_IFACE	0x00

#define RIO_PCI_RID		0x0008
#define RIO_PCI_BADR0		0x0010
#define RIO_PCI_INTLN		0x003C
#define RIO_PCI_INTPIN		0x003D

#define	RIO_PCI_MEM_SIZE	65536

#define	RIO_PCI_TURBO_TP	0x80
#define	RIO_PCI_FAST_LINKS	0x40
#define	RIO_PCI_INT_ENABLE	0x04
#define	RIO_PCI_TP_BUS_ENABLE	0x02
#define	RIO_PCI_BOOT_FROM_RAM	0x01

#define	RIO_PCI_DEFAULT_MODE	0x05

#endif				/* __rio_pci_h__ */
