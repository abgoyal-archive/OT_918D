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
 * Blackfin cache control code (simpler control-style functions)
 *
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/init.h>
#include <asm/blackfin.h>
#include <asm/cplbinit.h>

/* Invalidate the Entire Data cache by
 * clearing DMC[1:0] bits
 */
void blackfin_invalidate_entire_dcache(void)
{
	u32 dmem = bfin_read_DMEM_CONTROL();
	bfin_write_DMEM_CONTROL(dmem & ~0xc);
	SSYNC();
	bfin_write_DMEM_CONTROL(dmem);
	SSYNC();
}

/* Invalidate the Entire Instruction cache by
 * clearing IMC bit
 */
void blackfin_invalidate_entire_icache(void)
{
	u32 imem = bfin_read_IMEM_CONTROL();
	bfin_write_IMEM_CONTROL(imem & ~0x4);
	SSYNC();
	bfin_write_IMEM_CONTROL(imem);
	SSYNC();
}

#if defined(CONFIG_BFIN_ICACHE) || defined(CONFIG_BFIN_DCACHE)

static void
bfin_cache_init(struct cplb_entry *cplb_tbl, unsigned long cplb_addr,
                unsigned long cplb_data, unsigned long mem_control,
                unsigned long mem_mask)
{
	int i;

	for (i = 0; i < MAX_CPLBS; i++) {
		bfin_write32(cplb_addr + i * 4, cplb_tbl[i].addr);
		bfin_write32(cplb_data + i * 4, cplb_tbl[i].data);
	}

	_enable_cplb(mem_control, mem_mask);
}

#ifdef CONFIG_BFIN_ICACHE
void __cpuinit bfin_icache_init(struct cplb_entry *icplb_tbl)
{
	bfin_cache_init(icplb_tbl, ICPLB_ADDR0, ICPLB_DATA0, IMEM_CONTROL,
		(IMC | ENICPLB));
}
#endif

#ifdef CONFIG_BFIN_DCACHE
void __cpuinit bfin_dcache_init(struct cplb_entry *dcplb_tbl)
{
	/*
	 *  Anomaly notes:
	 *  05000287 - We implement workaround #2 - Change the DMEM_CONTROL
	 *  register, so that the port preferences for DAG0 and DAG1 are set
	 *  to port B
	 */
	bfin_cache_init(dcplb_tbl, DCPLB_ADDR0, DCPLB_DATA0, DMEM_CONTROL,
		(DMEM_CNTR | PORT_PREF0 | (ANOMALY_05000287 ? PORT_PREF1 : 0)));
}
#endif

#endif
