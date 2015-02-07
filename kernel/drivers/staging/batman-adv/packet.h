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
 * Copyright (C) 2007-2010 B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 *
 */

#define ETH_P_BATMAN  0x4305	/* unofficial/not registered Ethertype */

#define BAT_PACKET    0x01
#define BAT_ICMP      0x02
#define BAT_UNICAST   0x03
#define BAT_BCAST     0x04
#define BAT_VIS       0x05

/* this file is included by batctl which needs these defines */
#define COMPAT_VERSION 8
#define DIRECTLINK 0x40
#define VIS_SERVER 0x20

/* ICMP message types */
#define ECHO_REPLY 0
#define DESTINATION_UNREACHABLE 3
#define ECHO_REQUEST 8
#define TTL_EXCEEDED 11
#define PARAMETER_PROBLEM 12

/* vis defines */
#define VIS_TYPE_SERVER_SYNC		0
#define VIS_TYPE_CLIENT_UPDATE		1

struct batman_packet {
	uint8_t  packet_type;
	uint8_t  version;  /* batman version field */
	uint8_t  flags;    /* 0x40: DIRECTLINK flag, 0x20 VIS_SERVER flag... */
	uint8_t  tq;
	uint16_t seqno;
	uint8_t  orig[6];
	uint8_t  prev_sender[6];
	uint8_t  ttl;
	uint8_t  num_hna;
} __attribute__((packed));

#define BAT_PACKET_LEN sizeof(struct batman_packet)

struct icmp_packet {
	uint8_t  packet_type;
	uint8_t  version;  /* batman version field */
	uint8_t  msg_type; /* see ICMP message types above */
	uint8_t  ttl;
	uint8_t  dst[6];
	uint8_t  orig[6];
	uint16_t seqno;
	uint8_t  uid;
} __attribute__((packed));

struct unicast_packet {
	uint8_t  packet_type;
	uint8_t  version;  /* batman version field */
	uint8_t  dest[6];
	uint8_t  ttl;
} __attribute__((packed));

struct bcast_packet {
	uint8_t  packet_type;
	uint8_t  version;  /* batman version field */
	uint8_t  orig[6];
	uint16_t seqno;
} __attribute__((packed));

struct vis_packet {
	uint8_t  packet_type;
	uint8_t  version;        /* batman version field */
	uint8_t  vis_type;	 /* which type of vis-participant sent this? */
	uint8_t  seqno;		 /* sequence number */
	uint8_t  entries;	 /* number of entries behind this struct */
	uint8_t  ttl;		 /* TTL */
	uint8_t  vis_orig[6];	 /* originator that informs about its
				  * neighbors */
	uint8_t  target_orig[6]; /* who should receive this packet */
	uint8_t  sender_orig[6]; /* who sent or rebroadcasted this packet */
} __attribute__((packed));
