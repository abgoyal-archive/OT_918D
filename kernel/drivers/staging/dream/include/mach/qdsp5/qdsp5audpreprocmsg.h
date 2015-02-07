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

#ifndef QDSP5AUDPREPROCMSG_H
#define QDSP5AUDPREPROCMSG_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    A U D I O   P R E   P R O C E S S I N G  M E S S A G E S

GENERAL DESCRIPTION
  This file contains defintions of format blocks of messages
  that are rcvd by AUDPREPROC Task

REFERENCES
  None

EXTERNALIZED FUNCTIONS
  None

Copyright(c) 1992 - 2008 by QUALCOMM, Incorporated.

This software is licensed under the terms of the GNU General Public
License version 2, as published by the Free Software Foundation, and
may be copied, distributed, and modified under those terms.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

 $Header: //source/qcom/qct/multimedia2/Audio/drivers/QDSP5Driver/QDSP5Interface/main/latest/qdsp5audpreprocmsg.h#3 $

===========================================================================*/

/*
 * ADSPREPROCTASK Messages
 * AUDPREPROCTASK uses audPreProcUpRlist to communicate with ARM
 * Location	: MEMA
 * Message Length  : 2
 */

/*
 * Message to indicate particular feature has been enabled or disabled
 */


#define	AUDPREPROC_MSG_CMD_CFG_DONE_MSG	0x0000
#define	AUDPREPROC_MSG_CMD_CFG_DONE_MSG_LEN	\
	sizeof(audpreproc_msg_cmd_cfg_done_msg)

#define	AUDPREPROC_MSG_TYPE_AGC			0x0000
#define	AUDPREPROC_MSG_TYPE_NOISE_REDUCTION	0x0001
#define	AUDPREPROC_MSG_TYPE_IIR_FILTER		0x0002


#define	AUDPREPROC_MSG_STATUS_FLAG_ENA		-1
#define	AUDPREPROC_MSG_STATUS_FLAG_DIS		0x0000

typedef struct {
	unsigned short	type;
	signed short	status_flag;
} __attribute__((packed)) audpreproc_msg_cmd_cfg_done_msg;


/*
 * Message to indicate particular feature has selected for wrong samp freq
 */

#define	AUDPREPROC_MSG_ERROR_MSG_ID		0x0001
#define	AUDPREPROC_MSG_ERROR_MSG_ID_LEN	\
	sizeof(audpreproc_msg_error_msg_id)

#define	AUDPREPROC_MSG_ERR_INDEX_NS		0x0000

typedef struct {
	 unsigned short	err_index;
} __attribute__((packed)) audpreproc_msg_error_msg_id;

#endif
