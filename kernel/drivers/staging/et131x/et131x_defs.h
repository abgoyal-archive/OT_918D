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
 * Agere Systems Inc.
 * 10/100/1000 Base-T Ethernet Driver for the ET1301 and ET131x series MACs
 *
 * Copyright © 2005 Agere Systems Inc.
 * All rights reserved.
 *   http://www.agere.com
 *
 *------------------------------------------------------------------------------
 *
 * et131x_defs.h - Defines, structs, enums, prototypes, etc. to assist with OS
 *                 compatibility
 *
 *------------------------------------------------------------------------------
 *
 * SOFTWARE LICENSE
 *
 * This software is provided subject to the following terms and conditions,
 * which you should read carefully before using the software.  Using this
 * software indicates your acceptance of these terms and conditions.  If you do
 * not agree with these terms and conditions, do not use the software.
 *
 * Copyright © 2005 Agere Systems Inc.
 * All rights reserved.
 *
 * Redistribution and use in source or binary forms, with or without
 * modifications, are permitted provided that the following conditions are met:
 *
 * . Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following Disclaimer as comments in the code as
 *    well as in the documentation and/or other materials provided with the
 *    distribution.
 *
 * . Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following Disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * . Neither the name of Agere Systems Inc. nor the names of the contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Disclaimer
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, INFRINGEMENT AND THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  ANY
 * USE, MODIFICATION OR DISTRIBUTION OF THIS SOFTWARE IS SOLELY AT THE USERS OWN
 * RISK. IN NO EVENT SHALL AGERE SYSTEMS INC. OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, INCLUDING, BUT NOT LIMITED TO, CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */

#ifndef __ET131X_DEFS_H__
#define __ET131X_DEFS_H__

/* Packet and header sizes */
#define NIC_MIN_PACKET_SIZE	60

/* Multicast list size */
#define NIC_MAX_MCAST_LIST	128

/* Supported Filters */
#define ET131X_PACKET_TYPE_DIRECTED		0x0001
#define ET131X_PACKET_TYPE_MULTICAST		0x0002
#define ET131X_PACKET_TYPE_BROADCAST		0x0004
#define ET131X_PACKET_TYPE_PROMISCUOUS		0x0008
#define ET131X_PACKET_TYPE_ALL_MULTICAST	0x0010

/* Tx Timeout */
#define ET131X_TX_TIMEOUT	(1 * HZ)
#define NIC_SEND_HANG_THRESHOLD	0

/* MP_TCB flags */
#define fMP_DEST_MULTI			0x00000001
#define fMP_DEST_BROAD			0x00000002

/* MP_ADAPTER flags */
#define fMP_ADAPTER_RECV_LOOKASIDE	0x00000004
#define fMP_ADAPTER_INTERRUPT_IN_USE	0x00000008
#define fMP_ADAPTER_SECONDARY		0x00000010

/* MP_SHARED flags */
#define fMP_ADAPTER_SHUTDOWN		0x00100000
#define fMP_ADAPTER_LOWER_POWER		0x00200000

#define fMP_ADAPTER_NON_RECOVER_ERROR	0x00800000
#define fMP_ADAPTER_RESET_IN_PROGRESS	0x01000000
#define fMP_ADAPTER_NO_CABLE		0x02000000
#define fMP_ADAPTER_HARDWARE_ERROR	0x04000000
#define fMP_ADAPTER_REMOVE_IN_PROGRESS	0x08000000
#define fMP_ADAPTER_HALT_IN_PROGRESS	0x10000000
#define fMP_ADAPTER_LINK_DETECTION	0x20000000

#define fMP_ADAPTER_FAIL_SEND_MASK	0x3ff00000
#define fMP_ADAPTER_NOT_READY_MASK	0x3ff00000

/* Some offsets in PCI config space that are actually used. */
#define ET1310_PCI_MAX_PYLD		0x4C
#define ET1310_PCI_MAC_ADDRESS		0xA4
#define ET1310_PCI_EEPROM_STATUS	0xB2
#define ET1310_PCI_ACK_NACK		0xC0
#define ET1310_PCI_REPLAY		0xC2
#define ET1310_PCI_L0L1LATENCY		0xCF

/* PCI Vendor/Product IDs */
#define ET131X_PCI_VENDOR_ID		0x11C1	/* Agere Systems */
#define ET131X_PCI_DEVICE_ID_GIG	0xED00	/* ET1310 1000 Base-T 8 */
#define ET131X_PCI_DEVICE_ID_FAST	0xED01	/* ET1310 100  Base-T */

/* Define order of magnitude converter */
#define NANO_IN_A_MICRO	1000

#define PARM_RX_NUM_BUFS_DEF    4
#define PARM_RX_TIME_INT_DEF    10
#define PARM_RX_MEM_END_DEF     0x2bc
#define PARM_TX_TIME_INT_DEF    40
#define PARM_TX_NUM_BUFS_DEF    4
#define PARM_DMA_CACHE_DEF      0


#endif /* __ET131X_DEFS_H__ */
