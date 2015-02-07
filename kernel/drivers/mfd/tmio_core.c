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
 * Copyright(c) 2009 Ian Molton <spyro@f2s.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/mfd/tmio.h>

int tmio_core_mmc_enable(void __iomem *cnf, int shift, unsigned long base)
{
	/* Enable the MMC/SD Control registers */
	sd_config_write16(cnf, shift, CNF_CMD, SDCREN);
	sd_config_write32(cnf, shift, CNF_CTL_BASE, base & 0xfffe);

	/* Disable SD power during suspend */
	sd_config_write8(cnf, shift, CNF_PWR_CTL_3, 0x01);

	/* The below is required but why? FIXME */
	sd_config_write8(cnf, shift, CNF_STOP_CLK_CTL, 0x1f);

	/* Power down SD bus */
	sd_config_write8(cnf, shift, CNF_PWR_CTL_2, 0x00);

	return 0;
}
EXPORT_SYMBOL(tmio_core_mmc_enable);

int tmio_core_mmc_resume(void __iomem *cnf, int shift, unsigned long base)
{

	/* Enable the MMC/SD Control registers */
	sd_config_write16(cnf, shift, CNF_CMD, SDCREN);
	sd_config_write32(cnf, shift, CNF_CTL_BASE, base & 0xfffe);

	return 0;
}
EXPORT_SYMBOL(tmio_core_mmc_resume);

void tmio_core_mmc_pwr(void __iomem *cnf, int shift, int state)
{
	sd_config_write8(cnf, shift, CNF_PWR_CTL_2, state ? 0x02 : 0x00);
}
EXPORT_SYMBOL(tmio_core_mmc_pwr);

void tmio_core_mmc_clk_div(void __iomem *cnf, int shift, int state)
{
	sd_config_write8(cnf, shift, CNF_SD_CLK_MODE, state ? 1 : 0);
}
EXPORT_SYMBOL(tmio_core_mmc_clk_div);

