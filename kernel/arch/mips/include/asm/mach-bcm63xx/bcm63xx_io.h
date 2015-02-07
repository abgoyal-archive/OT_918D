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

#ifndef BCM63XX_IO_H_
#define BCM63XX_IO_H_

#include "bcm63xx_cpu.h"

/*
 * Physical memory map, RAM is mapped at 0x0.
 *
 * Note that size MUST be a power of two.
 */
#define BCM_PCMCIA_COMMON_BASE_PA	(0x20000000)
#define BCM_PCMCIA_COMMON_SIZE		(16 * 1024 * 1024)
#define BCM_PCMCIA_COMMON_END_PA	(BCM_PCMCIA_COMMON_BASE_PA +	\
					 BCM_PCMCIA_COMMON_SIZE - 1)

#define BCM_PCMCIA_ATTR_BASE_PA		(0x21000000)
#define BCM_PCMCIA_ATTR_SIZE		(16 * 1024 * 1024)
#define BCM_PCMCIA_ATTR_END_PA		(BCM_PCMCIA_ATTR_BASE_PA +	\
					 BCM_PCMCIA_ATTR_SIZE - 1)

#define BCM_PCMCIA_IO_BASE_PA		(0x22000000)
#define BCM_PCMCIA_IO_SIZE		(64 * 1024)
#define BCM_PCMCIA_IO_END_PA		(BCM_PCMCIA_IO_BASE_PA +	\
					BCM_PCMCIA_IO_SIZE - 1)

#define BCM_PCI_MEM_BASE_PA		(0x30000000)
#define BCM_PCI_MEM_SIZE		(128 * 1024 * 1024)
#define BCM_PCI_MEM_END_PA		(BCM_PCI_MEM_BASE_PA +		\
					BCM_PCI_MEM_SIZE - 1)

#define BCM_PCI_IO_BASE_PA		(0x08000000)
#define BCM_PCI_IO_SIZE			(64 * 1024)
#define BCM_PCI_IO_END_PA		(BCM_PCI_IO_BASE_PA +		\
					BCM_PCI_IO_SIZE - 1)
#define BCM_PCI_IO_HALF_PA		(BCM_PCI_IO_BASE_PA +		\
					(BCM_PCI_IO_SIZE / 2) - 1)

#define BCM_CB_MEM_BASE_PA		(0x38000000)
#define BCM_CB_MEM_SIZE			(128 * 1024 * 1024)
#define BCM_CB_MEM_END_PA		(BCM_CB_MEM_BASE_PA +		\
					BCM_CB_MEM_SIZE - 1)


/*
 * Internal registers are accessed through KSEG3
 */
#define BCM_REGS_VA(x)	((void __iomem *)(x))

#define bcm_readb(a)	(*(volatile unsigned char *)	BCM_REGS_VA(a))
#define bcm_readw(a)	(*(volatile unsigned short *)	BCM_REGS_VA(a))
#define bcm_readl(a)	(*(volatile unsigned int *)	BCM_REGS_VA(a))
#define bcm_writeb(v, a) (*(volatile unsigned char *) BCM_REGS_VA((a)) = (v))
#define bcm_writew(v, a) (*(volatile unsigned short *) BCM_REGS_VA((a)) = (v))
#define bcm_writel(v, a) (*(volatile unsigned int *) BCM_REGS_VA((a)) = (v))

/*
 * IO helpers to access register set for current CPU
 */
#define bcm_rset_readb(s, o)	bcm_readb(bcm63xx_regset_address(s) + (o))
#define bcm_rset_readw(s, o)	bcm_readw(bcm63xx_regset_address(s) + (o))
#define bcm_rset_readl(s, o)	bcm_readl(bcm63xx_regset_address(s) + (o))
#define bcm_rset_writeb(s, v, o)	bcm_writeb((v), \
					bcm63xx_regset_address(s) + (o))
#define bcm_rset_writew(s, v, o)	bcm_writew((v), \
					bcm63xx_regset_address(s) + (o))
#define bcm_rset_writel(s, v, o)	bcm_writel((v), \
					bcm63xx_regset_address(s) + (o))

/*
 * helpers for frequently used register sets
 */
#define bcm_perf_readl(o)	bcm_rset_readl(RSET_PERF, (o))
#define bcm_perf_writel(v, o)	bcm_rset_writel(RSET_PERF, (v), (o))
#define bcm_timer_readl(o)	bcm_rset_readl(RSET_TIMER, (o))
#define bcm_timer_writel(v, o)	bcm_rset_writel(RSET_TIMER, (v), (o))
#define bcm_wdt_readl(o)	bcm_rset_readl(RSET_WDT, (o))
#define bcm_wdt_writel(v, o)	bcm_rset_writel(RSET_WDT, (v), (o))
#define bcm_gpio_readl(o)	bcm_rset_readl(RSET_GPIO, (o))
#define bcm_gpio_writel(v, o)	bcm_rset_writel(RSET_GPIO, (v), (o))
#define bcm_uart0_readl(o)	bcm_rset_readl(RSET_UART0, (o))
#define bcm_uart0_writel(v, o)	bcm_rset_writel(RSET_UART0, (v), (o))
#define bcm_mpi_readl(o)	bcm_rset_readl(RSET_MPI, (o))
#define bcm_mpi_writel(v, o)	bcm_rset_writel(RSET_MPI, (v), (o))
#define bcm_pcmcia_readl(o)	bcm_rset_readl(RSET_PCMCIA, (o))
#define bcm_pcmcia_writel(v, o)	bcm_rset_writel(RSET_PCMCIA, (v), (o))
#define bcm_sdram_readl(o)	bcm_rset_readl(RSET_SDRAM, (o))
#define bcm_sdram_writel(v, o)	bcm_rset_writel(RSET_SDRAM, (v), (o))
#define bcm_memc_readl(o)	bcm_rset_readl(RSET_MEMC, (o))
#define bcm_memc_writel(v, o)	bcm_rset_writel(RSET_MEMC, (v), (o))
#define bcm_ddr_readl(o)	bcm_rset_readl(RSET_DDR, (o))
#define bcm_ddr_writel(v, o)	bcm_rset_writel(RSET_DDR, (v), (o))

#endif /* ! BCM63XX_IO_H_ */
