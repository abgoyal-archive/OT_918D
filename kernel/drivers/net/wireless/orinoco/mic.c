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

/* Orinoco MIC helpers
 *
 * See copyright notice in main.c
 */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/if_ether.h>
#include <linux/scatterlist.h>
#include <linux/crypto.h>

#include "orinoco.h"
#include "mic.h"

/********************************************************************/
/* Michael MIC crypto setup                                         */
/********************************************************************/
int orinoco_mic_init(struct orinoco_private *priv)
{
	priv->tx_tfm_mic = crypto_alloc_hash("michael_mic", 0, 0);
	if (IS_ERR(priv->tx_tfm_mic)) {
		printk(KERN_DEBUG "orinoco_mic_init: could not allocate "
		       "crypto API michael_mic\n");
		priv->tx_tfm_mic = NULL;
		return -ENOMEM;
	}

	priv->rx_tfm_mic = crypto_alloc_hash("michael_mic", 0, 0);
	if (IS_ERR(priv->rx_tfm_mic)) {
		printk(KERN_DEBUG "orinoco_mic_init: could not allocate "
		       "crypto API michael_mic\n");
		priv->rx_tfm_mic = NULL;
		return -ENOMEM;
	}

	return 0;
}

void orinoco_mic_free(struct orinoco_private *priv)
{
	if (priv->tx_tfm_mic)
		crypto_free_hash(priv->tx_tfm_mic);
	if (priv->rx_tfm_mic)
		crypto_free_hash(priv->rx_tfm_mic);
}

int orinoco_mic(struct crypto_hash *tfm_michael, u8 *key,
		u8 *da, u8 *sa, u8 priority,
		u8 *data, size_t data_len, u8 *mic)
{
	struct hash_desc desc;
	struct scatterlist sg[2];
	u8 hdr[ETH_HLEN + 2]; /* size of header + padding */

	if (tfm_michael == NULL) {
		printk(KERN_WARNING "orinoco_mic: tfm_michael == NULL\n");
		return -1;
	}

	/* Copy header into buffer. We need the padding on the end zeroed */
	memcpy(&hdr[0], da, ETH_ALEN);
	memcpy(&hdr[ETH_ALEN], sa, ETH_ALEN);
	hdr[ETH_ALEN*2] = priority;
	hdr[ETH_ALEN*2+1] = 0;
	hdr[ETH_ALEN*2+2] = 0;
	hdr[ETH_ALEN*2+3] = 0;

	/* Use scatter gather to MIC header and data in one go */
	sg_init_table(sg, 2);
	sg_set_buf(&sg[0], hdr, sizeof(hdr));
	sg_set_buf(&sg[1], data, data_len);

	if (crypto_hash_setkey(tfm_michael, key, MIC_KEYLEN))
		return -1;

	desc.tfm = tfm_michael;
	desc.flags = 0;
	return crypto_hash_digest(&desc, sg, data_len + sizeof(hdr),
				  mic);
}
