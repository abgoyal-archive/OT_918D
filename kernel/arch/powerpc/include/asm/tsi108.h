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
 * common routine and memory layout for Tundra TSI108(Grendel) host bridge
 * memory controller.
 *
 * Author: Jacob Pan (jacob.pan@freescale.com)
 *	   Alex Bounine (alexandreb@tundra.com)
 *
 * Copyright 2004-2006 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef __PPC_KERNEL_TSI108_H
#define __PPC_KERNEL_TSI108_H

#include <asm/pci-bridge.h>

/* Size of entire register space */
#define TSI108_REG_SIZE		(0x10000)

/* Sizes of register spaces for individual blocks */
#define TSI108_HLP_SIZE		0x1000
#define TSI108_PCI_SIZE		0x1000
#define TSI108_CLK_SIZE		0x1000
#define TSI108_PB_SIZE		0x1000
#define TSI108_SD_SIZE		0x1000
#define TSI108_DMA_SIZE		0x1000
#define TSI108_ETH_SIZE		0x1000
#define TSI108_I2C_SIZE		0x400
#define TSI108_MPIC_SIZE	0x400
#define TSI108_UART0_SIZE	0x200
#define TSI108_GPIO_SIZE	0x200
#define TSI108_UART1_SIZE	0x200

/* Offsets within Tsi108(A) CSR space for individual blocks */
#define TSI108_HLP_OFFSET	0x0000
#define TSI108_PCI_OFFSET	0x1000
#define TSI108_CLK_OFFSET	0x2000
#define TSI108_PB_OFFSET	0x3000
#define TSI108_SD_OFFSET	0x4000
#define TSI108_DMA_OFFSET	0x5000
#define TSI108_ETH_OFFSET	0x6000
#define TSI108_I2C_OFFSET	0x7000
#define TSI108_MPIC_OFFSET	0x7400
#define TSI108_UART0_OFFSET	0x7800
#define TSI108_GPIO_OFFSET	0x7A00
#define TSI108_UART1_OFFSET	0x7C00

/* Tsi108 registers used by common code components */
#define TSI108_PCI_CSR		(0x004)
#define TSI108_PCI_IRP_CFG_CTL	(0x180)
#define TSI108_PCI_IRP_STAT	(0x184)
#define TSI108_PCI_IRP_ENABLE	(0x188)
#define TSI108_PCI_IRP_INTAD	(0x18C)

#define TSI108_PCI_IRP_STAT_P_INT	(0x00400000)
#define TSI108_PCI_IRP_ENABLE_P_INT	(0x00400000)

#define TSI108_CG_PWRUP_STATUS	(0x234)

#define TSI108_PB_ISR		(0x00C)
#define TSI108_PB_ERRCS		(0x404)
#define TSI108_PB_AERR		(0x408)

#define TSI108_PB_ERRCS_ES		(1 << 1)
#define TSI108_PB_ISR_PBS_RD_ERR	(1 << 8)

#define TSI108_PCI_CFG_SIZE		(0x01000000)

/*
 * PHY Configuration Options
 *
 * Specify "bcm54xx" in the compatible property of your device tree phy
 * nodes if your board uses the Broadcom PHYs
 */
#define TSI108_PHY_MV88E	0	/* Marvel 88Exxxx PHY */
#define TSI108_PHY_BCM54XX	1	/* Broardcom BCM54xx PHY */

/* Global variables */

extern u32 tsi108_pci_cfg_base;
/* Exported functions */

extern int tsi108_bridge_init(struct pci_controller *hose, uint phys_csr_base);
extern unsigned long tsi108_get_mem_size(void);
extern unsigned long tsi108_get_cpu_clk(void);
extern unsigned long tsi108_get_sdc_clk(void);
extern int tsi108_direct_write_config(struct pci_bus *bus, unsigned int devfn,
				      int offset, int len, u32 val);
extern int tsi108_direct_read_config(struct pci_bus *bus, unsigned int devfn,
				     int offset, int len, u32 * val);
extern void tsi108_clear_pci_error(u32 pci_cfg_base);

extern phys_addr_t get_csrbase(void);

typedef struct {
	u32 regs;		/* hw registers base address */
	u32 phyregs;		/* phy registers base address */
	u16 phy;		/* phy address */
	u16 irq_num;		/* irq number */
	u8 mac_addr[6];		/* phy mac address */
	u16 phy_type;	/* type of phy on board */
} hw_info;

extern u32 get_vir_csrbase(void);
extern u32 tsi108_csr_vir_base;

static inline u32 tsi108_read_reg(u32 reg_offset)
{
	return in_be32((volatile u32 *)(tsi108_csr_vir_base + reg_offset));
}

static inline void tsi108_write_reg(u32 reg_offset, u32 val)
{
	out_be32((volatile u32 *)(tsi108_csr_vir_base + reg_offset), val);
}

#endif				/* __PPC_KERNEL_TSI108_H */