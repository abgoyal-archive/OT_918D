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

#ifndef _ASM_POWERPC_KEYLARGO_H
#define _ASM_POWERPC_KEYLARGO_H
#ifdef __KERNEL__
/*
 * keylargo.h: definitions for using the "KeyLargo" I/O controller chip.
 *
 */

/* "Pangea" chipset has keylargo device-id 0x25 while core99
 * has device-id 0x22. The rev. of the pangea one is 0, so we
 * fake an artificial rev. in keylargo_rev by oring 0x100
 */
#define KL_PANGEA_REV		0x100

/* offset from base for feature control registers */
#define KEYLARGO_MBCR		0x34	/* KL Only, Media bay control/status */
#define KEYLARGO_FCR0		0x38
#define KEYLARGO_FCR1		0x3c
#define KEYLARGO_FCR2		0x40
#define KEYLARGO_FCR3		0x44
#define KEYLARGO_FCR4		0x48
#define KEYLARGO_FCR5		0x4c	/* Pangea only */

/* K2 aditional FCRs */
#define K2_FCR6			0x34
#define K2_FCR7			0x30
#define K2_FCR8			0x2c
#define K2_FCR9			0x28
#define K2_FCR10		0x24

/* GPIO registers */
#define KEYLARGO_GPIO_LEVELS0		0x50
#define KEYLARGO_GPIO_LEVELS1		0x54
#define KEYLARGO_GPIO_EXTINT_0		0x58
#define KEYLARGO_GPIO_EXTINT_CNT	18
#define KEYLARGO_GPIO_0			0x6A
#define KEYLARGO_GPIO_CNT		17
#define KEYLARGO_GPIO_EXTINT_DUAL_EDGE	0x80
#define KEYLARGO_GPIO_OUTPUT_ENABLE	0x04
#define KEYLARGO_GPIO_OUTOUT_DATA	0x01
#define KEYLARGO_GPIO_INPUT_DATA	0x02

/* K2 does only extint GPIOs and does 51 of them */
#define K2_GPIO_EXTINT_0		0x58
#define K2_GPIO_EXTINT_CNT		51

/* Specific GPIO regs */

#define KL_GPIO_MODEM_RESET		(KEYLARGO_GPIO_0+0x03)
#define KL_GPIO_MODEM_POWER		(KEYLARGO_GPIO_0+0x02) /* Pangea */

#define KL_GPIO_SOUND_POWER		(KEYLARGO_GPIO_0+0x05)

/* Hrm... this one is only to be used on Pismo. It seeem to also
 * control the timebase enable on other machines. Still to be
 * experimented... --BenH.
 */
#define KL_GPIO_FW_CABLE_POWER		(KEYLARGO_GPIO_0+0x09)
#define KL_GPIO_TB_ENABLE		(KEYLARGO_GPIO_0+0x09)

#define KL_GPIO_ETH_PHY_RESET		(KEYLARGO_GPIO_0+0x10)

#define KL_GPIO_EXTINT_CPU1		(KEYLARGO_GPIO_0+0x0a)
#define KL_GPIO_EXTINT_CPU1_ASSERT	0x04
#define KL_GPIO_EXTINT_CPU1_RELEASE	0x38

#define KL_GPIO_RESET_CPU0		(KEYLARGO_GPIO_EXTINT_0+0x03)
#define KL_GPIO_RESET_CPU1		(KEYLARGO_GPIO_EXTINT_0+0x04)
#define KL_GPIO_RESET_CPU2		(KEYLARGO_GPIO_EXTINT_0+0x0f)
#define KL_GPIO_RESET_CPU3		(KEYLARGO_GPIO_EXTINT_0+0x10)

#define KL_GPIO_PMU_MESSAGE_IRQ		(KEYLARGO_GPIO_EXTINT_0+0x09)
#define KL_GPIO_PMU_MESSAGE_BIT		KEYLARGO_GPIO_INPUT_DATA

#define KL_GPIO_MEDIABAY_IRQ		(KEYLARGO_GPIO_EXTINT_0+0x0e)

#define KL_GPIO_AIRPORT_0		(KEYLARGO_GPIO_EXTINT_0+0x0a)
#define KL_GPIO_AIRPORT_1		(KEYLARGO_GPIO_EXTINT_0+0x0d)
#define KL_GPIO_AIRPORT_2		(KEYLARGO_GPIO_0+0x0d)
#define KL_GPIO_AIRPORT_3		(KEYLARGO_GPIO_0+0x0e)
#define KL_GPIO_AIRPORT_4		(KEYLARGO_GPIO_0+0x0f)

/*
 * Bits in feature control register. Those bits different for K2 are
 * listed separately
 */
#define KL_MBCR_MB0_PCI_ENABLE		0x00000800	/* exist ? */
#define KL_MBCR_MB0_IDE_ENABLE		0x00001000
#define KL_MBCR_MB0_FLOPPY_ENABLE	0x00002000	/* exist ? */
#define KL_MBCR_MB0_SOUND_ENABLE	0x00004000	/* hrm... */
#define KL_MBCR_MB0_DEV_MASK		0x00007800
#define KL_MBCR_MB0_DEV_POWER		0x00000400
#define KL_MBCR_MB0_DEV_RESET		0x00000200
#define KL_MBCR_MB0_ENABLE		0x00000100
#define KL_MBCR_MB1_PCI_ENABLE		0x08000000	/* exist ? */
#define KL_MBCR_MB1_IDE_ENABLE		0x10000000
#define KL_MBCR_MB1_FLOPPY_ENABLE	0x20000000	/* exist ? */
#define KL_MBCR_MB1_SOUND_ENABLE	0x40000000	/* hrm... */
#define KL_MBCR_MB1_DEV_MASK		0x78000000
#define KL_MBCR_MB1_DEV_POWER		0x04000000
#define KL_MBCR_MB1_DEV_RESET		0x02000000
#define KL_MBCR_MB1_ENABLE		0x01000000

#define KL0_SCC_B_INTF_ENABLE		0x00000001	/* (KL Only) */
#define KL0_SCC_A_INTF_ENABLE		0x00000002
#define KL0_SCC_SLOWPCLK		0x00000004
#define KL0_SCC_RESET			0x00000008
#define KL0_SCCA_ENABLE			0x00000010
#define KL0_SCCB_ENABLE			0x00000020
#define KL0_SCC_CELL_ENABLE		0x00000040
#define KL0_IRDA_HIGH_BAND		0x00000100	/* (KL Only) */
#define KL0_IRDA_SOURCE2_SEL		0x00000200	/* (KL Only) */
#define KL0_IRDA_SOURCE1_SEL		0x00000400	/* (KL Only) */
#define KL0_PG_USB0_PMI_ENABLE		0x00000400	/* (Pangea/Intrepid Only) */
#define KL0_IRDA_RESET			0x00000800	/* (KL Only) */
#define KL0_PG_USB0_REF_SUSPEND_SEL	0x00000800	/* (Pangea/Intrepid Only) */
#define KL0_IRDA_DEFAULT1		0x00001000	/* (KL Only) */
#define KL0_PG_USB0_REF_SUSPEND		0x00001000	/* (Pangea/Intrepid Only) */
#define KL0_IRDA_DEFAULT0		0x00002000	/* (KL Only) */
#define KL0_PG_USB0_PAD_SUSPEND		0x00002000	/* (Pangea/Intrepid Only) */
#define KL0_IRDA_FAST_CONNECT		0x00004000	/* (KL Only) */
#define KL0_PG_USB1_PMI_ENABLE		0x00004000	/* (Pangea/Intrepid Only) */
#define KL0_IRDA_ENABLE			0x00008000	/* (KL Only) */
#define KL0_PG_USB1_REF_SUSPEND_SEL	0x00008000	/* (Pangea/Intrepid Only) */
#define KL0_IRDA_CLK32_ENABLE		0x00010000	/* (KL Only) */
#define KL0_PG_USB1_REF_SUSPEND		0x00010000	/* (Pangea/Intrepid Only) */
#define KL0_IRDA_CLK19_ENABLE		0x00020000	/* (KL Only) */
#define KL0_PG_USB1_PAD_SUSPEND		0x00020000	/* (Pangea/Intrepid Only) */
#define KL0_USB0_PAD_SUSPEND0		0x00040000
#define KL0_USB0_PAD_SUSPEND1		0x00080000
#define KL0_USB0_CELL_ENABLE		0x00100000
#define KL0_USB1_PAD_SUSPEND0		0x00400000
#define KL0_USB1_PAD_SUSPEND1		0x00800000
#define KL0_USB1_CELL_ENABLE		0x01000000
#define KL0_USB_REF_SUSPEND		0x10000000	/* (KL Only) */

#define KL0_SERIAL_ENABLE		(KL0_SCC_B_INTF_ENABLE | \
					KL0_SCC_SLOWPCLK | \
					KL0_SCC_CELL_ENABLE | KL0_SCCA_ENABLE)

#define KL1_USB2_PMI_ENABLE		0x00000001	/* Intrepid only */
#define KL1_AUDIO_SEL_22MCLK		0x00000002	/* KL/Pangea only */
#define KL1_USB2_REF_SUSPEND_SEL	0x00000002	/* Intrepid only */
#define KL1_USB2_REF_SUSPEND		0x00000004	/* Intrepid only */
#define KL1_AUDIO_CLK_ENABLE_BIT	0x00000008	/* KL/Pangea only */
#define KL1_USB2_PAD_SUSPEND_SEL	0x00000008	/* Intrepid only */
#define KL1_USB2_PAD_SUSPEND0		0x00000010	/* Intrepid only */
#define KL1_AUDIO_CLK_OUT_ENABLE	0x00000020	/* KL/Pangea only */
#define KL1_USB2_PAD_SUSPEND1		0x00000020	/* Intrepid only */
#define KL1_AUDIO_CELL_ENABLE		0x00000040	/* KL/Pangea only */
#define KL1_USB2_CELL_ENABLE		0x00000040	/* Intrepid only */
#define KL1_AUDIO_CHOOSE		0x00000080	/* KL/Pangea only */
#define KL1_I2S0_CHOOSE			0x00000200	/* KL Only */
#define KL1_I2S0_CELL_ENABLE		0x00000400
#define KL1_I2S0_CLK_ENABLE_BIT		0x00001000
#define KL1_I2S0_ENABLE			0x00002000
#define KL1_I2S1_CELL_ENABLE		0x00020000
#define KL1_I2S1_CLK_ENABLE_BIT		0x00080000
#define KL1_I2S1_ENABLE			0x00100000
#define KL1_EIDE0_ENABLE		0x00800000	/* KL/Intrepid Only */
#define KL1_EIDE0_RESET_N		0x01000000	/* KL/Intrepid Only */
#define KL1_EIDE1_ENABLE		0x04000000	/* KL Only */
#define KL1_EIDE1_RESET_N		0x08000000	/* KL Only */
#define KL1_UIDE_ENABLE			0x20000000	/* KL/Pangea Only */
#define KL1_UIDE_RESET_N		0x40000000	/* KL/Pangea Only */

#define KL2_IOBUS_ENABLE		0x00000002
#define KL2_SLEEP_STATE_BIT		0x00000100	/* KL Only */
#define KL2_PG_STOP_ALL_CLOCKS		0x00000100	/* Pangea Only */
#define KL2_MPIC_ENABLE			0x00020000
#define KL2_CARDSLOT_RESET		0x00040000	/* Pangea/Intrepid Only */
#define KL2_ALT_DATA_OUT		0x02000000	/* KL Only ??? */
#define KL2_MEM_IS_BIG			0x04000000
#define KL2_CARDSEL_16			0x08000000

#define KL3_SHUTDOWN_PLL_TOTAL		0x00000001	/* KL/Pangea only */
#define KL3_SHUTDOWN_PLLKW6		0x00000002	/* KL/Pangea only */
#define KL3_IT_SHUTDOWN_PLL3		0x00000002	/* Intrepid only */
#define KL3_SHUTDOWN_PLLKW4		0x00000004	/* KL/Pangea only */
#define KL3_IT_SHUTDOWN_PLL2		0x00000004	/* Intrepid only */
#define KL3_SHUTDOWN_PLLKW35		0x00000008	/* KL/Pangea only */
#define KL3_IT_SHUTDOWN_PLL1		0x00000008	/* Intrepid only */
#define KL3_SHUTDOWN_PLLKW12		0x00000010	/* KL Only */
#define KL3_IT_ENABLE_PLL3_SHUTDOWN	0x00000010	/* Intrepid only */
#define KL3_PLL_RESET			0x00000020	/* KL/Pangea only */
#define KL3_IT_ENABLE_PLL2_SHUTDOWN	0x00000020	/* Intrepid only */
#define KL3_IT_ENABLE_PLL1_SHUTDOWN	0x00000010	/* Intrepid only */
#define KL3_SHUTDOWN_PLL2X		0x00000080	/* KL Only */
#define KL3_CLK66_ENABLE		0x00000100	/* KL Only */
#define KL3_CLK49_ENABLE		0x00000200
#define KL3_CLK45_ENABLE		0x00000400
#define KL3_CLK31_ENABLE		0x00000800	/* KL/Pangea only */
#define KL3_TIMER_CLK18_ENABLE		0x00001000
#define KL3_I2S1_CLK18_ENABLE		0x00002000
#define KL3_I2S0_CLK18_ENABLE		0x00004000
#define KL3_VIA_CLK16_ENABLE		0x00008000	/* KL/Pangea only */
#define KL3_IT_VIA_CLK32_ENABLE		0x00008000	/* Intrepid only */
#define KL3_STOPPING33_ENABLED		0x00080000	/* KL Only */
#define KL3_PG_PLL_ENABLE_TEST		0x00080000	/* Pangea Only */

/* Intrepid USB bus 2, port 0,1 */
#define KL3_IT_PORT_WAKEUP_ENABLE(p)		(0x00080000 << ((p)<<3))
#define KL3_IT_PORT_RESUME_WAKE_EN(p)		(0x00040000 << ((p)<<3))
#define KL3_IT_PORT_CONNECT_WAKE_EN(p)		(0x00020000 << ((p)<<3))
#define KL3_IT_PORT_DISCONNECT_WAKE_EN(p)	(0x00010000 << ((p)<<3))
#define KL3_IT_PORT_RESUME_STAT(p)		(0x00300000 << ((p)<<3))
#define KL3_IT_PORT_CONNECT_STAT(p)		(0x00200000 << ((p)<<3))
#define KL3_IT_PORT_DISCONNECT_STAT(p)		(0x00100000 << ((p)<<3))

/* Port 0,1 : bus 0, port 2,3 : bus 1 */
#define KL4_PORT_WAKEUP_ENABLE(p)	(0x00000008 << ((p)<<3))
#define KL4_PORT_RESUME_WAKE_EN(p)	(0x00000004 << ((p)<<3))
#define KL4_PORT_CONNECT_WAKE_EN(p)	(0x00000002 << ((p)<<3))
#define KL4_PORT_DISCONNECT_WAKE_EN(p)	(0x00000001 << ((p)<<3))
#define KL4_PORT_RESUME_STAT(p)		(0x00000040 << ((p)<<3))
#define KL4_PORT_CONNECT_STAT(p)	(0x00000020 << ((p)<<3))
#define KL4_PORT_DISCONNECT_STAT(p)	(0x00000010 << ((p)<<3))

/* Pangea and Intrepid only */
#define KL5_VIA_USE_CLK31		0000000001	/* Pangea Only */
#define KL5_SCC_USE_CLK31		0x00000002	/* Pangea Only */
#define KL5_PWM_CLK32_EN		0x00000004
#define KL5_CLK3_68_EN			0x00000010
#define KL5_CLK32_EN			0x00000020


/* K2 definitions */
#define K2_FCR0_USB0_SWRESET		0x00200000
#define K2_FCR0_USB1_SWRESET		0x02000000
#define K2_FCR0_RING_PME_DISABLE	0x08000000

#define K2_FCR1_PCI1_BUS_RESET_N	0x00000010
#define K2_FCR1_PCI1_SLEEP_RESET_EN	0x00000020
#define K2_FCR1_I2S0_CELL_ENABLE	0x00000400
#define K2_FCR1_I2S0_RESET		0x00000800
#define K2_FCR1_I2S0_CLK_ENABLE_BIT	0x00001000
#define K2_FCR1_I2S0_ENABLE    		0x00002000
#define K2_FCR1_PCI1_CLK_ENABLE		0x00004000
#define K2_FCR1_FW_CLK_ENABLE		0x00008000
#define K2_FCR1_FW_RESET_N		0x00010000
#define K2_FCR1_I2S1_CELL_ENABLE	0x00020000
#define K2_FCR1_I2S1_CLK_ENABLE_BIT	0x00080000
#define K2_FCR1_I2S1_ENABLE		0x00100000
#define K2_FCR1_GMAC_CLK_ENABLE		0x00400000
#define K2_FCR1_GMAC_POWER_DOWN		0x00800000
#define K2_FCR1_GMAC_RESET_N		0x01000000
#define K2_FCR1_SATA_CLK_ENABLE		0x02000000
#define K2_FCR1_SATA_POWER_DOWN		0x04000000
#define K2_FCR1_SATA_RESET_N		0x08000000
#define K2_FCR1_UATA_CLK_ENABLE		0x10000000
#define K2_FCR1_UATA_RESET_N		0x40000000
#define K2_FCR1_UATA_CHOOSE_CLK66	0x80000000

/* Shasta definitions */
#define SH_FCR1_I2S2_CELL_ENABLE	0x00000010
#define SH_FCR1_I2S2_CLK_ENABLE_BIT	0x00000040
#define SH_FCR1_I2S2_ENABLE		0x00000080
#define SH_FCR3_I2S2_CLK18_ENABLE	0x00008000

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_KEYLARGO_H */
