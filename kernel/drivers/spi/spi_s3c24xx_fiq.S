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

/* linux/drivers/spi/spi_s3c24xx_fiq.S
 *
 * Copyright 2009 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C24XX SPI - FIQ pseudo-DMA transfer code
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/linkage.h>
#include <asm/assembler.h>

#include <mach/map.h>
#include <mach/regs-irq.h>
#include <plat/regs-spi.h>

#include "spi_s3c24xx_fiq.h"

	.text

	@ entry to these routines is as follows, with the register names
	@ defined in fiq.h so that they can be shared with the C files which
	@ setup the calling registers.
	@
	@ fiq_rirq	The base of the IRQ registers to find S3C2410_SRCPND
	@ fiq_rtmp	Temporary register to hold tx/rx data
	@ fiq_rspi	The base of the SPI register block
	@ fiq_rtx	The tx buffer pointer
	@ fiq_rrx	The rx buffer pointer
	@ fiq_rcount	The number of bytes to move

	@ each entry starts with a word entry of how long it is
	@ and an offset to the irq acknowledgment word

ENTRY(s3c24xx_spi_fiq_rx)
s3c24xx_spi_fix_rx:
	.word	fiq_rx_end - fiq_rx_start
	.word	fiq_rx_irq_ack - fiq_rx_start
fiq_rx_start:
	ldr	fiq_rtmp, fiq_rx_irq_ack
	str	fiq_rtmp, [ fiq_rirq, # S3C2410_SRCPND - S3C24XX_VA_IRQ ]

	ldrb	fiq_rtmp, [ fiq_rspi, #  S3C2410_SPRDAT ]
	strb	fiq_rtmp, [ fiq_rrx ], #1

	mov	fiq_rtmp, #0xff
	strb	fiq_rtmp, [ fiq_rspi, # S3C2410_SPTDAT ]

	subs	fiq_rcount, fiq_rcount, #1
	subnes	pc, lr, #4		@@ return, still have work to do

	@@ set IRQ controller so that next op will trigger IRQ
	mov	fiq_rtmp, #0
	str	fiq_rtmp, [ fiq_rirq, # S3C2410_INTMOD  - S3C24XX_VA_IRQ ]
	subs	pc, lr, #4

fiq_rx_irq_ack:
	.word	0
fiq_rx_end:

ENTRY(s3c24xx_spi_fiq_txrx)
s3c24xx_spi_fiq_txrx:
	.word	fiq_txrx_end - fiq_txrx_start
	.word	fiq_txrx_irq_ack - fiq_txrx_start
fiq_txrx_start:

	ldrb	fiq_rtmp, [ fiq_rspi, #  S3C2410_SPRDAT ]
	strb	fiq_rtmp, [ fiq_rrx ], #1

	ldr	fiq_rtmp, fiq_txrx_irq_ack
	str	fiq_rtmp, [ fiq_rirq, # S3C2410_SRCPND - S3C24XX_VA_IRQ ]

	ldrb	fiq_rtmp, [ fiq_rtx ], #1
	strb	fiq_rtmp, [ fiq_rspi, # S3C2410_SPTDAT ]

	subs	fiq_rcount, fiq_rcount, #1
	subnes	pc, lr, #4		@@ return, still have work to do

	mov	fiq_rtmp, #0
	str	fiq_rtmp, [ fiq_rirq, # S3C2410_INTMOD  - S3C24XX_VA_IRQ ]
	subs	pc, lr, #4

fiq_txrx_irq_ack:
	.word	0

fiq_txrx_end:

ENTRY(s3c24xx_spi_fiq_tx)
s3c24xx_spi_fix_tx:
	.word	fiq_tx_end - fiq_tx_start
	.word	fiq_tx_irq_ack - fiq_tx_start
fiq_tx_start:
	ldrb	fiq_rtmp, [ fiq_rspi, #  S3C2410_SPRDAT ]

	ldr	fiq_rtmp, fiq_tx_irq_ack
	str	fiq_rtmp, [ fiq_rirq, # S3C2410_SRCPND - S3C24XX_VA_IRQ ]

	ldrb	fiq_rtmp, [ fiq_rtx ], #1
	strb	fiq_rtmp, [ fiq_rspi, # S3C2410_SPTDAT ]

	subs	fiq_rcount, fiq_rcount, #1
	subnes	pc, lr, #4		@@ return, still have work to do

	mov	fiq_rtmp, #0
	str	fiq_rtmp, [ fiq_rirq, # S3C2410_INTMOD  - S3C24XX_VA_IRQ ]
	subs	pc, lr, #4

fiq_tx_irq_ack:
	.word	0

fiq_tx_end:

	.end
