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

/* ------------------------------------------------------------
 * ibmvscsi.h
 * (C) Copyright IBM Corporation 1994, 2003
 * Authors: Colin DeVilbiss (devilbis@us.ibm.com)
 *          Santiago Leon (santil@us.ibm.com)
 *          Dave Boutcher (sleddog@us.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
 * ------------------------------------------------------------
 * Emulation of a SCSI host adapter for Virtual I/O devices
 *
 * This driver allows the Linux SCSI peripheral drivers to directly
 * access devices in the hosting partition, either on an iSeries
 * hypervisor system or a converged hypervisor system.
 */
#ifndef IBMVSCSI_H
#define IBMVSCSI_H
#include <linux/types.h>
#include <linux/list.h>
#include <linux/completion.h>
#include <linux/interrupt.h>
#include "viosrp.h"

struct scsi_cmnd;
struct Scsi_Host;

/* Number of indirect bufs...the list of these has to fit in the
 * additional data of the srp_cmd struct along with the indirect
 * descriptor
 */
#define MAX_INDIRECT_BUFS 10

#define IBMVSCSI_MAX_REQUESTS_DEFAULT 100
#define IBMVSCSI_CMDS_PER_LUN_DEFAULT 16
#define IBMVSCSI_MAX_SECTORS_DEFAULT 256 /* 32 * 8 = default max I/O 32 pages */
#define IBMVSCSI_MAX_CMDS_PER_LUN 64

/* ------------------------------------------------------------
 * Data Structures
 */
/* an RPA command/response transport queue */
struct crq_queue {
	struct viosrp_crq *msgs;
	int size, cur;
	dma_addr_t msg_token;
	spinlock_t lock;
};

/* a unit of work for the hosting partition */
struct srp_event_struct {
	union viosrp_iu *xfer_iu;
	struct scsi_cmnd *cmnd;
	struct list_head list;
	void (*done) (struct srp_event_struct *);
	struct viosrp_crq crq;
	struct ibmvscsi_host_data *hostdata;
	atomic_t free;
	union viosrp_iu iu;
	void (*cmnd_done) (struct scsi_cmnd *);
	struct completion comp;
	struct timer_list timer;
	union viosrp_iu *sync_srp;
	struct srp_direct_buf *ext_list;
	dma_addr_t ext_list_token;
};

/* a pool of event structs for use */
struct event_pool {
	struct srp_event_struct *events;
	u32 size;
	int next;
	union viosrp_iu *iu_storage;
	dma_addr_t iu_token;
};

/* all driver data associated with a host adapter */
struct ibmvscsi_host_data {
	atomic_t request_limit;
	int client_migrated;
	struct device *dev;
	struct event_pool pool;
	struct crq_queue queue;
	struct tasklet_struct srp_task;
	struct list_head sent;
	struct Scsi_Host *host;
	struct mad_adapter_info_data madapter_info;
	struct capabilities caps;
	dma_addr_t caps_addr;
	dma_addr_t adapter_info_addr;
};

/* routines for managing a command/response queue */
void ibmvscsi_handle_crq(struct viosrp_crq *crq,
			 struct ibmvscsi_host_data *hostdata);

struct ibmvscsi_ops {
	int (*init_crq_queue)(struct crq_queue *queue,
			      struct ibmvscsi_host_data *hostdata,
			      int max_requests);
	void (*release_crq_queue)(struct crq_queue *queue,
				  struct ibmvscsi_host_data *hostdata,
				  int max_requests);
	int (*reset_crq_queue)(struct crq_queue *queue,
			       struct ibmvscsi_host_data *hostdata);
	int (*reenable_crq_queue)(struct crq_queue *queue,
				  struct ibmvscsi_host_data *hostdata);
	int (*send_crq)(struct ibmvscsi_host_data *hostdata,
		       u64 word1, u64 word2);
	int (*resume) (struct ibmvscsi_host_data *hostdata);
};

extern struct ibmvscsi_ops iseriesvscsi_ops;
extern struct ibmvscsi_ops rpavscsi_ops;

#endif				/* IBMVSCSI_H */
