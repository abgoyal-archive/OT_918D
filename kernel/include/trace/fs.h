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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef _TRACE_FS_H
#define _TRACE_FS_H

#include <linux/buffer_head.h>
#include <linux/time.h>
#include <linux/tracepoint.h>

DECLARE_TRACE(fs_buffer_wait_start,
	TP_PROTO(struct buffer_head *bh),
	TP_ARGS(bh));
DECLARE_TRACE(fs_buffer_wait_end,
	TP_PROTO(struct buffer_head *bh),
	TP_ARGS(bh));
DECLARE_TRACE(fs_exec,
	TP_PROTO(char *filename),
	TP_ARGS(filename));
DECLARE_TRACE(fs_ioctl,
	TP_PROTO(unsigned int fd, unsigned int cmd, unsigned long arg),
	TP_ARGS(fd, cmd, arg));
DECLARE_TRACE(fs_open,
	TP_PROTO(int fd, char *filename),
	TP_ARGS(fd, filename));
DECLARE_TRACE(fs_close,
	TP_PROTO(unsigned int fd),
	TP_ARGS(fd));
DECLARE_TRACE(fs_lseek,
	TP_PROTO(unsigned int fd, long offset, unsigned int origin),
	TP_ARGS(fd, offset, origin));
DECLARE_TRACE(fs_llseek,
	TP_PROTO(unsigned int fd, loff_t offset, unsigned int origin),
	TP_ARGS(fd, offset, origin));

/*
 * Probes must be aware that __user * may be modified by concurrent userspace
 * or kernel threads.
 */
DECLARE_TRACE(fs_read,
	TP_PROTO(unsigned int fd, char __user *buf, size_t count, ssize_t ret),
	TP_ARGS(fd, buf, count, ret));
DECLARE_TRACE(fs_write,
	TP_PROTO(unsigned int fd, const char __user *buf, size_t count,
		ssize_t ret),
	TP_ARGS(fd, buf, count, ret));
DECLARE_TRACE(fs_pread64,
	TP_PROTO(unsigned int fd, char __user *buf, size_t count, loff_t pos,
		ssize_t ret),
	TP_ARGS(fd, buf, count, pos, ret));
DECLARE_TRACE(fs_pwrite64,
	TP_PROTO(unsigned int fd, const char __user *buf, size_t count,
		loff_t pos, ssize_t ret),
	TP_ARGS(fd, buf, count, pos, ret));
DECLARE_TRACE(fs_readv,
	TP_PROTO(unsigned long fd, const struct iovec __user *vec,
		unsigned long vlen, ssize_t ret),
	TP_ARGS(fd, vec, vlen, ret));
DECLARE_TRACE(fs_writev,
	TP_PROTO(unsigned long fd, const struct iovec __user *vec,
		unsigned long vlen, ssize_t ret),
	TP_ARGS(fd, vec, vlen, ret));
DECLARE_TRACE(fs_select,
	TP_PROTO(int fd, struct timespec *end_time),
	TP_ARGS(fd, end_time));
DECLARE_TRACE(fs_poll,
	TP_PROTO(int fd),
	TP_ARGS(fd));
#endif
