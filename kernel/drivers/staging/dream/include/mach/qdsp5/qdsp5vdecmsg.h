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

#ifndef QDSP5VIDDECMSGI_H
#define QDSP5VIDDECMSGI_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    V I D E O  D E C O D E R   I N T E R N A L  M E S S A G E S

GENERAL DESCRIPTION
  This file contains defintions of format blocks of messages
  that are sent by VIDDEC Task

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

$Header: //source/qcom/qct/multimedia2/AdspSvc/7XXX/qdsp5cmd/video/qdsp5vdecmsg.h#2 $ $DateTime: 2008/07/30 10:50:23 $ $Author: pavanr $
Revision History:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/10/08   ac      initial version
===========================================================================*/

/*
 * Message to inform ARM which VDEC_SUBFRAME_PKT_CMD processed by VIDDEC TASK
 */

#define	VIDDEC_MSG_SUBF_DONE	0x0000
#define	VIDDEC_MSG_SUBF_DONE_LEN	\
	sizeof(viddec_msg_subf_done)

typedef struct {
	unsigned short	packet_seq_number;
	unsigned short	codec_instance_id;
} __attribute__((packed)) viddec_msg_subf_done;


/*
 * Message to inform ARM one frame has been decoded
 */

#define	VIDDEC_MSG_FRAME_DONE	0x0001
#define	VIDDEC_MSG_FRAME_DONE_LEN	\
	sizeof(viddec_msg_frame_done)

typedef struct {
	unsigned short	packet_seq_number;
	unsigned short	codec_instance_id;
} __attribute__((packed)) viddec_msg_frame_done;


/*
 * Message to inform ARM that post processing frame has been decoded
 */

#define	VIDDEC_MSG_PP_ENABLE_CMD_DONE	0x0002
#define	VIDDEC_MSG_PP_ENABLE_CMD_DONE_LEN	\
	sizeof(viddec_msg_pp_enable_cmd_done)

typedef struct {
	unsigned short	packet_seq_number;
	unsigned short	codec_instance_id;
} __attribute__((packed)) viddec_msg_pp_enable_cmd_done;


/*
 * Message to inform ARM that one post processing frame has been decoded
 */


#define	VIDDEC_MSG_PP_FRAME_DONE		0x0003
#define	VIDDEC_MSG_PP_FRAME_DONE_LEN	\
	sizeof(viddec_msg_pp_frame_done)

#define	VIDDEC_MSG_DISP_WORTHY_DISP		0x0000
#define	VIDDEC_MSG_DISP_WORTHY_DISP_NONE	0xFFFF


typedef struct {
	unsigned short	packet_seq_number;
	unsigned short	codec_instance_id;
	unsigned short	display_worthy;
} __attribute__((packed)) viddec_msg_pp_frame_done;


#endif
