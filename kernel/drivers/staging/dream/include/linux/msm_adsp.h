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

/* drivers/staging/dream/include/linux/msm_adsp.h
 *
 * Copyright (c) QUALCOMM Incorporated
 * Copyright (C) 2007 Google, Inc.
 * Author: Iliyan Malchev <ibm@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LINUX_MSM_ADSP_H
#define __LINUX_MSM_ADSP_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define ADSP_IOCTL_MAGIC 'q'

/* ADSP_IOCTL_WRITE_COMMAND */
struct adsp_command_t {
	uint16_t queue;
	uint32_t len;		/* bytes */
	uint8_t *data;
};

/* ADSP_IOCTL_GET_EVENT */
struct adsp_event_t {
	uint16_t type;		/* 1 == event (RPC), 0 == message (adsp) */
	uint32_t timeout_ms;	/* -1 for infinite, 0 for immediate return */
	uint16_t msg_id;
	uint16_t flags;		/* 1 == 16--bit event, 0 == 32-bit event */
	uint32_t len;		/* size in, number of bytes out */
	uint8_t *data;
};

#define ADSP_IOCTL_ENABLE \
	_IOR(ADSP_IOCTL_MAGIC, 1, unsigned)

#define ADSP_IOCTL_DISABLE \
	_IOR(ADSP_IOCTL_MAGIC, 2, unsigned)

#define ADSP_IOCTL_DISABLE_ACK \
	_IOR(ADSP_IOCTL_MAGIC, 3, unsigned)

#define ADSP_IOCTL_WRITE_COMMAND \
	_IOR(ADSP_IOCTL_MAGIC, 4, struct adsp_command_t *)

#define ADSP_IOCTL_GET_EVENT \
	_IOWR(ADSP_IOCTL_MAGIC, 5, struct adsp_event_data_t *)

#define ADSP_IOCTL_SET_CLKRATE \
	_IOR(ADSP_IOCTL_MAGIC, 6, unsigned)

#define ADSP_IOCTL_DISABLE_EVENT_RSP \
	_IOR(ADSP_IOCTL_MAGIC, 10, unsigned)

struct adsp_pmem_info {
        int fd;
        void *vaddr;
};

#define ADSP_IOCTL_REGISTER_PMEM \
	_IOW(ADSP_IOCTL_MAGIC, 13, unsigned)

#define ADSP_IOCTL_UNREGISTER_PMEM \
	_IOW(ADSP_IOCTL_MAGIC, 14, unsigned)

/* Cause any further GET_EVENT ioctls to fail (-ENODEV)
 * until the device is closed and reopened.  Useful for
 * terminating event dispatch threads
 */
#define ADSP_IOCTL_ABORT_EVENT_READ \
	_IOW(ADSP_IOCTL_MAGIC, 15, unsigned)

#define ADSP_IOCTL_LINK_TASK \
	_IOW(ADSP_IOCTL_MAGIC, 16, unsigned)

#endif
