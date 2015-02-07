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
 * read/write operation to the PCI config space of CS5536
 *
 * Copyright (C) 2007 Lemote, Inc.
 * Author : jlliu, liujl@lemote.com
 *
 * Copyright (C) 2009 Lemote, Inc.
 * Author: Wu Zhangjin, wuzhangjin@gmail.com
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 *	the Virtual Support Module(VSM) for virtulizing the PCI
 *	configure space are defined in cs5536_modulename.c respectively,
 *
 *	after this virtulizing, user can access the PCI configure space
 *	directly as a normal multi-function PCI device which follows
 *	the PCI-2.2 spec.
 */

#include <linux/types.h>
#include <cs5536/cs5536_vsm.h>

enum {
	CS5536_FUNC_START = -1,
	CS5536_ISA_FUNC,
	reserved_func,
	CS5536_IDE_FUNC,
	CS5536_ACC_FUNC,
	CS5536_OHCI_FUNC,
	CS5536_EHCI_FUNC,
	CS5536_FUNC_END,
};

static const cs5536_pci_vsm_write vsm_conf_write[] = {
	[CS5536_ISA_FUNC]	pci_isa_write_reg,
	[reserved_func]		NULL,
	[CS5536_IDE_FUNC]	pci_ide_write_reg,
	[CS5536_ACC_FUNC]	pci_acc_write_reg,
	[CS5536_OHCI_FUNC]	pci_ohci_write_reg,
	[CS5536_EHCI_FUNC]	pci_ehci_write_reg,
};

static const cs5536_pci_vsm_read vsm_conf_read[] = {
	[CS5536_ISA_FUNC]	pci_isa_read_reg,
	[reserved_func]		NULL,
	[CS5536_IDE_FUNC]	pci_ide_read_reg,
	[CS5536_ACC_FUNC]	pci_acc_read_reg,
	[CS5536_OHCI_FUNC]	pci_ohci_read_reg,
	[CS5536_EHCI_FUNC]	pci_ehci_read_reg,
};

/*
 * write to PCI config space and transfer it to MSR write.
 */
void cs5536_pci_conf_write4(int function, int reg, u32 value)
{
	if ((function <= CS5536_FUNC_START) || (function >= CS5536_FUNC_END))
		return;
	if ((reg < 0) || (reg > 0x100) || ((reg & 0x03) != 0))
		return;

	if (vsm_conf_write[function] != NULL)
		vsm_conf_write[function](reg, value);
}

/*
 * read PCI config space and transfer it to MSR access.
 */
u32 cs5536_pci_conf_read4(int function, int reg)
{
	u32 data = 0;

	if ((function <= CS5536_FUNC_START) || (function >= CS5536_FUNC_END))
		return 0;
	if ((reg < 0) || ((reg & 0x03) != 0))
		return 0;
	if (reg > 0x100)
		return 0xffffffff;

	if (vsm_conf_read[function] != NULL)
		data = vsm_conf_read[function](reg);

	return data;
}
