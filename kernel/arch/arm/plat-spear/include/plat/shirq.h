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
 * arch/arm/plat-spear/include/plat/shirq.h
 *
 * SPEAr platform shared irq layer header file
 *
 * Copyright (C) 2009 ST Microelectronics
 * Viresh Kumar<viresh.kumar@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __PLAT_SHIRQ_H
#define __PLAT_SHIRQ_H

#include <linux/irq.h>
#include <linux/types.h>

/*
 * struct shirq_dev_config: shared irq device configuration
 *
 * virq: virtual irq number of device
 * enb_mask: enable mask of device
 * status_mask: status mask of device
 * clear_mask: clear mask of device
 */
struct shirq_dev_config {
	u32 virq;
	u32 enb_mask;
	u32 status_mask;
	u32 clear_mask;
};

/*
 * struct shirq_regs: shared irq register configuration
 *
 * base: base address of shared irq register
 * enb_reg: enable register offset
 * reset_to_enb: val 1 indicates, we need to clear bit for enabling interrupt
 * status_reg: status register offset
 * status_reg_mask: status register valid mask
 * clear_reg: clear register offset
 * reset_to_clear: val 1 indicates, we need to clear bit for clearing interrupt
 */
struct shirq_regs {
	void __iomem *base;
	u32 enb_reg;
	u32 reset_to_enb;
	u32 status_reg;
	u32 status_reg_mask;
	u32 clear_reg;
	u32 reset_to_clear;
};

/*
 * struct spear_shirq: shared irq structure
 *
 * irq: hardware irq number
 * dev_config: array of device config structures which are using "irq" line
 * dev_count: size of dev_config array
 * regs: register configuration for shared irq block
 */
struct spear_shirq {
	u32 irq;
	struct shirq_dev_config *dev_config;
	u32 dev_count;
	struct shirq_regs regs;
};

int spear_shirq_register(struct spear_shirq *shirq);

#endif /* __PLAT_SHIRQ_H */
