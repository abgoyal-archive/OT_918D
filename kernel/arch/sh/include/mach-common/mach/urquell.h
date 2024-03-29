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

#ifndef __MACH_URQUELL_H
#define __MACH_URQUELL_H

/*
 * ------ 0x00000000 ------------------------------------
 *  CS0 | (SW1,SW47)    EEPROM, SRAM, NOR FLASH
 * -----+ 0x04000000 ------------------------------------
 *  CS1 | (SW47)        SRAM, SRAM-LAN-PCMCIA, NOR FLASH
 * -----+ 0x08000000 ------------------------------------
 *  CS2 |               DDR3
 *  CS3 |
 * -----+ 0x10000000 ------------------------------------
 *  CS4 |               PCIe
 * -----+ 0x14000000 ------------------------------------
 *  CS5 | (SW47)        LRAM/URAM, SRAM-LAN-PCMCIA
 * -----+ 0x18000000 ------------------------------------
 *  CS6 |               ATA, NAND FLASH
 * -----+ 0x1c000000 ------------------------------------
 *  CS7 |               SH7786 register
 * -----+------------------------------------------------
 */

#define NOR_FLASH_ADDR	0x00000000
#define NOR_FLASH_SIZE	0x04000000

#define CS1_BASE	0x05000000
#define CS5_BASE	0x15000000
#define FPGA_BASE	CS1_BASE

#define BOARDREG(ofs)	(FPGA_BASE + ofs##_OFS)
#define UBOARDREG(ofs)	(0xa0000000 + FPGA_BASE + ofs##_OFS)

#define SRSTR_OFS	0x0000 /* System reset register */
#define BDMR_OFS	0x0010 /* Board operating mode resister */
#define IRL0SR_OFS	0x0020 /* IRL0 Status register */
#define IRL0MSKR_OFS	0x0030 /* IRL0 Mask register */
#define IRL1SR_OFS	0x0040 /* IRL1 Status register */
#define IRL1MSKR_OFS	0x0050 /* IRL1 Mask register */
#define IRL2SR_OFS	0x0060 /* IRL2 Status register */
#define IRL2MSKR_OFS	0x0070 /* IRL2 Mask register */
#define IRL3SR_OFS	0x0080 /* IRL3 Status register */
#define IRL3MSKR_OFS	0x0090 /* IRL3 Mask register */
#define SOFTINTR_OFS	0x0120 /* Softwear Interrupt register */
#define SLEDR_OFS	0x0130 /* LED control resister */
#define MAPSCIFSWR_OFS	0x0140 /* Map/SCIF Switch register */
#define FPVERR_OFS	0x0150 /* FPGA Version register */
#define FPDATER_OFS	0x0160 /* FPGA Date register */
#define FPYEARR_OFS	0x0170 /* FPGA Year register */
#define TCLKCR_OFS	0x0180 /* TCLK Control register */
#define DIPSWMR_OFS	0x1000 /* DIPSW monitor register */
#define FPODR_OFS	0x1010 /* Output port data register */
#define ATACNR_OFS	0x1020 /* ATA-CN Control/status register */
#define FPINDR_OFS	0x1030 /* Input port data register */
#define MDSWMR_OFS	0x1040 /* MODE SW monitor register */
#define DDR3BUPCR_OFS	0x1050 /* DDR3 Backup control register */
#define SSICODECCR_OFS	0x1060 /* SSI-CODEC control register */
#define PCIESLOTSR_OFS	0x1070 /* PCIexpress Slot status register */
#define ETHERPORTSR_OFS	0x1080 /* EtherPhy Port status register */
#define LATCHCR_OFS	0x3000 /* Latch control register */
#define LATCUAR_OFS	0x3010 /* Latch upper address register */
#define LATCLAR_OFS	0x3012 /* Latch lower address register */
#define LATCLUDR_OFS	0x3024 /* Latch D31-16 register */
#define LATCLLDR_OFS	0x3026 /* Latch D15-0 register */

#define CHARLED_OFS	0x2000 /* Character LED */

#endif  /* __MACH_URQUELL_H */

