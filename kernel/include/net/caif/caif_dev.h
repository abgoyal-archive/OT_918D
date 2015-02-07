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
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	Sjur Brendeland/ sjur.brandeland@stericsson.com
 * License terms: GNU General Public License (GPL) version 2
 */

#ifndef CAIF_DEV_H_
#define CAIF_DEV_H_

#include <net/caif/caif_layer.h>
#include <net/caif/cfcnfg.h>
#include <linux/caif/caif_socket.h>
#include <linux/if.h>

/**
 * struct caif_param - CAIF parameters.
 * @size:	Length of data
 * @data:	Binary Data Blob
 */
struct caif_param {
	u16  size;
	u8   data[256];
};

/**
 * struct caif_connect_request - Request data for CAIF channel setup.
 * @protocol:		Type of CAIF protocol to use (at, datagram etc)
 * @sockaddr:		Socket address to connect.
 * @priority:		Priority of the connection.
 * @link_selector:	Link selector (high bandwidth or low latency)
 * @link_name:		Name of the CAIF Link Layer to use.
 * @param:		Connect Request parameters (CAIF_SO_REQ_PARAM).
 *
 * This struct is used when connecting a CAIF channel.
 * It contains all CAIF channel configuration options.
 */
struct caif_connect_request {
	enum caif_protocol_type protocol;
	struct sockaddr_caif sockaddr;
	enum caif_channel_priority priority;
	enum caif_link_selector link_selector;
	char link_name[16];
	struct caif_param param;
};

/**
 * caif_connect_client - Connect a client to CAIF Core Stack.
 * @config:		Channel setup parameters, specifying what address
 *			to connect on the Modem.
 * @client_layer:	User implementation of client layer. This layer
 *			MUST have receive and control callback functions
 *			implemented.
 *
 * This function connects a CAIF channel. The Client must implement
 * the struct cflayer. This layer represents the Client layer and holds
 * receive functions and control callback functions. Control callback
 * function will receive information about connect/disconnect responses,
 * flow control etc (see enum caif_control).
 * E.g. CAIF Socket will call this function for each socket it connects
 * and have one client_layer instance for each socket.
 */
int caif_connect_client(struct caif_connect_request *config,
			   struct cflayer *client_layer);

/**
 * caif_disconnect_client - Disconnects a client from the CAIF stack.
 *
 * @client_layer: Client layer to be removed.
 */
int caif_disconnect_client(struct cflayer *client_layer);

/**
 * caif_release_client - Release adaptation layer reference to client.
 *
 * @client_layer: Client layer.
 *
 * Releases a client/adaptation layer use of the caif stack.
 * This function must be used after caif_disconnect_client to
 * decrease the reference count of the service layer.
 */
void caif_release_client(struct cflayer *client_layer);

/**
 * connect_req_to_link_param - Translate configuration parameters
 *				from socket format to internal format.
 * @cnfg:	Pointer to configuration handler
 * @con_req:	Configuration parameters supplied in function
 *		caif_connect_client
 * @channel_setup_param: Parameters supplied to the CAIF Core stack for
 *			 setting up channels.
 *
 */
int connect_req_to_link_param(struct cfcnfg *cnfg,
				struct caif_connect_request *con_req,
				struct cfctrl_link_param *channel_setup_param);

/**
 * get_caif_conf() - Get the configuration handler.
 */
struct cfcnfg *get_caif_conf(void);


#endif /* CAIF_DEV_H_ */
