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
 * Copyright (c) 2005-2009 Brocade Communications Systems, Inc.
 * All rights reserved
 * www.brocade.com
 *
 * Linux driver for Brocade Fibre Channel Host Bus Adapter.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (GPL) Version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */
#ifndef __BFA_DEFS_AUTH_H__
#define __BFA_DEFS_AUTH_H__

#include <defs/bfa_defs_types.h>

#define PUBLIC_KEY			15409
#define PRIVATE_KEY			19009
#define KEY_LEN				32399
#define BFA_AUTH_SECRET_STRING_LEN	256
#define BFA_AUTH_FAIL_NO_PASSWORD	0xFE
#define BFA_AUTH_FAIL_TIMEOUT		0xFF

/**
 * Authentication status
 */
enum bfa_auth_status {
	BFA_AUTH_STATUS_NONE 	= 0,	/*  no authentication */
	BFA_AUTH_UNINIT 	= 1,	/*  state - uninit */
	BFA_AUTH_NEG_SEND 	= 2,	/*  state - negotiate send */
	BFA_AUTH_CHAL_WAIT 	= 3,	/*  state - challenge wait */
	BFA_AUTH_NEG_RETRY 	= 4,	/*  state - negotiate retry */
	BFA_AUTH_REPLY_SEND 	= 5,	/*  state - reply send */
	BFA_AUTH_STATUS_WAIT 	= 6,	/*  state - status wait */
	BFA_AUTH_SUCCESS 	= 7,	/*  state - success */
	BFA_AUTH_FAILED 	= 8,	/*  state - failed */
	BFA_AUTH_STATUS_UNKNOWN = 9,	/*  authentication status unknown */
};

enum bfa_auth_rej_code {
	BFA_AUTH_RJT_CODE_AUTH_FAILURE   = 1, /* auth failure */
	BFA_AUTH_RJT_CODE_LOGICAL_ERR    = 2, /* logical error */
};

/**
 * Authentication reject codes
 */
enum bfa_auth_rej_code_exp {
	BFA_AUTH_MECH_NOT_USABLE        = 1, /* auth. mechanism not usable */
	BFA_AUTH_DH_GROUP_NOT_USABLE    = 2, /* DH Group not usable */
	BFA_AUTH_HASH_FUNC_NOT_USABLE   = 3, /* hash Function not usable */
	BFA_AUTH_AUTH_XACT_STARTED      = 4, /* auth xact started */
	BFA_AUTH_AUTH_FAILED            = 5, /* auth failed */
	BFA_AUTH_INCORRECT_PLD          = 6, /* incorrect payload */
	BFA_AUTH_INCORRECT_PROTO_MSG    = 7, /* incorrect proto msg */
	BFA_AUTH_RESTART_AUTH_PROTO     = 8, /* restart auth protocol */
	BFA_AUTH_AUTH_CONCAT_NOT_SUPP   = 9, /* auth concat not supported */
	BFA_AUTH_PROTO_VER_NOT_SUPP     = 10,/* proto version not supported */
};

struct auth_proto_stats_s {
	u32        auth_rjts;
	u32        auth_negs;
	u32        auth_dones;

	u32        dhchap_challenges;
	u32        dhchap_replies;
	u32        dhchap_successes;
};

/**
 * Authentication related statistics
 */
struct bfa_auth_stats_s {
	u32           auth_failures;	/*  authentication failures */
	u32           auth_successes;	/*  authentication successes*/
	struct auth_proto_stats_s auth_rx_stats; /*  Rx protocol stats */
	struct auth_proto_stats_s auth_tx_stats; /*  Tx protocol stats */
};

/**
 * Authentication hash function algorithms
 */
enum bfa_auth_algo {
	BFA_AUTH_ALGO_MD5 	= 1,	/*  Message-Digest algorithm 5 */
	BFA_AUTH_ALGO_SHA1 	= 2,	/*  Secure Hash Algorithm 1 */
	BFA_AUTH_ALGO_MS 	= 3,	/*  MD5, then SHA-1 */
	BFA_AUTH_ALGO_SM 	= 4,	/*  SHA-1, then MD5 */
};

/**
 * DH Groups
 *
 * Current value could be combination of one or more of the following values
 */
enum bfa_auth_group {
	BFA_AUTH_GROUP_DHNULL 	= 0,	/*  DH NULL (value == 0) */
	BFA_AUTH_GROUP_DH768 	= 1,	/*  DH group 768 (value == 1) */
	BFA_AUTH_GROUP_DH1024 	= 2,	/*  DH group 1024 (value == 2) */
	BFA_AUTH_GROUP_DH1280 	= 4,	/*  DH group 1280 (value == 3) */
	BFA_AUTH_GROUP_DH1536 	= 8,	/*  DH group 1536 (value == 4) */

	BFA_AUTH_GROUP_ALL 	= 256	/*  Use default DH group order
					 *    0, 1, 2, 3, 4 */
};

/**
 * Authentication secret sources
 */
enum bfa_auth_secretsource {
	BFA_AUTH_SECSRC_LOCAL 	= 1,	/*  locally configured */
	BFA_AUTH_SECSRC_RADIUS 	= 2,	/*  use radius server */
	BFA_AUTH_SECSRC_TACACS 	= 3,	/*  TACACS server */
};

/**
 * Authentication attributes
 */
struct bfa_auth_attr_s {
	enum bfa_auth_status 	status;
	enum bfa_auth_algo 	algo;
	enum bfa_auth_group 	dh_grp;
	u16		rjt_code;
	u16		rjt_code_exp;
	u8			secret_set;
	u8			resv[7];
};

#endif /* __BFA_DEFS_AUTH_H__ */
