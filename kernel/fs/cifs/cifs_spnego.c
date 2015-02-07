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
 *   fs/cifs/cifs_spnego.c -- SPNEGO upcall management for CIFS
 *
 *   Copyright (c) 2007 Red Hat, Inc.
 *   Author(s): Jeff Layton (jlayton@redhat.com)
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <keys/user-type.h>
#include <linux/key-type.h>
#include <linux/inet.h>
#include "cifsglob.h"
#include "cifs_spnego.h"
#include "cifs_debug.h"

/* create a new cifs key */
static int
cifs_spnego_key_instantiate(struct key *key, const void *data, size_t datalen)
{
	char *payload;
	int ret;

	ret = -ENOMEM;
	payload = kmalloc(datalen, GFP_KERNEL);
	if (!payload)
		goto error;

	/* attach the data */
	memcpy(payload, data, datalen);
	key->payload.data = payload;
	ret = 0;

error:
	return ret;
}

static void
cifs_spnego_key_destroy(struct key *key)
{
	kfree(key->payload.data);
}


/*
 * keytype for CIFS spnego keys
 */
struct key_type cifs_spnego_key_type = {
	.name		= "cifs.spnego",
	.instantiate	= cifs_spnego_key_instantiate,
	.match		= user_match,
	.destroy	= cifs_spnego_key_destroy,
	.describe	= user_describe,
};

/* length of longest version string e.g.  strlen("ver=0xFF") */
#define MAX_VER_STR_LEN		8

/* length of longest security mechanism name, eg in future could have
 * strlen(";sec=ntlmsspi") */
#define MAX_MECH_STR_LEN	13

/* strlen of "host=" */
#define HOST_KEY_LEN		5

/* strlen of ";ip4=" or ";ip6=" */
#define IP_KEY_LEN		5

/* strlen of ";uid=0x" */
#define UID_KEY_LEN		7

/* strlen of ";user=" */
#define USER_KEY_LEN		6

/* strlen of ";pid=0x" */
#define PID_KEY_LEN		7

/* get a key struct with a SPNEGO security blob, suitable for session setup */
struct key *
cifs_get_spnego_key(struct cifsSesInfo *sesInfo)
{
	struct TCP_Server_Info *server = sesInfo->server;
	char *description, *dp;
	size_t desc_len;
	struct key *spnego_key;
	const char *hostname = server->hostname;

	/* length of fields (with semicolons): ver=0xyz ip4=ipaddress
	   host=hostname sec=mechanism uid=0xFF user=username */
	desc_len = MAX_VER_STR_LEN +
		   HOST_KEY_LEN + strlen(hostname) +
		   IP_KEY_LEN + INET6_ADDRSTRLEN +
		   MAX_MECH_STR_LEN +
		   UID_KEY_LEN + (sizeof(uid_t) * 2) +
		   USER_KEY_LEN + strlen(sesInfo->userName) +
		   PID_KEY_LEN + (sizeof(pid_t) * 2) + 1;

	spnego_key = ERR_PTR(-ENOMEM);
	description = kzalloc(desc_len, GFP_KERNEL);
	if (description == NULL)
		goto out;

	dp = description;
	/* start with version and hostname portion of UNC string */
	spnego_key = ERR_PTR(-EINVAL);
	sprintf(dp, "ver=0x%x;host=%s;", CIFS_SPNEGO_UPCALL_VERSION,
		hostname);
	dp = description + strlen(description);

	/* add the server address */
	if (server->addr.sockAddr.sin_family == AF_INET)
		sprintf(dp, "ip4=%pI4", &server->addr.sockAddr.sin_addr);
	else if (server->addr.sockAddr.sin_family == AF_INET6)
		sprintf(dp, "ip6=%pI6", &server->addr.sockAddr6.sin6_addr);
	else
		goto out;

	dp = description + strlen(description);

	/* for now, only sec=krb5 and sec=mskrb5 are valid */
	if (server->sec_kerberos)
		sprintf(dp, ";sec=krb5");
	else if (server->sec_mskerberos)
		sprintf(dp, ";sec=mskrb5");
	else
		goto out;

	dp = description + strlen(description);
	sprintf(dp, ";uid=0x%x", sesInfo->linux_uid);

	dp = description + strlen(description);
	sprintf(dp, ";user=%s", sesInfo->userName);

	dp = description + strlen(description);
	sprintf(dp, ";pid=0x%x", current->pid);

	cFYI(1, "key description = %s", description);
	spnego_key = request_key(&cifs_spnego_key_type, description, "");

#ifdef CONFIG_CIFS_DEBUG2
	if (cifsFYI && !IS_ERR(spnego_key)) {
		struct cifs_spnego_msg *msg = spnego_key->payload.data;
		cifs_dump_mem("SPNEGO reply blob:", msg->data, min(1024U,
				msg->secblob_len + msg->sesskey_len));
	}
#endif /* CONFIG_CIFS_DEBUG2 */

out:
	kfree(description);
	return spnego_key;
}
