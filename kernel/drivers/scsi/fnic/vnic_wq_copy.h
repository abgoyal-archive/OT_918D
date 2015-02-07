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
 * Copyright 2008 Cisco Systems, Inc.  All rights reserved.
 * Copyright 2007 Nuova Systems, Inc.  All rights reserved.
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _VNIC_WQ_COPY_H_
#define _VNIC_WQ_COPY_H_

#include <linux/pci.h>
#include "vnic_wq.h"
#include "fcpio.h"

#define	VNIC_WQ_COPY_MAX 1

struct vnic_wq_copy {
	unsigned int index;
	struct vnic_dev *vdev;
	struct vnic_wq_ctrl __iomem *ctrl;	/* memory-mapped */
	struct vnic_dev_ring ring;
	unsigned to_use_index;
	unsigned to_clean_index;
};

static inline unsigned int vnic_wq_copy_desc_avail(struct vnic_wq_copy *wq)
{
	return wq->ring.desc_avail;
}

static inline unsigned int vnic_wq_copy_desc_in_use(struct vnic_wq_copy *wq)
{
	return wq->ring.desc_count - 1 - wq->ring.desc_avail;
}

static inline void *vnic_wq_copy_next_desc(struct vnic_wq_copy *wq)
{
	struct fcpio_host_req *desc = wq->ring.descs;
	return &desc[wq->to_use_index];
}

static inline void vnic_wq_copy_post(struct vnic_wq_copy *wq)
{

	((wq->to_use_index + 1) == wq->ring.desc_count) ?
		(wq->to_use_index = 0) : (wq->to_use_index++);
	wq->ring.desc_avail--;

	/* Adding write memory barrier prevents compiler and/or CPU
	 * reordering, thus avoiding descriptor posting before
	 * descriptor is initialized. Otherwise, hardware can read
	 * stale descriptor fields.
	 */
	wmb();

	iowrite32(wq->to_use_index, &wq->ctrl->posted_index);
}

static inline void vnic_wq_copy_desc_process(struct vnic_wq_copy *wq, u16 index)
{
	unsigned int cnt;

	if (wq->to_clean_index <= index)
		cnt = (index - wq->to_clean_index) + 1;
	else
		cnt = wq->ring.desc_count - wq->to_clean_index + index + 1;

	wq->to_clean_index = ((index + 1) % wq->ring.desc_count);
	wq->ring.desc_avail += cnt;

}

static inline void vnic_wq_copy_service(struct vnic_wq_copy *wq,
	u16 completed_index,
	void (*q_service)(struct vnic_wq_copy *wq,
	struct fcpio_host_req *wq_desc))
{
	struct fcpio_host_req *wq_desc = wq->ring.descs;
	unsigned int curr_index;

	while (1) {

		if (q_service)
			(*q_service)(wq, &wq_desc[wq->to_clean_index]);

		wq->ring.desc_avail++;

		curr_index = wq->to_clean_index;

		/* increment the to-clean index so that we start
		 * with an unprocessed index next time we enter the loop
		 */
		((wq->to_clean_index + 1) == wq->ring.desc_count) ?
			(wq->to_clean_index = 0) : (wq->to_clean_index++);

		if (curr_index == completed_index)
			break;

		/* we have cleaned all the entries */
		if ((completed_index == (u16)-1) &&
		    (wq->to_clean_index == wq->to_use_index))
			break;
	}
}

void vnic_wq_copy_enable(struct vnic_wq_copy *wq);
int vnic_wq_copy_disable(struct vnic_wq_copy *wq);
void vnic_wq_copy_free(struct vnic_wq_copy *wq);
int vnic_wq_copy_alloc(struct vnic_dev *vdev, struct vnic_wq_copy *wq,
	unsigned int index, unsigned int desc_count, unsigned int desc_size);
void vnic_wq_copy_init(struct vnic_wq_copy *wq, unsigned int cq_index,
	unsigned int error_interrupt_enable,
	unsigned int error_interrupt_offset);
void vnic_wq_copy_clean(struct vnic_wq_copy *wq,
	void (*q_clean)(struct vnic_wq_copy *wq,
	struct fcpio_host_req *wq_desc));

#endif /* _VNIC_WQ_COPY_H_ */
