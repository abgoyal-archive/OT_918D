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
 * Copyright(c) 2008 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Maintained at www.Open-FCoE.org
 */

/*
 * Provide interface to send ELS/CT FC frames
 */

#include <asm/unaligned.h>
#include <scsi/fc/fc_gs.h>
#include <scsi/fc/fc_ns.h>
#include <scsi/fc/fc_els.h>
#include <scsi/libfc.h>
#include <scsi/fc_encode.h>

/**
 * fc_elsct_send() - Send an ELS or CT frame
 * @lport:	The local port to send the frame on
 * @did:	The destination ID for the frame
 * @fp:		The frame to be sent
 * @op:		The operational code
 * @resp:	The callback routine when the response is received
 * @arg:	The argument to pass to the response callback routine
 * @timer_msec: The timeout period for the frame (in msecs)
 */
struct fc_seq *fc_elsct_send(struct fc_lport *lport, u32 did,
			     struct fc_frame *fp, unsigned int op,
			     void (*resp)(struct fc_seq *,
					  struct fc_frame *,
					  void *),
			     void *arg, u32 timer_msec)
{
	enum fc_rctl r_ctl;
	enum fc_fh_type fh_type;
	int rc;

	/* ELS requests */
	if ((op >= ELS_LS_RJT) && (op <= ELS_AUTH_ELS))
		rc = fc_els_fill(lport, did, fp, op, &r_ctl, &fh_type);
	else {
		/* CT requests */
		rc = fc_ct_fill(lport, did, fp, op, &r_ctl, &fh_type);
		did = FC_FID_DIR_SERV;
	}

	if (rc) {
		fc_frame_free(fp);
		return NULL;
	}

	fc_fill_fc_hdr(fp, r_ctl, did, lport->port_id, fh_type,
		       FC_FC_FIRST_SEQ | FC_FC_END_SEQ | FC_FC_SEQ_INIT, 0);

	return lport->tt.exch_seq_send(lport, fp, resp, NULL, arg, timer_msec);
}
EXPORT_SYMBOL(fc_elsct_send);

/**
 * fc_elsct_init() - Initialize the ELS/CT layer
 * @lport: The local port to initialize the ELS/CT layer for
 */
int fc_elsct_init(struct fc_lport *lport)
{
	if (!lport->tt.elsct_send)
		lport->tt.elsct_send = fc_elsct_send;

	return 0;
}
EXPORT_SYMBOL(fc_elsct_init);

/**
 * fc_els_resp_type() - Return a string describing the ELS response
 * @fp: The frame pointer or possible error code
 */
const char *fc_els_resp_type(struct fc_frame *fp)
{
	const char *msg;
	struct fc_frame_header *fh;
	struct fc_ct_hdr *ct;

	if (IS_ERR(fp)) {
		switch (-PTR_ERR(fp)) {
		case FC_NO_ERR:
			msg = "response no error";
			break;
		case FC_EX_TIMEOUT:
			msg = "response timeout";
			break;
		case FC_EX_CLOSED:
			msg = "response closed";
			break;
		default:
			msg = "response unknown error";
			break;
		}
	} else {
		fh = fc_frame_header_get(fp);
		switch (fh->fh_type) {
		case FC_TYPE_ELS:
			switch (fc_frame_payload_op(fp)) {
			case ELS_LS_ACC:
				msg = "accept";
				break;
			case ELS_LS_RJT:
				msg = "reject";
				break;
			default:
				msg = "response unknown ELS";
				break;
			}
			break;
		case FC_TYPE_CT:
			ct = fc_frame_payload_get(fp, sizeof(*ct));
			if (ct) {
				switch (ntohs(ct->ct_cmd)) {
				case FC_FS_ACC:
					msg = "CT accept";
					break;
				case FC_FS_RJT:
					msg = "CT reject";
					break;
				default:
					msg = "response unknown CT";
					break;
				}
			} else {
				msg = "short CT response";
			}
			break;
		default:
			msg = "response not ELS or CT";
			break;
		}
	}
	return msg;
}
