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
 * simple DMA Implementation for Blackfin
 *
 * Copyright 2007-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/module.h>

#include <asm/blackfin.h>
#include <asm/dma.h>

struct dma_register *dma_io_base_addr[MAX_DMA_CHANNELS] = {
	(struct dma_register *) DMA0_NEXT_DESC_PTR,
	(struct dma_register *) DMA1_NEXT_DESC_PTR,
	(struct dma_register *) DMA2_NEXT_DESC_PTR,
	(struct dma_register *) DMA3_NEXT_DESC_PTR,
	(struct dma_register *) DMA4_NEXT_DESC_PTR,
	(struct dma_register *) DMA5_NEXT_DESC_PTR,
	(struct dma_register *) DMA6_NEXT_DESC_PTR,
	(struct dma_register *) DMA7_NEXT_DESC_PTR,
	(struct dma_register *) MDMA_D0_NEXT_DESC_PTR,
	(struct dma_register *) MDMA_S0_NEXT_DESC_PTR,
	(struct dma_register *) MDMA_D1_NEXT_DESC_PTR,
	(struct dma_register *) MDMA_S1_NEXT_DESC_PTR,
};
EXPORT_SYMBOL(dma_io_base_addr);

int channel2irq(unsigned int channel)
{
	int ret_irq = -1;

	switch (channel) {
	case CH_PPI:
		ret_irq = IRQ_PPI;
		break;

	case CH_SPORT0_RX:
		ret_irq = IRQ_SPORT0_RX;
		break;

	case CH_SPORT0_TX:
		ret_irq = IRQ_SPORT0_TX;
		break;

	case CH_SPORT1_RX:
		ret_irq = IRQ_SPORT1_RX;
		break;

	case CH_SPORT1_TX:
		ret_irq = IRQ_SPORT1_TX;
		break;

	case CH_SPI:
		ret_irq = IRQ_SPI;
		break;

	case CH_UART0_RX:
		ret_irq = IRQ_UART0_RX;
		break;

	case CH_UART0_TX:
		ret_irq = IRQ_UART0_TX;
		break;

	case CH_MEM_STREAM0_SRC:
	case CH_MEM_STREAM0_DEST:
		ret_irq = IRQ_MEM_DMA0;
		break;

	case CH_MEM_STREAM1_SRC:
	case CH_MEM_STREAM1_DEST:
		ret_irq = IRQ_MEM_DMA1;
		break;
	}
	return ret_irq;
}