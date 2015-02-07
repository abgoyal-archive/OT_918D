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
 * iwmc3200top - Intel Wireless MultiCom 3200 Top Driver
 * drivers/misc/iwmc3200top/fw-msg.h
 *
 * Copyright (C) 2009 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 *
 * Author Name: Maxim Grabarnik <maxim.grabarnink@intel.com>
 *  -
 *
 */

#ifndef __FWMSG_H__
#define __FWMSG_H__

#define COMM_TYPE_D2H           	0xFF
#define COMM_TYPE_H2D           	0xEE

#define COMM_CATEGORY_OPERATIONAL      	0x00
#define COMM_CATEGORY_DEBUG            	0x01
#define COMM_CATEGORY_TESTABILITY      	0x02
#define COMM_CATEGORY_DIAGNOSTICS      	0x03

#define OP_DBG_ZSTR_MSG			cpu_to_le16(0x1A)

#define FW_LOG_SRC_MAX			32
#define FW_LOG_SRC_ALL			255

#define FW_STRING_TABLE_ADDR		cpu_to_le32(0x0C000000)

#define CMD_DBG_LOG_LEVEL		cpu_to_le16(0x0001)
#define CMD_TST_DEV_RESET		cpu_to_le16(0x0060)
#define CMD_TST_FUNC_RESET		cpu_to_le16(0x0062)
#define CMD_TST_IFACE_RESET		cpu_to_le16(0x0064)
#define CMD_TST_CPU_UTILIZATION		cpu_to_le16(0x0065)
#define CMD_TST_TOP_DEEP_SLEEP		cpu_to_le16(0x0080)
#define CMD_TST_WAKEUP			cpu_to_le16(0x0081)
#define CMD_TST_FUNC_WAKEUP		cpu_to_le16(0x0082)
#define CMD_TST_FUNC_DEEP_SLEEP_REQUEST	cpu_to_le16(0x0083)
#define CMD_TST_GET_MEM_DUMP		cpu_to_le16(0x0096)

#define OP_OPR_ALIVE			cpu_to_le16(0x0010)
#define OP_OPR_CMD_ACK			cpu_to_le16(0x001F)
#define OP_OPR_CMD_NACK			cpu_to_le16(0x0020)
#define OP_TST_MEM_DUMP			cpu_to_le16(0x0043)

#define CMD_FLAG_PADDING_256		0x80

#define FW_HCMD_BLOCK_SIZE      	256

struct msg_hdr {
	u8 type;
	u8 category;
	__le16 opcode;
	u8 seqnum;
	u8 flags;
	__le16 length;
} __attribute__((__packed__));

struct log_hdr {
	__le32 timestamp;
	u8 severity;
	u8 logsource;
	__le16 reserved;
} __attribute__((__packed__));

struct mdump_hdr {
	u8 dmpid;
	u8 frag;
	__le16 size;
	__le32 addr;
} __attribute__((__packed__));

struct top_msg {
	struct msg_hdr hdr;
	union {
		/* D2H messages */
		struct {
			struct log_hdr log_hdr;
			u8 data[1];
		} __attribute__((__packed__)) log;

		struct {
			struct log_hdr log_hdr;
			struct mdump_hdr md_hdr;
			u8 data[1];
		} __attribute__((__packed__)) mdump;

		/* H2D messages */
		struct {
			u8 logsource;
			u8 sevmask;
		} __attribute__((__packed__)) logdefs[FW_LOG_SRC_MAX];
		struct mdump_hdr mdump_req;
	} u;
} __attribute__((__packed__));


#endif /* __FWMSG_H__ */
