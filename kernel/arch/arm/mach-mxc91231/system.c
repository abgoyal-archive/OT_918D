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
 * Copyright 2009 Dmitriy Taychenachev <dimichxp@gmail.com>
 *
 * This file is released under the GPLv2 or later.
 */

#include <linux/delay.h>
#include <linux/io.h>

#include <asm/proc-fns.h>
#include <mach/hardware.h>

#include "crm_regs.h"

#define WDOG_WCR		MXC91231_IO_ADDRESS(MXC91231_WDOG1_BASE_ADDR)
#define WDOG_WCR_OUT_ENABLE	(1 << 6)
#define WDOG_WCR_ASSERT		(1 << 5)

void mxc91231_power_off(void)
{
	u16 wcr;

	wcr = __raw_readw(WDOG_WCR);
	wcr |= WDOG_WCR_OUT_ENABLE;
	wcr &= ~WDOG_WCR_ASSERT;
	__raw_writew(wcr, WDOG_WCR);
}

void mxc91231_arch_reset(char mode, const char *cmd)
{
	u32 amcr;

	/* Reset the AP using CRM */
	amcr = __raw_readl(MXC_CRMAP_AMCR);
	amcr &= ~MXC_CRMAP_AMCR_SW_AP;
	__raw_writel(amcr, MXC_CRMAP_AMCR);

	mdelay(10);
	cpu_reset(0);
}

void mxc91231_prepare_idle(void)
{
	u32 crm_ctl;

	/* Go to WAIT mode after WFI */
	crm_ctl = __raw_readl(MXC_DSM_CRM_CONTROL);
	crm_ctl &= ~(MXC_DSM_CRM_CTRL_LPMD0 | MXC_DSM_CRM_CTRL_LPMD1);
	crm_ctl |=  MXC_DSM_CRM_CTRL_LPMD_WAIT_MODE;
	__raw_writel(crm_ctl, MXC_DSM_CRM_CONTROL);
}
