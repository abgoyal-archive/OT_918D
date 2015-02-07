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

#ifndef _TRACE_SOCKET_H
#define _TRACE_SOCKET_H

#include <net/sock.h>
#include <linux/tracepoint.h>

DECLARE_TRACE(socket_create,
	TP_PROTO(int family, int type, int protocol, struct socket *sock,
	int ret),
	TP_ARGS(family, type, protocol, sock, ret));

DECLARE_TRACE(socket_bind,
	TP_PROTO(int fd, struct sockaddr __user *umyaddr, int addrlen, int ret),
	TP_ARGS(fd, umyaddr, addrlen, ret));

DECLARE_TRACE(socket_connect,
	TP_PROTO(int fd, struct sockaddr __user *uservaddr, int addrlen,
	int ret),
	TP_ARGS(fd, uservaddr, addrlen, ret));

DECLARE_TRACE(socket_listen,
	TP_PROTO(int fd, int backlog, int ret),
	TP_ARGS(fd, backlog, ret));

DECLARE_TRACE(socket_accept,
	TP_PROTO(int fd, struct sockaddr __user *upeer_sockaddr,
	int __user *upeer_addrlen, int flags, int ret),
	TP_ARGS(fd, upeer_sockaddr, upeer_addrlen, flags, ret));

DECLARE_TRACE(socket_getsockname,
	TP_PROTO(int fd, struct sockaddr __user *usockaddr,
	int __user *usockaddr_len, int ret),
	TP_ARGS(fd, usockaddr, usockaddr_len, ret));

DECLARE_TRACE(socket_getpeername,
	TP_PROTO(int fd, struct sockaddr __user *usockaddr,
	int __user *usockaddr_len, int ret),
	TP_ARGS(fd, usockaddr, usockaddr_len, ret));

DECLARE_TRACE(socket_socketpair,
	TP_PROTO(int family, int type, int protocol, int __user *usockvec,
	int ret),
	TP_ARGS(family, type, protocol, usockvec, ret));

DECLARE_TRACE(socket_sendmsg,
	TP_PROTO(struct socket *sock, struct msghdr *msg, size_t size, int ret),
	TP_ARGS(sock, msg, size, ret));

DECLARE_TRACE(socket_recvmsg,
	TP_PROTO(struct socket *sock, struct msghdr *msg, size_t size,
		int flags, int ret),
	TP_ARGS(sock, msg, size, flags, ret));

DECLARE_TRACE(socket_setsockopt,
	TP_PROTO(int fd, int level, int optname, char __user *optval,
	int optlen, int ret),
	TP_ARGS(fd, level, optname, optval, optlen, ret));

DECLARE_TRACE(socket_getsockopt,
	TP_PROTO(int fd, int level, int optname, char __user *optval,
	int __user *optlen, int ret),
	TP_ARGS(fd, level, optname, optval, optlen, ret));

DECLARE_TRACE(socket_shutdown,
	TP_PROTO(int fd, int how, int ret),
	TP_ARGS(fd, how, ret));

/*
 * socket_call
 *
 * We also trace socket_call so we can know which syscall is used by user
 * (socket_call or sock_send...)
 */
DECLARE_TRACE(socket_call,
	TP_PROTO(int call, unsigned long a0),
	TP_ARGS(call, a0));
#endif
