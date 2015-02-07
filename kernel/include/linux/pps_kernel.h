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
 * PPS API kernel header
 *
 * Copyright (C) 2009   Rodolfo Giometti <giometti@linux.it>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/pps.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/time.h>

/*
 * Global defines
 */

/* The specific PPS source info */
struct pps_source_info {
	char name[PPS_MAX_NAME_LEN];		/* simbolic name */
	char path[PPS_MAX_NAME_LEN];		/* path of connected device */
	int mode;				/* PPS's allowed mode */

	void (*echo)(int source, int event, void *data); /* PPS echo function */

	struct module *owner;
	struct device *dev;
};

/* The main struct */
struct pps_device {
	struct pps_source_info info;		/* PSS source info */

	struct pps_kparams params;		/* PPS's current params */

	__u32 assert_sequence;			/* PPS' assert event seq # */
	__u32 clear_sequence;			/* PPS' clear event seq # */
	struct pps_ktime assert_tu;
	struct pps_ktime clear_tu;
	int current_mode;			/* PPS mode at event time */

	int go;					/* PPS event is arrived? */
	wait_queue_head_t queue;		/* PPS event queue */

	unsigned int id;			/* PPS source unique ID */
	struct cdev cdev;
	struct device *dev;
	int devno;
	struct fasync_struct *async_queue;	/* fasync method */
	spinlock_t lock;

	atomic_t usage;				/* usage count */
};

/*
 * Global variables
 */

extern spinlock_t pps_idr_lock;
extern struct idr pps_idr;
extern struct timespec pps_irq_ts[];

extern struct device_attribute pps_attrs[];

/*
 * Exported functions
 */

struct pps_device *pps_get_source(int source);
extern void pps_put_source(struct pps_device *pps);
extern int pps_register_source(struct pps_source_info *info,
				int default_params);
extern void pps_unregister_source(int source);
extern int pps_register_cdev(struct pps_device *pps);
extern void pps_unregister_cdev(struct pps_device *pps);
extern void pps_event(int source, struct pps_ktime *ts, int event, void *data);
