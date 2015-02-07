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
 * Author:	Sjur Brendeland/sjur.brandeland@stericsson.com
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/stddef.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <net/caif/caif_layer.h>
#include <net/caif/cfsrvl.h>
#include <net/caif/cfpkt.h>

#define container_obj(layr) container_of(layr, struct cfsrvl, layer)

#define RFM_SEGMENTATION_BIT 0x01
#define RFM_PAYLOAD  0x00
#define RFM_CMD_BIT  0x80
#define RFM_FLOW_OFF 0x81
#define RFM_FLOW_ON  0x80
#define RFM_SET_PIN  0x82
#define RFM_CTRL_PKT_SIZE 1

static int cfrfml_receive(struct cflayer *layr, struct cfpkt *pkt);
static int cfrfml_transmit(struct cflayer *layr, struct cfpkt *pkt);
static int cfservl_modemcmd(struct cflayer *layr, enum caif_modemcmd ctrl);

struct cflayer *cfrfml_create(u8 channel_id, struct dev_info *dev_info)
{
	struct cfsrvl *rfm = kmalloc(sizeof(struct cfsrvl), GFP_ATOMIC);
	if (!rfm) {
		pr_warning("CAIF: %s(): Out of memory\n", __func__);
		return NULL;
	}
	caif_assert(offsetof(struct cfsrvl, layer) == 0);
	memset(rfm, 0, sizeof(struct cfsrvl));
	cfsrvl_init(rfm, channel_id, dev_info);
	rfm->layer.modemcmd = cfservl_modemcmd;
	rfm->layer.receive = cfrfml_receive;
	rfm->layer.transmit = cfrfml_transmit;
	snprintf(rfm->layer.name, CAIF_LAYER_NAME_SZ, "rfm%d", channel_id);
	return &rfm->layer;
}

static int cfservl_modemcmd(struct cflayer *layr, enum caif_modemcmd ctrl)
{
       return -EPROTO;
}

static int cfrfml_receive(struct cflayer *layr, struct cfpkt *pkt)
{
	u8 tmp;
	bool segmented;
	int ret;
	caif_assert(layr->up != NULL);
	caif_assert(layr->receive != NULL);

	/*
	 * RFM is taking care of segmentation and stripping of
	 * segmentation bit.
	 */
	if (cfpkt_extr_head(pkt, &tmp, 1) < 0) {
		pr_err("CAIF: %s(): Packet is erroneous!\n", __func__);
		cfpkt_destroy(pkt);
		return -EPROTO;
	}
	segmented = tmp & RFM_SEGMENTATION_BIT;
	caif_assert(!segmented);

	ret = layr->up->receive(layr->up, pkt);
	return ret;
}

static int cfrfml_transmit(struct cflayer *layr, struct cfpkt *pkt)
{
	u8 tmp = 0;
	int ret;
	struct cfsrvl *service = container_obj(layr);

	caif_assert(layr->dn != NULL);
	caif_assert(layr->dn->transmit != NULL);

	if (!cfsrvl_ready(service, &ret))
		return ret;

	if (cfpkt_getlen(pkt) > CAIF_MAX_PAYLOAD_SIZE) {
		pr_err("CAIF: %s():Packet too large - size=%d\n",
			__func__, cfpkt_getlen(pkt));
		return -EOVERFLOW;
	}
	if (cfpkt_add_head(pkt, &tmp, 1) < 0) {
		pr_err("CAIF: %s(): Packet is erroneous!\n", __func__);
		return -EPROTO;
	}

	/* Add info for MUX-layer to route the packet out. */
	cfpkt_info(pkt)->channel_id = service->layer.id;
	/*
	 * To optimize alignment, we add up the size of CAIF header before
	 * payload.
	 */
	cfpkt_info(pkt)->hdr_len = 1;
	cfpkt_info(pkt)->dev_info = &service->dev_info;
	ret = layr->dn->transmit(layr->dn, pkt);
	if (ret < 0)
		cfpkt_extr_head(pkt, &tmp, 1);
	return ret;
}
