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
 * linux/arch/arm/mach-omap2/mcbsp.c
 *
 * Copyright (C) 2008 Instituto Nokia de Tecnologia
 * Contact: Eduardo Valentin <eduardo.valentin@indt.org.br>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Multichannel mode not supported.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include <mach/irqs.h>
#include <plat/dma.h>
#include <plat/mux.h>
#include <plat/cpu.h>
#include <plat/mcbsp.h>

static void omap2_mcbsp2_mux_setup(void)
{
	omap_cfg_reg(Y15_24XX_MCBSP2_CLKX);
	omap_cfg_reg(R14_24XX_MCBSP2_FSX);
	omap_cfg_reg(W15_24XX_MCBSP2_DR);
	omap_cfg_reg(V15_24XX_MCBSP2_DX);
	omap_cfg_reg(V14_24XX_GPIO117);
	/*
	 * TODO: Need to add MUX settings for OMAP 2430 SDP
	 */
}

static void omap2_mcbsp_request(unsigned int id)
{
	if (cpu_is_omap2420() && (id == OMAP_MCBSP2))
		omap2_mcbsp2_mux_setup();
}

static struct omap_mcbsp_ops omap2_mcbsp_ops = {
	.request	= omap2_mcbsp_request,
};

#ifdef CONFIG_ARCH_OMAP2420
static struct omap_mcbsp_platform_data omap2420_mcbsp_pdata[] = {
	{
		.phys_base	= OMAP24XX_MCBSP1_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP1_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP1_TX,
		.rx_irq		= INT_24XX_MCBSP1_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP1_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
	},
	{
		.phys_base	= OMAP24XX_MCBSP2_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP2_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP2_TX,
		.rx_irq		= INT_24XX_MCBSP2_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP2_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
	},
};
#define OMAP2420_MCBSP_PDATA_SZ		ARRAY_SIZE(omap2420_mcbsp_pdata)
#define OMAP2420_MCBSP_REG_NUM		(OMAP_MCBSP_REG_RCCR / sizeof(u32) + 1)
#else
#define omap2420_mcbsp_pdata		NULL
#define OMAP2420_MCBSP_PDATA_SZ		0
#define OMAP2420_MCBSP_REG_NUM		0
#endif

#ifdef CONFIG_ARCH_OMAP2430
static struct omap_mcbsp_platform_data omap2430_mcbsp_pdata[] = {
	{
		.phys_base	= OMAP24XX_MCBSP1_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP1_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP1_TX,
		.rx_irq		= INT_24XX_MCBSP1_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP1_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
	},
	{
		.phys_base	= OMAP24XX_MCBSP2_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP2_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP2_TX,
		.rx_irq		= INT_24XX_MCBSP2_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP2_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
	},
	{
		.phys_base	= OMAP2430_MCBSP3_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP3_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP3_TX,
		.rx_irq		= INT_24XX_MCBSP3_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP3_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
	},
	{
		.phys_base	= OMAP2430_MCBSP4_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP4_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP4_TX,
		.rx_irq		= INT_24XX_MCBSP4_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP4_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
	},
	{
		.phys_base	= OMAP2430_MCBSP5_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP5_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP5_TX,
		.rx_irq		= INT_24XX_MCBSP5_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP5_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
	},
};
#define OMAP2430_MCBSP_PDATA_SZ		ARRAY_SIZE(omap2430_mcbsp_pdata)
#define OMAP2430_MCBSP_REG_NUM		(OMAP_MCBSP_REG_RCCR / sizeof(u32) + 1)
#else
#define omap2430_mcbsp_pdata		NULL
#define OMAP2430_MCBSP_PDATA_SZ		0
#define OMAP2430_MCBSP_REG_NUM		0
#endif

#ifdef CONFIG_ARCH_OMAP3
static struct omap_mcbsp_platform_data omap34xx_mcbsp_pdata[] = {
	{
		.phys_base	= OMAP34XX_MCBSP1_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP1_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP1_TX,
		.rx_irq		= INT_24XX_MCBSP1_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP1_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
		.buffer_size	= 0x6F,
	},
	{
		.phys_base	= OMAP34XX_MCBSP2_BASE,
		.phys_base_st	= OMAP34XX_MCBSP2_ST_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP2_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP2_TX,
		.rx_irq		= INT_24XX_MCBSP2_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP2_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
		.buffer_size	= 0x3FF,
	},
	{
		.phys_base	= OMAP34XX_MCBSP3_BASE,
		.phys_base_st	= OMAP34XX_MCBSP3_ST_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP3_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP3_TX,
		.rx_irq		= INT_24XX_MCBSP3_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP3_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
		.buffer_size	= 0x6F,
	},
	{
		.phys_base	= OMAP34XX_MCBSP4_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP4_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP4_TX,
		.rx_irq		= INT_24XX_MCBSP4_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP4_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
		.buffer_size	= 0x6F,
	},
	{
		.phys_base	= OMAP34XX_MCBSP5_BASE,
		.dma_rx_sync	= OMAP24XX_DMA_MCBSP5_RX,
		.dma_tx_sync	= OMAP24XX_DMA_MCBSP5_TX,
		.rx_irq		= INT_24XX_MCBSP5_IRQ_RX,
		.tx_irq		= INT_24XX_MCBSP5_IRQ_TX,
		.ops		= &omap2_mcbsp_ops,
		.buffer_size	= 0x6F,
	},
};
#define OMAP34XX_MCBSP_PDATA_SZ		ARRAY_SIZE(omap34xx_mcbsp_pdata)
#define OMAP34XX_MCBSP_REG_NUM		(OMAP_MCBSP_REG_RCCR / sizeof(u32) + 1)
#else
#define omap34xx_mcbsp_pdata		NULL
#define OMAP34XX_MCBSP_PDATA_SZ		0
#define OMAP34XX_MCBSP_REG_NUM		0
#endif

static struct omap_mcbsp_platform_data omap44xx_mcbsp_pdata[] = {
	{
		.phys_base      = OMAP44XX_MCBSP1_BASE,
		.dma_rx_sync    = OMAP44XX_DMA_MCBSP1_RX,
		.dma_tx_sync    = OMAP44XX_DMA_MCBSP1_TX,
		.tx_irq         = OMAP44XX_IRQ_MCBSP1,
		.ops            = &omap2_mcbsp_ops,
	},
	{
		.phys_base      = OMAP44XX_MCBSP2_BASE,
		.dma_rx_sync    = OMAP44XX_DMA_MCBSP2_RX,
		.dma_tx_sync    = OMAP44XX_DMA_MCBSP2_TX,
		.tx_irq         = OMAP44XX_IRQ_MCBSP2,
		.ops            = &omap2_mcbsp_ops,
	},
	{
		.phys_base      = OMAP44XX_MCBSP3_BASE,
		.dma_rx_sync    = OMAP44XX_DMA_MCBSP3_RX,
		.dma_tx_sync    = OMAP44XX_DMA_MCBSP3_TX,
		.tx_irq         = OMAP44XX_IRQ_MCBSP3,
		.ops            = &omap2_mcbsp_ops,
	},
	{
		.phys_base      = OMAP44XX_MCBSP4_BASE,
		.dma_rx_sync    = OMAP44XX_DMA_MCBSP4_RX,
		.dma_tx_sync    = OMAP44XX_DMA_MCBSP4_TX,
		.tx_irq         = OMAP44XX_IRQ_MCBSP4,
		.ops            = &omap2_mcbsp_ops,
	},
};
#define OMAP44XX_MCBSP_PDATA_SZ		ARRAY_SIZE(omap44xx_mcbsp_pdata)
#define OMAP44XX_MCBSP_REG_NUM		(OMAP_MCBSP_REG_RCCR / sizeof(u32) + 1)

static int __init omap2_mcbsp_init(void)
{
	if (cpu_is_omap2420()) {
		omap_mcbsp_count = OMAP2420_MCBSP_PDATA_SZ;
		omap_mcbsp_cache_size = OMAP2420_MCBSP_REG_NUM * sizeof(u16);
	} else if (cpu_is_omap2430()) {
		omap_mcbsp_count = OMAP2430_MCBSP_PDATA_SZ;
		omap_mcbsp_cache_size = OMAP2430_MCBSP_REG_NUM * sizeof(u32);
	} else if (cpu_is_omap34xx()) {
		omap_mcbsp_count = OMAP34XX_MCBSP_PDATA_SZ;
		omap_mcbsp_cache_size = OMAP34XX_MCBSP_REG_NUM * sizeof(u32);
	} else if (cpu_is_omap44xx()) {
		omap_mcbsp_count = OMAP44XX_MCBSP_PDATA_SZ;
		omap_mcbsp_cache_size = OMAP44XX_MCBSP_REG_NUM * sizeof(u32);
	}

	mcbsp_ptr = kzalloc(omap_mcbsp_count * sizeof(struct omap_mcbsp *),
								GFP_KERNEL);
	if (!mcbsp_ptr)
		return -ENOMEM;

	if (cpu_is_omap2420())
		omap_mcbsp_register_board_cfg(omap2420_mcbsp_pdata,
						OMAP2420_MCBSP_PDATA_SZ);
	if (cpu_is_omap2430())
		omap_mcbsp_register_board_cfg(omap2430_mcbsp_pdata,
						OMAP2430_MCBSP_PDATA_SZ);
	if (cpu_is_omap34xx())
		omap_mcbsp_register_board_cfg(omap34xx_mcbsp_pdata,
						OMAP34XX_MCBSP_PDATA_SZ);
	if (cpu_is_omap44xx())
		omap_mcbsp_register_board_cfg(omap44xx_mcbsp_pdata,
						OMAP44XX_MCBSP_PDATA_SZ);

	return omap_mcbsp_init();
}
arch_initcall(omap2_mcbsp_init);
