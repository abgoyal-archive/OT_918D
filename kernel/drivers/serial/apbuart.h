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

#ifndef __GRLIB_APBUART_H__
#define __GRLIB_APBUART_H__

#include <asm/io.h>

#define UART_NR		8
static int grlib_apbuart_port_nr;

struct grlib_apbuart_regs_map {
	u32 data;
	u32 status;
	u32 ctrl;
	u32 scaler;
};

struct amba_prom_registers {
	unsigned int phys_addr;
	unsigned int reg_size;
};

/*
 *  The following defines the bits in the APBUART Status Registers.
 */
#define UART_STATUS_DR   0x00000001	/* Data Ready */
#define UART_STATUS_TSE  0x00000002	/* TX Send Register Empty */
#define UART_STATUS_THE  0x00000004	/* TX Hold Register Empty */
#define UART_STATUS_BR   0x00000008	/* Break Error */
#define UART_STATUS_OE   0x00000010	/* RX Overrun Error */
#define UART_STATUS_PE   0x00000020	/* RX Parity Error */
#define UART_STATUS_FE   0x00000040	/* RX Framing Error */
#define UART_STATUS_ERR  0x00000078	/* Error Mask */

/*
 *  The following defines the bits in the APBUART Ctrl Registers.
 */
#define UART_CTRL_RE     0x00000001	/* Receiver enable */
#define UART_CTRL_TE     0x00000002	/* Transmitter enable */
#define UART_CTRL_RI     0x00000004	/* Receiver interrupt enable */
#define UART_CTRL_TI     0x00000008	/* Transmitter irq */
#define UART_CTRL_PS     0x00000010	/* Parity select */
#define UART_CTRL_PE     0x00000020	/* Parity enable */
#define UART_CTRL_FL     0x00000040	/* Flow control enable */
#define UART_CTRL_LB     0x00000080	/* Loopback enable */

#define APBBASE(port) ((struct grlib_apbuart_regs_map *)((port)->membase))

#define APBBASE_DATA_P(port)	(&(APBBASE(port)->data))
#define APBBASE_STATUS_P(port)	(&(APBBASE(port)->status))
#define APBBASE_CTRL_P(port)	(&(APBBASE(port)->ctrl))
#define APBBASE_SCALAR_P(port)	(&(APBBASE(port)->scaler))

#define UART_GET_CHAR(port)	(__raw_readl(APBBASE_DATA_P(port)))
#define UART_PUT_CHAR(port, v)	(__raw_writel(v, APBBASE_DATA_P(port)))
#define UART_GET_STATUS(port)	(__raw_readl(APBBASE_STATUS_P(port)))
#define UART_PUT_STATUS(port, v)(__raw_writel(v, APBBASE_STATUS_P(port)))
#define UART_GET_CTRL(port)	(__raw_readl(APBBASE_CTRL_P(port)))
#define UART_PUT_CTRL(port, v)	(__raw_writel(v, APBBASE_CTRL_P(port)))
#define UART_GET_SCAL(port)	(__raw_readl(APBBASE_SCALAR_P(port)))
#define UART_PUT_SCAL(port, v)	(__raw_writel(v, APBBASE_SCALAR_P(port)))

#define UART_RX_DATA(s)		(((s) & UART_STATUS_DR) != 0)
#define UART_TX_READY(s)	(((s) & UART_STATUS_THE) != 0)

#endif /* __GRLIB_APBUART_H__ */
