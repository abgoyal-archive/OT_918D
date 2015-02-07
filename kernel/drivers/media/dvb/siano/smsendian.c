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

/****************************************************************

 Siano Mobile Silicon, Inc.
 MDTV receiver kernel modules.
 Copyright (C) 2006-2009, Uri Shkolnik

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ****************************************************************/

#include <asm/byteorder.h>

#include "smsendian.h"
#include "smscoreapi.h"

void smsendian_handle_tx_message(void *buffer)
{
#ifdef __BIG_ENDIAN
	struct SmsMsgData_ST *msg = (struct SmsMsgData_ST *)buffer;
	int i;
	int msgWords;

	switch (msg->xMsgHeader.msgType) {
	case MSG_SMS_DATA_DOWNLOAD_REQ:
	{
		msg->msgData[0] = le32_to_cpu(msg->msgData[0]);
		break;
	}

	default:
		msgWords = (msg->xMsgHeader.msgLength -
				sizeof(struct SmsMsgHdr_ST))/4;

		for (i = 0; i < msgWords; i++)
			msg->msgData[i] = le32_to_cpu(msg->msgData[i]);

		break;
	}
#endif /* __BIG_ENDIAN */
}
EXPORT_SYMBOL_GPL(smsendian_handle_tx_message);

void smsendian_handle_rx_message(void *buffer)
{
#ifdef __BIG_ENDIAN
	struct SmsMsgData_ST *msg = (struct SmsMsgData_ST *)buffer;
	int i;
	int msgWords;

	switch (msg->xMsgHeader.msgType) {
	case MSG_SMS_GET_VERSION_EX_RES:
	{
		struct SmsVersionRes_ST *ver =
			(struct SmsVersionRes_ST *) msg;
		ver->ChipModel = le16_to_cpu(ver->ChipModel);
		break;
	}

	case MSG_SMS_DVBT_BDA_DATA:
	case MSG_SMS_DAB_CHANNEL:
	case MSG_SMS_DATA_MSG:
	{
		break;
	}

	default:
	{
		msgWords = (msg->xMsgHeader.msgLength -
				sizeof(struct SmsMsgHdr_ST))/4;

		for (i = 0; i < msgWords; i++)
			msg->msgData[i] = le32_to_cpu(msg->msgData[i]);

		break;
	}
	}
#endif /* __BIG_ENDIAN */
}
EXPORT_SYMBOL_GPL(smsendian_handle_rx_message);

void smsendian_handle_message_header(void *msg)
{
#ifdef __BIG_ENDIAN
	struct SmsMsgHdr_ST *phdr = (struct SmsMsgHdr_ST *)msg;

	phdr->msgType = le16_to_cpu(phdr->msgType);
	phdr->msgLength = le16_to_cpu(phdr->msgLength);
	phdr->msgFlags = le16_to_cpu(phdr->msgFlags);
#endif /* __BIG_ENDIAN */
}
EXPORT_SYMBOL_GPL(smsendian_handle_message_header);
