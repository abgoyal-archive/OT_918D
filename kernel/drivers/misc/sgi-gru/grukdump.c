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
 * SN Platform GRU Driver
 *
 *            Dump GRU State
 *
 *  Copyright (c) 2008 Silicon Graphics, Inc.  All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/bitops.h>
#include <asm/uv/uv_hub.h>
#include "gru.h"
#include "grutables.h"
#include "gruhandles.h"
#include "grulib.h"

#define CCH_LOCK_ATTEMPTS	10

static int gru_user_copy_handle(void __user **dp, void *s)
{
	if (copy_to_user(*dp, s, GRU_HANDLE_BYTES))
		return -1;
	*dp += GRU_HANDLE_BYTES;
	return 0;
}

static int gru_dump_context_data(void *grubase,
			struct gru_context_configuration_handle *cch,
			void __user *ubuf, int ctxnum, int dsrcnt,
			int flush_cbrs)
{
	void *cb, *cbe, *tfh, *gseg;
	int i, scr;

	gseg = grubase + ctxnum * GRU_GSEG_STRIDE;
	cb = gseg + GRU_CB_BASE;
	cbe = grubase + GRU_CBE_BASE;
	tfh = grubase + GRU_TFH_BASE;

	for_each_cbr_in_allocation_map(i, &cch->cbr_allocation_map, scr) {
		if (flush_cbrs)
			gru_flush_cache(cb);
		if (gru_user_copy_handle(&ubuf, cb))
			goto fail;
		if (gru_user_copy_handle(&ubuf, tfh + i * GRU_HANDLE_STRIDE))
			goto fail;
		if (gru_user_copy_handle(&ubuf, cbe + i * GRU_HANDLE_STRIDE))
			goto fail;
		cb += GRU_HANDLE_STRIDE;
	}
	if (dsrcnt)
		memcpy(ubuf, gseg + GRU_DS_BASE, dsrcnt * GRU_HANDLE_STRIDE);
	return 0;

fail:
	return -EFAULT;
}

static int gru_dump_tfm(struct gru_state *gru,
		void __user *ubuf, void __user *ubufend)
{
	struct gru_tlb_fault_map *tfm;
	int i, ret, bytes;

	bytes = GRU_NUM_TFM * GRU_CACHE_LINE_BYTES;
	if (bytes > ubufend - ubuf)
		ret = -EFBIG;

	for (i = 0; i < GRU_NUM_TFM; i++) {
		tfm = get_tfm(gru->gs_gru_base_vaddr, i);
		if (gru_user_copy_handle(&ubuf, tfm))
			goto fail;
	}
	return GRU_NUM_TFM * GRU_CACHE_LINE_BYTES;

fail:
	return -EFAULT;
}

static int gru_dump_tgh(struct gru_state *gru,
		void __user *ubuf, void __user *ubufend)
{
	struct gru_tlb_global_handle *tgh;
	int i, ret, bytes;

	bytes = GRU_NUM_TGH * GRU_CACHE_LINE_BYTES;
	if (bytes > ubufend - ubuf)
		ret = -EFBIG;

	for (i = 0; i < GRU_NUM_TGH; i++) {
		tgh = get_tgh(gru->gs_gru_base_vaddr, i);
		if (gru_user_copy_handle(&ubuf, tgh))
			goto fail;
	}
	return GRU_NUM_TGH * GRU_CACHE_LINE_BYTES;

fail:
	return -EFAULT;
}

static int gru_dump_context(struct gru_state *gru, int ctxnum,
		void __user *ubuf, void __user *ubufend, char data_opt,
		char lock_cch, char flush_cbrs)
{
	struct gru_dump_context_header hdr;
	struct gru_dump_context_header __user *uhdr = ubuf;
	struct gru_context_configuration_handle *cch, *ubufcch;
	struct gru_thread_state *gts;
	int try, cch_locked, cbrcnt = 0, dsrcnt = 0, bytes = 0, ret = 0;
	void *grubase;

	memset(&hdr, 0, sizeof(hdr));
	grubase = gru->gs_gru_base_vaddr;
	cch = get_cch(grubase, ctxnum);
	for (try = 0; try < CCH_LOCK_ATTEMPTS; try++) {
		cch_locked =  trylock_cch_handle(cch);
		if (cch_locked)
			break;
		msleep(1);
	}

	ubuf += sizeof(hdr);
	ubufcch = ubuf;
	if (gru_user_copy_handle(&ubuf, cch))
		goto fail;
	if (cch_locked)
		ubufcch->delresp = 0;
	bytes = sizeof(hdr) + GRU_CACHE_LINE_BYTES;

	if (cch_locked || !lock_cch) {
		gts = gru->gs_gts[ctxnum];
		if (gts && gts->ts_vma) {
			hdr.pid = gts->ts_tgid_owner;
			hdr.vaddr = gts->ts_vma->vm_start;
		}
		if (cch->state != CCHSTATE_INACTIVE) {
			cbrcnt = hweight64(cch->cbr_allocation_map) *
						GRU_CBR_AU_SIZE;
			dsrcnt = data_opt ? hweight32(cch->dsr_allocation_map) *
						GRU_DSR_AU_CL : 0;
		}
		bytes += (3 * cbrcnt + dsrcnt) * GRU_CACHE_LINE_BYTES;
		if (bytes > ubufend - ubuf)
			ret = -EFBIG;
		else
			ret = gru_dump_context_data(grubase, cch, ubuf, ctxnum,
							dsrcnt, flush_cbrs);
	}
	if (cch_locked)
		unlock_cch_handle(cch);
	if (ret)
		return ret;

	hdr.magic = GRU_DUMP_MAGIC;
	hdr.gid = gru->gs_gid;
	hdr.ctxnum = ctxnum;
	hdr.cbrcnt = cbrcnt;
	hdr.dsrcnt = dsrcnt;
	hdr.cch_locked = cch_locked;
	if (!ret && copy_to_user((void __user *)uhdr, &hdr, sizeof(hdr)))
		ret = -EFAULT;

	return ret ? ret : bytes;

fail:
	unlock_cch_handle(cch);
	return -EFAULT;
}

int gru_dump_chiplet_request(unsigned long arg)
{
	struct gru_state *gru;
	struct gru_dump_chiplet_state_req req;
	void __user *ubuf;
	void __user *ubufend;
	int ctxnum, ret, cnt = 0;

	if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
		return -EFAULT;

	/* Currently, only dump by gid is implemented */
	if (req.gid >= gru_max_gids || req.gid < 0)
		return -EINVAL;

	gru = GID_TO_GRU(req.gid);
	ubuf = req.buf;
	ubufend = req.buf + req.buflen;

	ret = gru_dump_tfm(gru, ubuf, ubufend);
	if (ret < 0)
		goto fail;
	ubuf += ret;

	ret = gru_dump_tgh(gru, ubuf, ubufend);
	if (ret < 0)
		goto fail;
	ubuf += ret;

	for (ctxnum = 0; ctxnum < GRU_NUM_CCH; ctxnum++) {
		if (req.ctxnum == ctxnum || req.ctxnum < 0) {
			ret = gru_dump_context(gru, ctxnum, ubuf, ubufend,
						req.data_opt, req.lock_cch,
						req.flush_cbrs);
			if (ret < 0)
				goto fail;
			ubuf += ret;
			cnt++;
		}
	}

	if (copy_to_user((void __user *)arg, &req, sizeof(req)))
		return -EFAULT;
	return cnt;

fail:
	return ret;
}
