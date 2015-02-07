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
 * Code commons to all DaVinci SoCs.
 *
 * Author: Mark A. Greer <mgreer@mvista.com>
 *
 * 2009 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/etherdevice.h>
#include <linux/davinci_emac.h>

#include <asm/tlb.h>
#include <asm/mach/map.h>

#include <mach/common.h>
#include <mach/cputype.h>

#include "clock.h"

struct davinci_soc_info davinci_soc_info;
EXPORT_SYMBOL(davinci_soc_info);

void __iomem *davinci_intc_base;
int davinci_intc_type;

void davinci_get_mac_addr(struct memory_accessor *mem_acc, void *context)
{
	char *mac_addr = davinci_soc_info.emac_pdata->mac_addr;
	off_t offset = (off_t)context;

	/* Read MAC addr from EEPROM */
	if (mem_acc->read(mem_acc, mac_addr, offset, ETH_ALEN) == ETH_ALEN)
		pr_info("Read MAC addr from EEPROM: %pM\n", mac_addr);
}

static int __init davinci_init_id(struct davinci_soc_info *soc_info)
{
	int			i;
	struct davinci_id	*dip;
	u8			variant;
	u16			part_no;
	void __iomem		*base;

	base = ioremap(soc_info->jtag_id_reg, SZ_4K);
	if (!base) {
		pr_err("Unable to map JTAG ID register\n");
		return -ENOMEM;
	}

	soc_info->jtag_id = __raw_readl(base);
	iounmap(base);

	variant = (soc_info->jtag_id & 0xf0000000) >> 28;
	part_no = (soc_info->jtag_id & 0x0ffff000) >> 12;

	for (i = 0, dip = soc_info->ids; i < soc_info->ids_num;
			i++, dip++)
		/* Don't care about the manufacturer right now */
		if ((dip->part_no == part_no) && (dip->variant == variant)) {
			soc_info->cpu_id = dip->cpu_id;
			pr_info("DaVinci %s variant 0x%x\n", dip->name,
					dip->variant);
			return 0;
		}

	pr_err("Unknown DaVinci JTAG ID 0x%x\n", soc_info->jtag_id);
	return -EINVAL;
}

void __init davinci_common_init(struct davinci_soc_info *soc_info)
{
	int ret;

	if (!soc_info) {
		ret = -EINVAL;
		goto err;
	}

	memcpy(&davinci_soc_info, soc_info, sizeof(struct davinci_soc_info));

	if (davinci_soc_info.io_desc && (davinci_soc_info.io_desc_num > 0))
		iotable_init(davinci_soc_info.io_desc,
				davinci_soc_info.io_desc_num);

	/*
	 * Normally devicemaps_init() would flush caches and tlb after
	 * mdesc->map_io(), but we must also do it here because of the CPU
	 * revision check below.
	 */
	local_flush_tlb_all();
	flush_cache_all();

	if (!davinci_soc_info.reset)
		davinci_soc_info.reset = davinci_watchdog_reset;

	/*
	 * We want to check CPU revision early for cpu_is_xxxx() macros.
	 * IO space mapping must be initialized before we can do that.
	 */
	ret = davinci_init_id(&davinci_soc_info);
	if (ret < 0)
		goto err;

	if (davinci_soc_info.cpu_clks) {
		ret = davinci_clk_init(davinci_soc_info.cpu_clks);

		if (ret != 0)
			goto err;
	}

	return;

err:
	panic("davinci_common_init: SoC Initialization failed\n");
}
