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
 * arch/arm/mach-dove/include/mach/irqs.h
 *
 * IRQ definitions for Marvell Dove 88AP510 SoC
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H

/*
 * Dove Low Interrupt Controller
 */
#define IRQ_DOVE_BRIDGE		0
#define IRQ_DOVE_H2C		1
#define IRQ_DOVE_C2H		2
#define IRQ_DOVE_NAND		3
#define IRQ_DOVE_PDMA		4
#define IRQ_DOVE_SPI1		5
#define IRQ_DOVE_SPI0		6
#define IRQ_DOVE_UART_0		7
#define IRQ_DOVE_UART_1		8
#define IRQ_DOVE_UART_2		9
#define IRQ_DOVE_UART_3		10
#define IRQ_DOVE_I2C		11
#define IRQ_DOVE_GPIO_0_7	12
#define IRQ_DOVE_GPIO_8_15	13
#define IRQ_DOVE_GPIO_16_23	14
#define IRQ_DOVE_PCIE0_ERR	15
#define IRQ_DOVE_PCIE0		16
#define IRQ_DOVE_PCIE1_ERR	17
#define IRQ_DOVE_PCIE1		18
#define IRQ_DOVE_I2S0		19
#define IRQ_DOVE_I2S0_ERR	20
#define IRQ_DOVE_I2S1		21
#define IRQ_DOVE_I2S1_ERR	22
#define IRQ_DOVE_USB_ERR	23
#define IRQ_DOVE_USB0		24
#define IRQ_DOVE_USB1		25
#define IRQ_DOVE_GE00_RX	26
#define IRQ_DOVE_GE00_TX	27
#define IRQ_DOVE_GE00_MISC	28
#define IRQ_DOVE_GE00_SUM	29
#define IRQ_DOVE_GE00_ERR	30
#define IRQ_DOVE_CRYPTO		31

/*
 * Dove High Interrupt Controller
 */
#define IRQ_DOVE_AC97		32
#define IRQ_DOVE_PMU		33
#define IRQ_DOVE_CAM		34
#define IRQ_DOVE_SDIO0		35
#define IRQ_DOVE_SDIO1		36
#define IRQ_DOVE_SDIO0_WAKEUP	37
#define IRQ_DOVE_SDIO1_WAKEUP	38
#define IRQ_DOVE_XOR_00		39
#define IRQ_DOVE_XOR_01		40
#define IRQ_DOVE_XOR0_ERR	41
#define IRQ_DOVE_XOR_10		42
#define IRQ_DOVE_XOR_11		43
#define IRQ_DOVE_XOR1_ERR	44
#define IRQ_DOVE_LCD_DCON	45
#define IRQ_DOVE_LCD1		46
#define IRQ_DOVE_LCD0		47
#define IRQ_DOVE_GPU		48
#define IRQ_DOVE_PERFORM_MNTR	49
#define IRQ_DOVE_VPRO_DMA1	51
#define IRQ_DOVE_SSP_TIMER	54
#define IRQ_DOVE_SSP		55
#define IRQ_DOVE_MC_L2_ERR	56
#define IRQ_DOVE_CRYPTO_ERR	59
#define IRQ_DOVE_GPIO_24_31	60
#define IRQ_DOVE_HIGH_GPIO	61
#define IRQ_DOVE_SATA		62

/*
 * DOVE General Purpose Pins
 */
#define IRQ_DOVE_GPIO_START	64
#define NR_GPIO_IRQS		64

/*
 * PMU interrupts
 */
#define IRQ_DOVE_PMU_START	(IRQ_DOVE_GPIO_START + NR_GPIO_IRQS)
#define NR_PMU_IRQS		7
#define IRQ_DOVE_RTC		(IRQ_DOVE_PMU_START + 5)

#define NR_IRQS			(IRQ_DOVE_PMU_START + NR_PMU_IRQS)

/* Required for compatability with PXA AC97 driver.	*/
#define IRQ_AC97		IRQ_DOVE_AC97
/* Required for compatability with PXA DMA driver.	*/
#define IRQ_DMA			IRQ_DOVE_PDMA
/* Required for compatability with PXA NAND driver 	*/
#define IRQ_NAND		IRQ_DOVE_NAND
#endif
