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
 * CAIF Framing Layer.
 *
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	Sjur Brendeland/sjur.brandeland@stericsson.com
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/stddef.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/crc-ccitt.h>
#include <net/caif/caif_layer.h>
#include <net/caif/cfpkt.h>
#include <net/caif/cffrml.h>

#define container_obj(layr) container_of(layr, struct cffrml, layer)

struct cffrml {
	struct cflayer layer;
	bool dofcs;		/* !< FCS active */
};

static int cffrml_receive(struct cflayer *layr, struct cfpkt *pkt);
static int cffrml_transmit(struct cflayer *layr, struct cfpkt *pkt);
static void cffrml_ctrlcmd(struct cflayer *layr, enum caif_ctrlcmd ctrl,
				int phyid);

static u32 cffrml_rcv_error;
static u32 cffrml_rcv_checsum_error;
struct cflayer *cffrml_create(u16 phyid, bool use_fcs)
{
	struct cffrml *this = kmalloc(sizeof(struct cffrml), GFP_ATOMIC);
	if (!this) {
		pr_warning("CAIF: %s(): Out of memory\n", __func__);
		return NULL;
	}
	caif_assert(offsetof(struct cffrml, layer) == 0);

	memset(this, 0, sizeof(struct cflayer));
	this->layer.receive = cffrml_receive;
	this->layer.transmit = cffrml_transmit;
	this->layer.ctrlcmd = cffrml_ctrlcmd;
	snprintf(this->layer.name, CAIF_LAYER_NAME_SZ, "frm%d", phyid);
	this->dofcs = use_fcs;
	this->layer.id = phyid;
	return (struct cflayer *) this;
}

void cffrml_set_uplayer(struct cflayer *this, struct cflayer *up)
{
	this->up = up;
}

void cffrml_set_dnlayer(struct cflayer *this, struct cflayer *dn)
{
	this->dn = dn;
}

static u16 cffrml_checksum(u16 chks, void *buf, u16 len)
{
	/* FIXME: FCS should be moved to glue in order to use OS-Specific
	 * solutions
	 */
	return crc_ccitt(chks, buf, len);
}

static int cffrml_receive(struct cflayer *layr, struct cfpkt *pkt)
{
	u16 tmp;
	u16 len;
	u16 hdrchks;
	u16 pktchks;
	struct cffrml *this;
	this = container_obj(layr);

	cfpkt_extr_head(pkt, &tmp, 2);
	len = le16_to_cpu(tmp);

	/* Subtract for FCS on length if FCS is not used. */
	if (!this->dofcs)
		len -= 2;

	if (cfpkt_setlen(pkt, len) < 0) {
		++cffrml_rcv_error;
		pr_err("CAIF: %s():Framing length error (%d)\n", __func__, len);
		cfpkt_destroy(pkt);
		return -EPROTO;
	}
	/*
	 * Don't do extract if FCS is false, rather do setlen - then we don't
	 * get a cache-miss.
	 */
	if (this->dofcs) {
		cfpkt_extr_trail(pkt, &tmp, 2);
		hdrchks = le16_to_cpu(tmp);
		pktchks = cfpkt_iterate(pkt, cffrml_checksum, 0xffff);
		if (pktchks != hdrchks) {
			cfpkt_add_trail(pkt, &tmp, 2);
			++cffrml_rcv_error;
			++cffrml_rcv_checsum_error;
			pr_info("CAIF: %s(): Frame checksum error "
				"(0x%x != 0x%x)\n", __func__, hdrchks, pktchks);
			return -EILSEQ;
		}
	}
	if (cfpkt_erroneous(pkt)) {
		++cffrml_rcv_error;
		pr_err("CAIF: %s(): Packet is erroneous!\n", __func__);
		cfpkt_destroy(pkt);
		return -EPROTO;
	}
	return layr->up->receive(layr->up, pkt);
}

static int cffrml_transmit(struct cflayer *layr, struct cfpkt *pkt)
{
	int tmp;
	u16 chks;
	u16 len;
	int ret;
	struct cffrml *this = container_obj(layr);
	if (this->dofcs) {
		chks = cfpkt_iterate(pkt, cffrml_checksum, 0xffff);
		tmp = cpu_to_le16(chks);
		cfpkt_add_trail(pkt, &tmp, 2);
	} else {
		cfpkt_pad_trail(pkt, 2);
	}
	len = cfpkt_getlen(pkt);
	tmp = cpu_to_le16(len);
	cfpkt_add_head(pkt, &tmp, 2);
	cfpkt_info(pkt)->hdr_len += 2;
	if (cfpkt_erroneous(pkt)) {
		pr_err("CAIF: %s(): Packet is erroneous!\n", __func__);
		return -EPROTO;
	}
	ret = layr->dn->transmit(layr->dn, pkt);
	if (ret < 0) {
		/* Remove header on faulty packet. */
		cfpkt_extr_head(pkt, &tmp, 2);
	}
	return ret;
}

static void cffrml_ctrlcmd(struct cflayer *layr, enum caif_ctrlcmd ctrl,
					int phyid)
{
	if (layr->up->ctrlcmd)
		layr->up->ctrlcmd(layr->up, ctrl, layr->id);
}
