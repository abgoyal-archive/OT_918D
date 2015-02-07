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
 * Copyright (c) 2005 Voltaire Inc.  All rights reserved.
 * Copyright (c) 2005 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
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

#if !defined(IB_ADDR_H)
#define IB_ADDR_H

#include <linux/in.h>
#include <linux/in6.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <rdma/ib_verbs.h>

struct rdma_addr_client {
	atomic_t refcount;
	struct completion comp;
};

/**
 * rdma_addr_register_client - Register an address client.
 */
void rdma_addr_register_client(struct rdma_addr_client *client);

/**
 * rdma_addr_unregister_client - Deregister an address client.
 * @client: Client object to deregister.
 */
void rdma_addr_unregister_client(struct rdma_addr_client *client);

struct rdma_dev_addr {
	unsigned char src_dev_addr[MAX_ADDR_LEN];
	unsigned char dst_dev_addr[MAX_ADDR_LEN];
	unsigned char broadcast[MAX_ADDR_LEN];
	unsigned short dev_type;
	int bound_dev_if;
};

/**
 * rdma_translate_ip - Translate a local IP address to an RDMA hardware
 *   address.
 */
int rdma_translate_ip(struct sockaddr *addr, struct rdma_dev_addr *dev_addr);

/**
 * rdma_resolve_ip - Resolve source and destination IP addresses to
 *   RDMA hardware addresses.
 * @client: Address client associated with request.
 * @src_addr: An optional source address to use in the resolution.  If a
 *   source address is not provided, a usable address will be returned via
 *   the callback.
 * @dst_addr: The destination address to resolve.
 * @addr: A reference to a data location that will receive the resolved
 *   addresses.  The data location must remain valid until the callback has
 *   been invoked.
 * @timeout_ms: Amount of time to wait for the address resolution to complete.
 * @callback: Call invoked once address resolution has completed, timed out,
 *   or been canceled.  A status of 0 indicates success.
 * @context: User-specified context associated with the call.
 */
int rdma_resolve_ip(struct rdma_addr_client *client,
		    struct sockaddr *src_addr, struct sockaddr *dst_addr,
		    struct rdma_dev_addr *addr, int timeout_ms,
		    void (*callback)(int status, struct sockaddr *src_addr,
				     struct rdma_dev_addr *addr, void *context),
		    void *context);

void rdma_addr_cancel(struct rdma_dev_addr *addr);

int rdma_copy_addr(struct rdma_dev_addr *dev_addr, struct net_device *dev,
	      const unsigned char *dst_dev_addr);

static inline int ip_addr_size(struct sockaddr *addr)
{
	return addr->sa_family == AF_INET6 ?
	       sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
}

static inline u16 ib_addr_get_pkey(struct rdma_dev_addr *dev_addr)
{
	return ((u16)dev_addr->broadcast[8] << 8) | (u16)dev_addr->broadcast[9];
}

static inline void ib_addr_set_pkey(struct rdma_dev_addr *dev_addr, u16 pkey)
{
	dev_addr->broadcast[8] = pkey >> 8;
	dev_addr->broadcast[9] = (unsigned char) pkey;
}

static inline void ib_addr_get_mgid(struct rdma_dev_addr *dev_addr,
				    union ib_gid *gid)
{
	memcpy(gid, dev_addr->broadcast + 4, sizeof *gid);
}

static inline int rdma_addr_gid_offset(struct rdma_dev_addr *dev_addr)
{
	return dev_addr->dev_type == ARPHRD_INFINIBAND ? 4 : 0;
}

static inline void rdma_addr_get_sgid(struct rdma_dev_addr *dev_addr, union ib_gid *gid)
{
	memcpy(gid, dev_addr->src_dev_addr + rdma_addr_gid_offset(dev_addr), sizeof *gid);
}

static inline void rdma_addr_set_sgid(struct rdma_dev_addr *dev_addr, union ib_gid *gid)
{
	memcpy(dev_addr->src_dev_addr + rdma_addr_gid_offset(dev_addr), gid, sizeof *gid);
}

static inline void rdma_addr_get_dgid(struct rdma_dev_addr *dev_addr, union ib_gid *gid)
{
	memcpy(gid, dev_addr->dst_dev_addr + rdma_addr_gid_offset(dev_addr), sizeof *gid);
}

static inline void rdma_addr_set_dgid(struct rdma_dev_addr *dev_addr, union ib_gid *gid)
{
	memcpy(dev_addr->dst_dev_addr + rdma_addr_gid_offset(dev_addr), gid, sizeof *gid);
}

#endif /* IB_ADDR_H */
