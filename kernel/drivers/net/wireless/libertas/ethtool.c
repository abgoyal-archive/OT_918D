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

#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/delay.h>

#include "host.h"
#include "decl.h"
#include "defs.h"
#include "dev.h"
#include "wext.h"
#include "cmd.h"
#include "mesh.h"


static void lbs_ethtool_get_drvinfo(struct net_device *dev,
					 struct ethtool_drvinfo *info)
{
	struct lbs_private *priv = dev->ml_priv;

	snprintf(info->fw_version, 32, "%u.%u.%u.p%u",
		priv->fwrelease >> 24 & 0xff,
		priv->fwrelease >> 16 & 0xff,
		priv->fwrelease >>  8 & 0xff,
		priv->fwrelease       & 0xff);
	strcpy(info->driver, "libertas");
	strcpy(info->version, lbs_driver_version);
}

/* All 8388 parts have 16KiB EEPROM size at the time of writing.
 * In case that changes this needs fixing.
 */
#define LBS_EEPROM_LEN 16384

static int lbs_ethtool_get_eeprom_len(struct net_device *dev)
{
	return LBS_EEPROM_LEN;
}

static int lbs_ethtool_get_eeprom(struct net_device *dev,
                                  struct ethtool_eeprom *eeprom, u8 * bytes)
{
	struct lbs_private *priv = dev->ml_priv;
	struct cmd_ds_802_11_eeprom_access cmd;
	int ret;

	lbs_deb_enter(LBS_DEB_ETHTOOL);

	if (eeprom->offset + eeprom->len > LBS_EEPROM_LEN ||
	    eeprom->len > LBS_EEPROM_READ_LEN) {
		ret = -EINVAL;
		goto out;
	}

	cmd.hdr.size = cpu_to_le16(sizeof(struct cmd_ds_802_11_eeprom_access) -
		LBS_EEPROM_READ_LEN + eeprom->len);
	cmd.action = cpu_to_le16(CMD_ACT_GET);
	cmd.offset = cpu_to_le16(eeprom->offset);
	cmd.len    = cpu_to_le16(eeprom->len);
	ret = lbs_cmd_with_response(priv, CMD_802_11_EEPROM_ACCESS, &cmd);
	if (!ret)
		memcpy(bytes, cmd.value, eeprom->len);

out:
	lbs_deb_leave_args(LBS_DEB_ETHTOOL, "ret %d", ret);
        return ret;
}

static void lbs_ethtool_get_wol(struct net_device *dev,
				struct ethtool_wolinfo *wol)
{
	struct lbs_private *priv = dev->ml_priv;

	if (priv->wol_criteria == 0xffffffff) {
		/* Interface driver didn't configure wake */
		wol->supported = wol->wolopts = 0;
		return;
	}

	wol->supported = WAKE_UCAST|WAKE_MCAST|WAKE_BCAST|WAKE_PHY;

	if (priv->wol_criteria & EHS_WAKE_ON_UNICAST_DATA)
		wol->wolopts |= WAKE_UCAST;
	if (priv->wol_criteria & EHS_WAKE_ON_MULTICAST_DATA)
		wol->wolopts |= WAKE_MCAST;
	if (priv->wol_criteria & EHS_WAKE_ON_BROADCAST_DATA)
		wol->wolopts |= WAKE_BCAST;
	if (priv->wol_criteria & EHS_WAKE_ON_MAC_EVENT)
		wol->wolopts |= WAKE_PHY;
}

static int lbs_ethtool_set_wol(struct net_device *dev,
			       struct ethtool_wolinfo *wol)
{
	struct lbs_private *priv = dev->ml_priv;
	uint32_t criteria = 0;

	if (wol->wolopts & ~(WAKE_UCAST|WAKE_MCAST|WAKE_BCAST|WAKE_PHY))
		return -EOPNOTSUPP;

	if (wol->wolopts & WAKE_UCAST)
		criteria |= EHS_WAKE_ON_UNICAST_DATA;
	if (wol->wolopts & WAKE_MCAST)
		criteria |= EHS_WAKE_ON_MULTICAST_DATA;
	if (wol->wolopts & WAKE_BCAST)
		criteria |= EHS_WAKE_ON_BROADCAST_DATA;
	if (wol->wolopts & WAKE_PHY)
		criteria |= EHS_WAKE_ON_MAC_EVENT;
	if (wol->wolopts == 0)
		criteria |= EHS_REMOVE_WAKEUP;

	return lbs_host_sleep_cfg(priv, criteria, (struct wol_config *)NULL);
}

const struct ethtool_ops lbs_ethtool_ops = {
	.get_drvinfo = lbs_ethtool_get_drvinfo,
	.get_eeprom =  lbs_ethtool_get_eeprom,
	.get_eeprom_len = lbs_ethtool_get_eeprom_len,
#ifdef CONFIG_LIBERTAS_MESH
	.get_sset_count = lbs_mesh_ethtool_get_sset_count,
	.get_ethtool_stats = lbs_mesh_ethtool_get_stats,
	.get_strings = lbs_mesh_ethtool_get_strings,
#endif
	.get_wol = lbs_ethtool_get_wol,
	.set_wol = lbs_ethtool_set_wol,
};

