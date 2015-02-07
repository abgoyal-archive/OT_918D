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

/***********************license start***************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
 ***********************license end**************************************/

#ifndef __CVMX_PCI_DEFS_H__
#define __CVMX_PCI_DEFS_H__

#define CVMX_PCI_BAR1_INDEXX(offset) \
	 (0x0000000000000100ull + (((offset) & 31) * 4))
#define CVMX_PCI_BIST_REG \
	 (0x00000000000001C0ull)
#define CVMX_PCI_CFG00 \
	 (0x0000000000000000ull)
#define CVMX_PCI_CFG01 \
	 (0x0000000000000004ull)
#define CVMX_PCI_CFG02 \
	 (0x0000000000000008ull)
#define CVMX_PCI_CFG03 \
	 (0x000000000000000Cull)
#define CVMX_PCI_CFG04 \
	 (0x0000000000000010ull)
#define CVMX_PCI_CFG05 \
	 (0x0000000000000014ull)
#define CVMX_PCI_CFG06 \
	 (0x0000000000000018ull)
#define CVMX_PCI_CFG07 \
	 (0x000000000000001Cull)
#define CVMX_PCI_CFG08 \
	 (0x0000000000000020ull)
#define CVMX_PCI_CFG09 \
	 (0x0000000000000024ull)
#define CVMX_PCI_CFG10 \
	 (0x0000000000000028ull)
#define CVMX_PCI_CFG11 \
	 (0x000000000000002Cull)
#define CVMX_PCI_CFG12 \
	 (0x0000000000000030ull)
#define CVMX_PCI_CFG13 \
	 (0x0000000000000034ull)
#define CVMX_PCI_CFG15 \
	 (0x000000000000003Cull)
#define CVMX_PCI_CFG16 \
	 (0x0000000000000040ull)
#define CVMX_PCI_CFG17 \
	 (0x0000000000000044ull)
#define CVMX_PCI_CFG18 \
	 (0x0000000000000048ull)
#define CVMX_PCI_CFG19 \
	 (0x000000000000004Cull)
#define CVMX_PCI_CFG20 \
	 (0x0000000000000050ull)
#define CVMX_PCI_CFG21 \
	 (0x0000000000000054ull)
#define CVMX_PCI_CFG22 \
	 (0x0000000000000058ull)
#define CVMX_PCI_CFG56 \
	 (0x00000000000000E0ull)
#define CVMX_PCI_CFG57 \
	 (0x00000000000000E4ull)
#define CVMX_PCI_CFG58 \
	 (0x00000000000000E8ull)
#define CVMX_PCI_CFG59 \
	 (0x00000000000000ECull)
#define CVMX_PCI_CFG60 \
	 (0x00000000000000F0ull)
#define CVMX_PCI_CFG61 \
	 (0x00000000000000F4ull)
#define CVMX_PCI_CFG62 \
	 (0x00000000000000F8ull)
#define CVMX_PCI_CFG63 \
	 (0x00000000000000FCull)
#define CVMX_PCI_CNT_REG \
	 (0x00000000000001B8ull)
#define CVMX_PCI_CTL_STATUS_2 \
	 (0x000000000000018Cull)
#define CVMX_PCI_DBELL_0 \
	 (0x0000000000000080ull)
#define CVMX_PCI_DBELL_1 \
	 (0x0000000000000088ull)
#define CVMX_PCI_DBELL_2 \
	 (0x0000000000000090ull)
#define CVMX_PCI_DBELL_3 \
	 (0x0000000000000098ull)
#define CVMX_PCI_DBELL_X(offset) \
	 (0x0000000000000080ull + (((offset) & 3) * 8))
#define CVMX_PCI_DMA_CNT0 \
	 (0x00000000000000A0ull)
#define CVMX_PCI_DMA_CNT1 \
	 (0x00000000000000A8ull)
#define CVMX_PCI_DMA_CNTX(offset) \
	 (0x00000000000000A0ull + (((offset) & 1) * 8))
#define CVMX_PCI_DMA_INT_LEV0 \
	 (0x00000000000000A4ull)
#define CVMX_PCI_DMA_INT_LEV1 \
	 (0x00000000000000ACull)
#define CVMX_PCI_DMA_INT_LEVX(offset) \
	 (0x00000000000000A4ull + (((offset) & 1) * 8))
#define CVMX_PCI_DMA_TIME0 \
	 (0x00000000000000B0ull)
#define CVMX_PCI_DMA_TIME1 \
	 (0x00000000000000B4ull)
#define CVMX_PCI_DMA_TIMEX(offset) \
	 (0x00000000000000B0ull + (((offset) & 1) * 4))
#define CVMX_PCI_INSTR_COUNT0 \
	 (0x0000000000000084ull)
#define CVMX_PCI_INSTR_COUNT1 \
	 (0x000000000000008Cull)
#define CVMX_PCI_INSTR_COUNT2 \
	 (0x0000000000000094ull)
#define CVMX_PCI_INSTR_COUNT3 \
	 (0x000000000000009Cull)
#define CVMX_PCI_INSTR_COUNTX(offset) \
	 (0x0000000000000084ull + (((offset) & 3) * 8))
#define CVMX_PCI_INT_ENB \
	 (0x0000000000000038ull)
#define CVMX_PCI_INT_ENB2 \
	 (0x00000000000001A0ull)
#define CVMX_PCI_INT_SUM \
	 (0x0000000000000030ull)
#define CVMX_PCI_INT_SUM2 \
	 (0x0000000000000198ull)
#define CVMX_PCI_MSI_RCV \
	 (0x00000000000000F0ull)
#define CVMX_PCI_PKTS_SENT0 \
	 (0x0000000000000040ull)
#define CVMX_PCI_PKTS_SENT1 \
	 (0x0000000000000050ull)
#define CVMX_PCI_PKTS_SENT2 \
	 (0x0000000000000060ull)
#define CVMX_PCI_PKTS_SENT3 \
	 (0x0000000000000070ull)
#define CVMX_PCI_PKTS_SENTX(offset) \
	 (0x0000000000000040ull + (((offset) & 3) * 16))
#define CVMX_PCI_PKTS_SENT_INT_LEV0 \
	 (0x0000000000000048ull)
#define CVMX_PCI_PKTS_SENT_INT_LEV1 \
	 (0x0000000000000058ull)
#define CVMX_PCI_PKTS_SENT_INT_LEV2 \
	 (0x0000000000000068ull)
#define CVMX_PCI_PKTS_SENT_INT_LEV3 \
	 (0x0000000000000078ull)
#define CVMX_PCI_PKTS_SENT_INT_LEVX(offset) \
	 (0x0000000000000048ull + (((offset) & 3) * 16))
#define CVMX_PCI_PKTS_SENT_TIME0 \
	 (0x000000000000004Cull)
#define CVMX_PCI_PKTS_SENT_TIME1 \
	 (0x000000000000005Cull)
#define CVMX_PCI_PKTS_SENT_TIME2 \
	 (0x000000000000006Cull)
#define CVMX_PCI_PKTS_SENT_TIME3 \
	 (0x000000000000007Cull)
#define CVMX_PCI_PKTS_SENT_TIMEX(offset) \
	 (0x000000000000004Cull + (((offset) & 3) * 16))
#define CVMX_PCI_PKT_CREDITS0 \
	 (0x0000000000000044ull)
#define CVMX_PCI_PKT_CREDITS1 \
	 (0x0000000000000054ull)
#define CVMX_PCI_PKT_CREDITS2 \
	 (0x0000000000000064ull)
#define CVMX_PCI_PKT_CREDITS3 \
	 (0x0000000000000074ull)
#define CVMX_PCI_PKT_CREDITSX(offset) \
	 (0x0000000000000044ull + (((offset) & 3) * 16))
#define CVMX_PCI_READ_CMD_6 \
	 (0x0000000000000180ull)
#define CVMX_PCI_READ_CMD_C \
	 (0x0000000000000184ull)
#define CVMX_PCI_READ_CMD_E \
	 (0x0000000000000188ull)
#define CVMX_PCI_READ_TIMEOUT \
	 CVMX_ADD_IO_SEG(0x00011F00000000B0ull)
#define CVMX_PCI_SCM_REG \
	 (0x00000000000001A8ull)
#define CVMX_PCI_TSR_REG \
	 (0x00000000000001B0ull)
#define CVMX_PCI_WIN_RD_ADDR \
	 (0x0000000000000008ull)
#define CVMX_PCI_WIN_RD_DATA \
	 (0x0000000000000020ull)
#define CVMX_PCI_WIN_WR_ADDR \
	 (0x0000000000000000ull)
#define CVMX_PCI_WIN_WR_DATA \
	 (0x0000000000000010ull)
#define CVMX_PCI_WIN_WR_MASK \
	 (0x0000000000000018ull)

union cvmx_pci_bar1_indexx {
	uint32_t u32;
	struct cvmx_pci_bar1_indexx_s {
		uint32_t reserved_18_31:14;
		uint32_t addr_idx:14;
		uint32_t ca:1;
		uint32_t end_swp:2;
		uint32_t addr_v:1;
	} s;
	struct cvmx_pci_bar1_indexx_s cn30xx;
	struct cvmx_pci_bar1_indexx_s cn31xx;
	struct cvmx_pci_bar1_indexx_s cn38xx;
	struct cvmx_pci_bar1_indexx_s cn38xxp2;
	struct cvmx_pci_bar1_indexx_s cn50xx;
	struct cvmx_pci_bar1_indexx_s cn58xx;
	struct cvmx_pci_bar1_indexx_s cn58xxp1;
};

union cvmx_pci_bist_reg {
	uint64_t u64;
	struct cvmx_pci_bist_reg_s {
		uint64_t reserved_10_63:54;
		uint64_t rsp_bs:1;
		uint64_t dma0_bs:1;
		uint64_t cmd0_bs:1;
		uint64_t cmd_bs:1;
		uint64_t csr2p_bs:1;
		uint64_t csrr_bs:1;
		uint64_t rsp2p_bs:1;
		uint64_t csr2n_bs:1;
		uint64_t dat2n_bs:1;
		uint64_t dbg2n_bs:1;
	} s;
	struct cvmx_pci_bist_reg_s cn50xx;
};

union cvmx_pci_cfg00 {
	uint32_t u32;
	struct cvmx_pci_cfg00_s {
		uint32_t devid:16;
		uint32_t vendid:16;
	} s;
	struct cvmx_pci_cfg00_s cn30xx;
	struct cvmx_pci_cfg00_s cn31xx;
	struct cvmx_pci_cfg00_s cn38xx;
	struct cvmx_pci_cfg00_s cn38xxp2;
	struct cvmx_pci_cfg00_s cn50xx;
	struct cvmx_pci_cfg00_s cn58xx;
	struct cvmx_pci_cfg00_s cn58xxp1;
};

union cvmx_pci_cfg01 {
	uint32_t u32;
	struct cvmx_pci_cfg01_s {
		uint32_t dpe:1;
		uint32_t sse:1;
		uint32_t rma:1;
		uint32_t rta:1;
		uint32_t sta:1;
		uint32_t devt:2;
		uint32_t mdpe:1;
		uint32_t fbb:1;
		uint32_t reserved_22_22:1;
		uint32_t m66:1;
		uint32_t cle:1;
		uint32_t i_stat:1;
		uint32_t reserved_11_18:8;
		uint32_t i_dis:1;
		uint32_t fbbe:1;
		uint32_t see:1;
		uint32_t ads:1;
		uint32_t pee:1;
		uint32_t vps:1;
		uint32_t mwice:1;
		uint32_t scse:1;
		uint32_t me:1;
		uint32_t msae:1;
		uint32_t isae:1;
	} s;
	struct cvmx_pci_cfg01_s cn30xx;
	struct cvmx_pci_cfg01_s cn31xx;
	struct cvmx_pci_cfg01_s cn38xx;
	struct cvmx_pci_cfg01_s cn38xxp2;
	struct cvmx_pci_cfg01_s cn50xx;
	struct cvmx_pci_cfg01_s cn58xx;
	struct cvmx_pci_cfg01_s cn58xxp1;
};

union cvmx_pci_cfg02 {
	uint32_t u32;
	struct cvmx_pci_cfg02_s {
		uint32_t cc:24;
		uint32_t rid:8;
	} s;
	struct cvmx_pci_cfg02_s cn30xx;
	struct cvmx_pci_cfg02_s cn31xx;
	struct cvmx_pci_cfg02_s cn38xx;
	struct cvmx_pci_cfg02_s cn38xxp2;
	struct cvmx_pci_cfg02_s cn50xx;
	struct cvmx_pci_cfg02_s cn58xx;
	struct cvmx_pci_cfg02_s cn58xxp1;
};

union cvmx_pci_cfg03 {
	uint32_t u32;
	struct cvmx_pci_cfg03_s {
		uint32_t bcap:1;
		uint32_t brb:1;
		uint32_t reserved_28_29:2;
		uint32_t bcod:4;
		uint32_t ht:8;
		uint32_t lt:8;
		uint32_t cls:8;
	} s;
	struct cvmx_pci_cfg03_s cn30xx;
	struct cvmx_pci_cfg03_s cn31xx;
	struct cvmx_pci_cfg03_s cn38xx;
	struct cvmx_pci_cfg03_s cn38xxp2;
	struct cvmx_pci_cfg03_s cn50xx;
	struct cvmx_pci_cfg03_s cn58xx;
	struct cvmx_pci_cfg03_s cn58xxp1;
};

union cvmx_pci_cfg04 {
	uint32_t u32;
	struct cvmx_pci_cfg04_s {
		uint32_t lbase:20;
		uint32_t lbasez:8;
		uint32_t pf:1;
		uint32_t typ:2;
		uint32_t mspc:1;
	} s;
	struct cvmx_pci_cfg04_s cn30xx;
	struct cvmx_pci_cfg04_s cn31xx;
	struct cvmx_pci_cfg04_s cn38xx;
	struct cvmx_pci_cfg04_s cn38xxp2;
	struct cvmx_pci_cfg04_s cn50xx;
	struct cvmx_pci_cfg04_s cn58xx;
	struct cvmx_pci_cfg04_s cn58xxp1;
};

union cvmx_pci_cfg05 {
	uint32_t u32;
	struct cvmx_pci_cfg05_s {
		uint32_t hbase:32;
	} s;
	struct cvmx_pci_cfg05_s cn30xx;
	struct cvmx_pci_cfg05_s cn31xx;
	struct cvmx_pci_cfg05_s cn38xx;
	struct cvmx_pci_cfg05_s cn38xxp2;
	struct cvmx_pci_cfg05_s cn50xx;
	struct cvmx_pci_cfg05_s cn58xx;
	struct cvmx_pci_cfg05_s cn58xxp1;
};

union cvmx_pci_cfg06 {
	uint32_t u32;
	struct cvmx_pci_cfg06_s {
		uint32_t lbase:5;
		uint32_t lbasez:23;
		uint32_t pf:1;
		uint32_t typ:2;
		uint32_t mspc:1;
	} s;
	struct cvmx_pci_cfg06_s cn30xx;
	struct cvmx_pci_cfg06_s cn31xx;
	struct cvmx_pci_cfg06_s cn38xx;
	struct cvmx_pci_cfg06_s cn38xxp2;
	struct cvmx_pci_cfg06_s cn50xx;
	struct cvmx_pci_cfg06_s cn58xx;
	struct cvmx_pci_cfg06_s cn58xxp1;
};

union cvmx_pci_cfg07 {
	uint32_t u32;
	struct cvmx_pci_cfg07_s {
		uint32_t hbase:32;
	} s;
	struct cvmx_pci_cfg07_s cn30xx;
	struct cvmx_pci_cfg07_s cn31xx;
	struct cvmx_pci_cfg07_s cn38xx;
	struct cvmx_pci_cfg07_s cn38xxp2;
	struct cvmx_pci_cfg07_s cn50xx;
	struct cvmx_pci_cfg07_s cn58xx;
	struct cvmx_pci_cfg07_s cn58xxp1;
};

union cvmx_pci_cfg08 {
	uint32_t u32;
	struct cvmx_pci_cfg08_s {
		uint32_t lbasez:28;
		uint32_t pf:1;
		uint32_t typ:2;
		uint32_t mspc:1;
	} s;
	struct cvmx_pci_cfg08_s cn30xx;
	struct cvmx_pci_cfg08_s cn31xx;
	struct cvmx_pci_cfg08_s cn38xx;
	struct cvmx_pci_cfg08_s cn38xxp2;
	struct cvmx_pci_cfg08_s cn50xx;
	struct cvmx_pci_cfg08_s cn58xx;
	struct cvmx_pci_cfg08_s cn58xxp1;
};

union cvmx_pci_cfg09 {
	uint32_t u32;
	struct cvmx_pci_cfg09_s {
		uint32_t hbase:25;
		uint32_t hbasez:7;
	} s;
	struct cvmx_pci_cfg09_s cn30xx;
	struct cvmx_pci_cfg09_s cn31xx;
	struct cvmx_pci_cfg09_s cn38xx;
	struct cvmx_pci_cfg09_s cn38xxp2;
	struct cvmx_pci_cfg09_s cn50xx;
	struct cvmx_pci_cfg09_s cn58xx;
	struct cvmx_pci_cfg09_s cn58xxp1;
};

union cvmx_pci_cfg10 {
	uint32_t u32;
	struct cvmx_pci_cfg10_s {
		uint32_t cisp:32;
	} s;
	struct cvmx_pci_cfg10_s cn30xx;
	struct cvmx_pci_cfg10_s cn31xx;
	struct cvmx_pci_cfg10_s cn38xx;
	struct cvmx_pci_cfg10_s cn38xxp2;
	struct cvmx_pci_cfg10_s cn50xx;
	struct cvmx_pci_cfg10_s cn58xx;
	struct cvmx_pci_cfg10_s cn58xxp1;
};

union cvmx_pci_cfg11 {
	uint32_t u32;
	struct cvmx_pci_cfg11_s {
		uint32_t ssid:16;
		uint32_t ssvid:16;
	} s;
	struct cvmx_pci_cfg11_s cn30xx;
	struct cvmx_pci_cfg11_s cn31xx;
	struct cvmx_pci_cfg11_s cn38xx;
	struct cvmx_pci_cfg11_s cn38xxp2;
	struct cvmx_pci_cfg11_s cn50xx;
	struct cvmx_pci_cfg11_s cn58xx;
	struct cvmx_pci_cfg11_s cn58xxp1;
};

union cvmx_pci_cfg12 {
	uint32_t u32;
	struct cvmx_pci_cfg12_s {
		uint32_t erbar:16;
		uint32_t erbarz:5;
		uint32_t reserved_1_10:10;
		uint32_t erbar_en:1;
	} s;
	struct cvmx_pci_cfg12_s cn30xx;
	struct cvmx_pci_cfg12_s cn31xx;
	struct cvmx_pci_cfg12_s cn38xx;
	struct cvmx_pci_cfg12_s cn38xxp2;
	struct cvmx_pci_cfg12_s cn50xx;
	struct cvmx_pci_cfg12_s cn58xx;
	struct cvmx_pci_cfg12_s cn58xxp1;
};

union cvmx_pci_cfg13 {
	uint32_t u32;
	struct cvmx_pci_cfg13_s {
		uint32_t reserved_8_31:24;
		uint32_t cp:8;
	} s;
	struct cvmx_pci_cfg13_s cn30xx;
	struct cvmx_pci_cfg13_s cn31xx;
	struct cvmx_pci_cfg13_s cn38xx;
	struct cvmx_pci_cfg13_s cn38xxp2;
	struct cvmx_pci_cfg13_s cn50xx;
	struct cvmx_pci_cfg13_s cn58xx;
	struct cvmx_pci_cfg13_s cn58xxp1;
};

union cvmx_pci_cfg15 {
	uint32_t u32;
	struct cvmx_pci_cfg15_s {
		uint32_t ml:8;
		uint32_t mg:8;
		uint32_t inta:8;
		uint32_t il:8;
	} s;
	struct cvmx_pci_cfg15_s cn30xx;
	struct cvmx_pci_cfg15_s cn31xx;
	struct cvmx_pci_cfg15_s cn38xx;
	struct cvmx_pci_cfg15_s cn38xxp2;
	struct cvmx_pci_cfg15_s cn50xx;
	struct cvmx_pci_cfg15_s cn58xx;
	struct cvmx_pci_cfg15_s cn58xxp1;
};

union cvmx_pci_cfg16 {
	uint32_t u32;
	struct cvmx_pci_cfg16_s {
		uint32_t trdnpr:1;
		uint32_t trdard:1;
		uint32_t rdsati:1;
		uint32_t trdrs:1;
		uint32_t trtae:1;
		uint32_t twsei:1;
		uint32_t twsen:1;
		uint32_t twtae:1;
		uint32_t tmae:1;
		uint32_t tslte:3;
		uint32_t tilt:4;
		uint32_t pbe:12;
		uint32_t dppmr:1;
		uint32_t reserved_2_2:1;
		uint32_t tswc:1;
		uint32_t mltd:1;
	} s;
	struct cvmx_pci_cfg16_s cn30xx;
	struct cvmx_pci_cfg16_s cn31xx;
	struct cvmx_pci_cfg16_s cn38xx;
	struct cvmx_pci_cfg16_s cn38xxp2;
	struct cvmx_pci_cfg16_s cn50xx;
	struct cvmx_pci_cfg16_s cn58xx;
	struct cvmx_pci_cfg16_s cn58xxp1;
};

union cvmx_pci_cfg17 {
	uint32_t u32;
	struct cvmx_pci_cfg17_s {
		uint32_t tscme:32;
	} s;
	struct cvmx_pci_cfg17_s cn30xx;
	struct cvmx_pci_cfg17_s cn31xx;
	struct cvmx_pci_cfg17_s cn38xx;
	struct cvmx_pci_cfg17_s cn38xxp2;
	struct cvmx_pci_cfg17_s cn50xx;
	struct cvmx_pci_cfg17_s cn58xx;
	struct cvmx_pci_cfg17_s cn58xxp1;
};

union cvmx_pci_cfg18 {
	uint32_t u32;
	struct cvmx_pci_cfg18_s {
		uint32_t tdsrps:32;
	} s;
	struct cvmx_pci_cfg18_s cn30xx;
	struct cvmx_pci_cfg18_s cn31xx;
	struct cvmx_pci_cfg18_s cn38xx;
	struct cvmx_pci_cfg18_s cn38xxp2;
	struct cvmx_pci_cfg18_s cn50xx;
	struct cvmx_pci_cfg18_s cn58xx;
	struct cvmx_pci_cfg18_s cn58xxp1;
};

union cvmx_pci_cfg19 {
	uint32_t u32;
	struct cvmx_pci_cfg19_s {
		uint32_t mrbcm:1;
		uint32_t mrbci:1;
		uint32_t mdwe:1;
		uint32_t mdre:1;
		uint32_t mdrimc:1;
		uint32_t mdrrmc:3;
		uint32_t tmes:8;
		uint32_t teci:1;
		uint32_t tmei:1;
		uint32_t tmse:1;
		uint32_t tmdpes:1;
		uint32_t tmapes:1;
		uint32_t reserved_9_10:2;
		uint32_t tibcd:1;
		uint32_t tibde:1;
		uint32_t reserved_6_6:1;
		uint32_t tidomc:1;
		uint32_t tdomc:5;
	} s;
	struct cvmx_pci_cfg19_s cn30xx;
	struct cvmx_pci_cfg19_s cn31xx;
	struct cvmx_pci_cfg19_s cn38xx;
	struct cvmx_pci_cfg19_s cn38xxp2;
	struct cvmx_pci_cfg19_s cn50xx;
	struct cvmx_pci_cfg19_s cn58xx;
	struct cvmx_pci_cfg19_s cn58xxp1;
};

union cvmx_pci_cfg20 {
	uint32_t u32;
	struct cvmx_pci_cfg20_s {
		uint32_t mdsp:32;
	} s;
	struct cvmx_pci_cfg20_s cn30xx;
	struct cvmx_pci_cfg20_s cn31xx;
	struct cvmx_pci_cfg20_s cn38xx;
	struct cvmx_pci_cfg20_s cn38xxp2;
	struct cvmx_pci_cfg20_s cn50xx;
	struct cvmx_pci_cfg20_s cn58xx;
	struct cvmx_pci_cfg20_s cn58xxp1;
};

union cvmx_pci_cfg21 {
	uint32_t u32;
	struct cvmx_pci_cfg21_s {
		uint32_t scmre:32;
	} s;
	struct cvmx_pci_cfg21_s cn30xx;
	struct cvmx_pci_cfg21_s cn31xx;
	struct cvmx_pci_cfg21_s cn38xx;
	struct cvmx_pci_cfg21_s cn38xxp2;
	struct cvmx_pci_cfg21_s cn50xx;
	struct cvmx_pci_cfg21_s cn58xx;
	struct cvmx_pci_cfg21_s cn58xxp1;
};

union cvmx_pci_cfg22 {
	uint32_t u32;
	struct cvmx_pci_cfg22_s {
		uint32_t mac:7;
		uint32_t reserved_19_24:6;
		uint32_t flush:1;
		uint32_t mra:1;
		uint32_t mtta:1;
		uint32_t mrv:8;
		uint32_t mttv:8;
	} s;
	struct cvmx_pci_cfg22_s cn30xx;
	struct cvmx_pci_cfg22_s cn31xx;
	struct cvmx_pci_cfg22_s cn38xx;
	struct cvmx_pci_cfg22_s cn38xxp2;
	struct cvmx_pci_cfg22_s cn50xx;
	struct cvmx_pci_cfg22_s cn58xx;
	struct cvmx_pci_cfg22_s cn58xxp1;
};

union cvmx_pci_cfg56 {
	uint32_t u32;
	struct cvmx_pci_cfg56_s {
		uint32_t reserved_23_31:9;
		uint32_t most:3;
		uint32_t mmbc:2;
		uint32_t roe:1;
		uint32_t dpere:1;
		uint32_t ncp:8;
		uint32_t pxcid:8;
	} s;
	struct cvmx_pci_cfg56_s cn30xx;
	struct cvmx_pci_cfg56_s cn31xx;
	struct cvmx_pci_cfg56_s cn38xx;
	struct cvmx_pci_cfg56_s cn38xxp2;
	struct cvmx_pci_cfg56_s cn50xx;
	struct cvmx_pci_cfg56_s cn58xx;
	struct cvmx_pci_cfg56_s cn58xxp1;
};

union cvmx_pci_cfg57 {
	uint32_t u32;
	struct cvmx_pci_cfg57_s {
		uint32_t reserved_30_31:2;
		uint32_t scemr:1;
		uint32_t mcrsd:3;
		uint32_t mostd:3;
		uint32_t mmrbcd:2;
		uint32_t dc:1;
		uint32_t usc:1;
		uint32_t scd:1;
		uint32_t m133:1;
		uint32_t w64:1;
		uint32_t bn:8;
		uint32_t dn:5;
		uint32_t fn:3;
	} s;
	struct cvmx_pci_cfg57_s cn30xx;
	struct cvmx_pci_cfg57_s cn31xx;
	struct cvmx_pci_cfg57_s cn38xx;
	struct cvmx_pci_cfg57_s cn38xxp2;
	struct cvmx_pci_cfg57_s cn50xx;
	struct cvmx_pci_cfg57_s cn58xx;
	struct cvmx_pci_cfg57_s cn58xxp1;
};

union cvmx_pci_cfg58 {
	uint32_t u32;
	struct cvmx_pci_cfg58_s {
		uint32_t pmes:5;
		uint32_t d2s:1;
		uint32_t d1s:1;
		uint32_t auxc:3;
		uint32_t dsi:1;
		uint32_t reserved_20_20:1;
		uint32_t pmec:1;
		uint32_t pcimiv:3;
		uint32_t ncp:8;
		uint32_t pmcid:8;
	} s;
	struct cvmx_pci_cfg58_s cn30xx;
	struct cvmx_pci_cfg58_s cn31xx;
	struct cvmx_pci_cfg58_s cn38xx;
	struct cvmx_pci_cfg58_s cn38xxp2;
	struct cvmx_pci_cfg58_s cn50xx;
	struct cvmx_pci_cfg58_s cn58xx;
	struct cvmx_pci_cfg58_s cn58xxp1;
};

union cvmx_pci_cfg59 {
	uint32_t u32;
	struct cvmx_pci_cfg59_s {
		uint32_t pmdia:8;
		uint32_t bpccen:1;
		uint32_t bd3h:1;
		uint32_t reserved_16_21:6;
		uint32_t pmess:1;
		uint32_t pmedsia:2;
		uint32_t pmds:4;
		uint32_t pmeens:1;
		uint32_t reserved_2_7:6;
		uint32_t ps:2;
	} s;
	struct cvmx_pci_cfg59_s cn30xx;
	struct cvmx_pci_cfg59_s cn31xx;
	struct cvmx_pci_cfg59_s cn38xx;
	struct cvmx_pci_cfg59_s cn38xxp2;
	struct cvmx_pci_cfg59_s cn50xx;
	struct cvmx_pci_cfg59_s cn58xx;
	struct cvmx_pci_cfg59_s cn58xxp1;
};

union cvmx_pci_cfg60 {
	uint32_t u32;
	struct cvmx_pci_cfg60_s {
		uint32_t reserved_24_31:8;
		uint32_t m64:1;
		uint32_t mme:3;
		uint32_t mmc:3;
		uint32_t msien:1;
		uint32_t ncp:8;
		uint32_t msicid:8;
	} s;
	struct cvmx_pci_cfg60_s cn30xx;
	struct cvmx_pci_cfg60_s cn31xx;
	struct cvmx_pci_cfg60_s cn38xx;
	struct cvmx_pci_cfg60_s cn38xxp2;
	struct cvmx_pci_cfg60_s cn50xx;
	struct cvmx_pci_cfg60_s cn58xx;
	struct cvmx_pci_cfg60_s cn58xxp1;
};

union cvmx_pci_cfg61 {
	uint32_t u32;
	struct cvmx_pci_cfg61_s {
		uint32_t msi31t2:30;
		uint32_t reserved_0_1:2;
	} s;
	struct cvmx_pci_cfg61_s cn30xx;
	struct cvmx_pci_cfg61_s cn31xx;
	struct cvmx_pci_cfg61_s cn38xx;
	struct cvmx_pci_cfg61_s cn38xxp2;
	struct cvmx_pci_cfg61_s cn50xx;
	struct cvmx_pci_cfg61_s cn58xx;
	struct cvmx_pci_cfg61_s cn58xxp1;
};

union cvmx_pci_cfg62 {
	uint32_t u32;
	struct cvmx_pci_cfg62_s {
		uint32_t msi:32;
	} s;
	struct cvmx_pci_cfg62_s cn30xx;
	struct cvmx_pci_cfg62_s cn31xx;
	struct cvmx_pci_cfg62_s cn38xx;
	struct cvmx_pci_cfg62_s cn38xxp2;
	struct cvmx_pci_cfg62_s cn50xx;
	struct cvmx_pci_cfg62_s cn58xx;
	struct cvmx_pci_cfg62_s cn58xxp1;
};

union cvmx_pci_cfg63 {
	uint32_t u32;
	struct cvmx_pci_cfg63_s {
		uint32_t reserved_16_31:16;
		uint32_t msimd:16;
	} s;
	struct cvmx_pci_cfg63_s cn30xx;
	struct cvmx_pci_cfg63_s cn31xx;
	struct cvmx_pci_cfg63_s cn38xx;
	struct cvmx_pci_cfg63_s cn38xxp2;
	struct cvmx_pci_cfg63_s cn50xx;
	struct cvmx_pci_cfg63_s cn58xx;
	struct cvmx_pci_cfg63_s cn58xxp1;
};

union cvmx_pci_cnt_reg {
	uint64_t u64;
	struct cvmx_pci_cnt_reg_s {
		uint64_t reserved_38_63:26;
		uint64_t hm_pcix:1;
		uint64_t hm_speed:2;
		uint64_t ap_pcix:1;
		uint64_t ap_speed:2;
		uint64_t pcicnt:32;
	} s;
	struct cvmx_pci_cnt_reg_s cn50xx;
	struct cvmx_pci_cnt_reg_s cn58xx;
	struct cvmx_pci_cnt_reg_s cn58xxp1;
};

union cvmx_pci_ctl_status_2 {
	uint32_t u32;
	struct cvmx_pci_ctl_status_2_s {
		uint32_t reserved_29_31:3;
		uint32_t bb1_hole:3;
		uint32_t bb1_siz:1;
		uint32_t bb_ca:1;
		uint32_t bb_es:2;
		uint32_t bb1:1;
		uint32_t bb0:1;
		uint32_t erst_n:1;
		uint32_t bar2pres:1;
		uint32_t scmtyp:1;
		uint32_t scm:1;
		uint32_t en_wfilt:1;
		uint32_t reserved_14_14:1;
		uint32_t ap_pcix:1;
		uint32_t ap_64ad:1;
		uint32_t b12_bist:1;
		uint32_t pmo_amod:1;
		uint32_t pmo_fpc:3;
		uint32_t tsr_hwm:3;
		uint32_t bar2_enb:1;
		uint32_t bar2_esx:2;
		uint32_t bar2_cax:1;
	} s;
	struct cvmx_pci_ctl_status_2_s cn30xx;
	struct cvmx_pci_ctl_status_2_cn31xx {
		uint32_t reserved_20_31:12;
		uint32_t erst_n:1;
		uint32_t bar2pres:1;
		uint32_t scmtyp:1;
		uint32_t scm:1;
		uint32_t en_wfilt:1;
		uint32_t reserved_14_14:1;
		uint32_t ap_pcix:1;
		uint32_t ap_64ad:1;
		uint32_t b12_bist:1;
		uint32_t pmo_amod:1;
		uint32_t pmo_fpc:3;
		uint32_t tsr_hwm:3;
		uint32_t bar2_enb:1;
		uint32_t bar2_esx:2;
		uint32_t bar2_cax:1;
	} cn31xx;
	struct cvmx_pci_ctl_status_2_s cn38xx;
	struct cvmx_pci_ctl_status_2_cn31xx cn38xxp2;
	struct cvmx_pci_ctl_status_2_s cn50xx;
	struct cvmx_pci_ctl_status_2_s cn58xx;
	struct cvmx_pci_ctl_status_2_s cn58xxp1;
};

union cvmx_pci_dbellx {
	uint32_t u32;
	struct cvmx_pci_dbellx_s {
		uint32_t reserved_16_31:16;
		uint32_t inc_val:16;
	} s;
	struct cvmx_pci_dbellx_s cn30xx;
	struct cvmx_pci_dbellx_s cn31xx;
	struct cvmx_pci_dbellx_s cn38xx;
	struct cvmx_pci_dbellx_s cn38xxp2;
	struct cvmx_pci_dbellx_s cn50xx;
	struct cvmx_pci_dbellx_s cn58xx;
	struct cvmx_pci_dbellx_s cn58xxp1;
};

union cvmx_pci_dma_cntx {
	uint32_t u32;
	struct cvmx_pci_dma_cntx_s {
		uint32_t dma_cnt:32;
	} s;
	struct cvmx_pci_dma_cntx_s cn30xx;
	struct cvmx_pci_dma_cntx_s cn31xx;
	struct cvmx_pci_dma_cntx_s cn38xx;
	struct cvmx_pci_dma_cntx_s cn38xxp2;
	struct cvmx_pci_dma_cntx_s cn50xx;
	struct cvmx_pci_dma_cntx_s cn58xx;
	struct cvmx_pci_dma_cntx_s cn58xxp1;
};

union cvmx_pci_dma_int_levx {
	uint32_t u32;
	struct cvmx_pci_dma_int_levx_s {
		uint32_t pkt_cnt:32;
	} s;
	struct cvmx_pci_dma_int_levx_s cn30xx;
	struct cvmx_pci_dma_int_levx_s cn31xx;
	struct cvmx_pci_dma_int_levx_s cn38xx;
	struct cvmx_pci_dma_int_levx_s cn38xxp2;
	struct cvmx_pci_dma_int_levx_s cn50xx;
	struct cvmx_pci_dma_int_levx_s cn58xx;
	struct cvmx_pci_dma_int_levx_s cn58xxp1;
};

union cvmx_pci_dma_timex {
	uint32_t u32;
	struct cvmx_pci_dma_timex_s {
		uint32_t dma_time:32;
	} s;
	struct cvmx_pci_dma_timex_s cn30xx;
	struct cvmx_pci_dma_timex_s cn31xx;
	struct cvmx_pci_dma_timex_s cn38xx;
	struct cvmx_pci_dma_timex_s cn38xxp2;
	struct cvmx_pci_dma_timex_s cn50xx;
	struct cvmx_pci_dma_timex_s cn58xx;
	struct cvmx_pci_dma_timex_s cn58xxp1;
};

union cvmx_pci_instr_countx {
	uint32_t u32;
	struct cvmx_pci_instr_countx_s {
		uint32_t icnt:32;
	} s;
	struct cvmx_pci_instr_countx_s cn30xx;
	struct cvmx_pci_instr_countx_s cn31xx;
	struct cvmx_pci_instr_countx_s cn38xx;
	struct cvmx_pci_instr_countx_s cn38xxp2;
	struct cvmx_pci_instr_countx_s cn50xx;
	struct cvmx_pci_instr_countx_s cn58xx;
	struct cvmx_pci_instr_countx_s cn58xxp1;
};

union cvmx_pci_int_enb {
	uint64_t u64;
	struct cvmx_pci_int_enb_s {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t idtime1:1;
		uint64_t idtime0:1;
		uint64_t idcnt1:1;
		uint64_t idcnt0:1;
		uint64_t iptime3:1;
		uint64_t iptime2:1;
		uint64_t iptime1:1;
		uint64_t iptime0:1;
		uint64_t ipcnt3:1;
		uint64_t ipcnt2:1;
		uint64_t ipcnt1:1;
		uint64_t ipcnt0:1;
		uint64_t irsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t idperr:1;
		uint64_t iaperr:1;
		uint64_t iserr:1;
		uint64_t itsr_abt:1;
		uint64_t imsc_msg:1;
		uint64_t imsi_mabt:1;
		uint64_t imsi_tabt:1;
		uint64_t imsi_per:1;
		uint64_t imr_tto:1;
		uint64_t imr_abt:1;
		uint64_t itr_abt:1;
		uint64_t imr_wtto:1;
		uint64_t imr_wabt:1;
		uint64_t itr_wabt:1;
	} s;
	struct cvmx_pci_int_enb_cn30xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t idtime1:1;
		uint64_t idtime0:1;
		uint64_t idcnt1:1;
		uint64_t idcnt0:1;
		uint64_t reserved_22_24:3;
		uint64_t iptime0:1;
		uint64_t reserved_18_20:3;
		uint64_t ipcnt0:1;
		uint64_t irsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t idperr:1;
		uint64_t iaperr:1;
		uint64_t iserr:1;
		uint64_t itsr_abt:1;
		uint64_t imsc_msg:1;
		uint64_t imsi_mabt:1;
		uint64_t imsi_tabt:1;
		uint64_t imsi_per:1;
		uint64_t imr_tto:1;
		uint64_t imr_abt:1;
		uint64_t itr_abt:1;
		uint64_t imr_wtto:1;
		uint64_t imr_wabt:1;
		uint64_t itr_wabt:1;
	} cn30xx;
	struct cvmx_pci_int_enb_cn31xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t idtime1:1;
		uint64_t idtime0:1;
		uint64_t idcnt1:1;
		uint64_t idcnt0:1;
		uint64_t reserved_23_24:2;
		uint64_t iptime1:1;
		uint64_t iptime0:1;
		uint64_t reserved_19_20:2;
		uint64_t ipcnt1:1;
		uint64_t ipcnt0:1;
		uint64_t irsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t idperr:1;
		uint64_t iaperr:1;
		uint64_t iserr:1;
		uint64_t itsr_abt:1;
		uint64_t imsc_msg:1;
		uint64_t imsi_mabt:1;
		uint64_t imsi_tabt:1;
		uint64_t imsi_per:1;
		uint64_t imr_tto:1;
		uint64_t imr_abt:1;
		uint64_t itr_abt:1;
		uint64_t imr_wtto:1;
		uint64_t imr_wabt:1;
		uint64_t itr_wabt:1;
	} cn31xx;
	struct cvmx_pci_int_enb_s cn38xx;
	struct cvmx_pci_int_enb_s cn38xxp2;
	struct cvmx_pci_int_enb_cn31xx cn50xx;
	struct cvmx_pci_int_enb_s cn58xx;
	struct cvmx_pci_int_enb_s cn58xxp1;
};

union cvmx_pci_int_enb2 {
	uint64_t u64;
	struct cvmx_pci_int_enb2_s {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t rdtime1:1;
		uint64_t rdtime0:1;
		uint64_t rdcnt1:1;
		uint64_t rdcnt0:1;
		uint64_t rptime3:1;
		uint64_t rptime2:1;
		uint64_t rptime1:1;
		uint64_t rptime0:1;
		uint64_t rpcnt3:1;
		uint64_t rpcnt2:1;
		uint64_t rpcnt1:1;
		uint64_t rpcnt0:1;
		uint64_t rrsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t rdperr:1;
		uint64_t raperr:1;
		uint64_t rserr:1;
		uint64_t rtsr_abt:1;
		uint64_t rmsc_msg:1;
		uint64_t rmsi_mabt:1;
		uint64_t rmsi_tabt:1;
		uint64_t rmsi_per:1;
		uint64_t rmr_tto:1;
		uint64_t rmr_abt:1;
		uint64_t rtr_abt:1;
		uint64_t rmr_wtto:1;
		uint64_t rmr_wabt:1;
		uint64_t rtr_wabt:1;
	} s;
	struct cvmx_pci_int_enb2_cn30xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t rdtime1:1;
		uint64_t rdtime0:1;
		uint64_t rdcnt1:1;
		uint64_t rdcnt0:1;
		uint64_t reserved_22_24:3;
		uint64_t rptime0:1;
		uint64_t reserved_18_20:3;
		uint64_t rpcnt0:1;
		uint64_t rrsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t rdperr:1;
		uint64_t raperr:1;
		uint64_t rserr:1;
		uint64_t rtsr_abt:1;
		uint64_t rmsc_msg:1;
		uint64_t rmsi_mabt:1;
		uint64_t rmsi_tabt:1;
		uint64_t rmsi_per:1;
		uint64_t rmr_tto:1;
		uint64_t rmr_abt:1;
		uint64_t rtr_abt:1;
		uint64_t rmr_wtto:1;
		uint64_t rmr_wabt:1;
		uint64_t rtr_wabt:1;
	} cn30xx;
	struct cvmx_pci_int_enb2_cn31xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t rdtime1:1;
		uint64_t rdtime0:1;
		uint64_t rdcnt1:1;
		uint64_t rdcnt0:1;
		uint64_t reserved_23_24:2;
		uint64_t rptime1:1;
		uint64_t rptime0:1;
		uint64_t reserved_19_20:2;
		uint64_t rpcnt1:1;
		uint64_t rpcnt0:1;
		uint64_t rrsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t rdperr:1;
		uint64_t raperr:1;
		uint64_t rserr:1;
		uint64_t rtsr_abt:1;
		uint64_t rmsc_msg:1;
		uint64_t rmsi_mabt:1;
		uint64_t rmsi_tabt:1;
		uint64_t rmsi_per:1;
		uint64_t rmr_tto:1;
		uint64_t rmr_abt:1;
		uint64_t rtr_abt:1;
		uint64_t rmr_wtto:1;
		uint64_t rmr_wabt:1;
		uint64_t rtr_wabt:1;
	} cn31xx;
	struct cvmx_pci_int_enb2_s cn38xx;
	struct cvmx_pci_int_enb2_s cn38xxp2;
	struct cvmx_pci_int_enb2_cn31xx cn50xx;
	struct cvmx_pci_int_enb2_s cn58xx;
	struct cvmx_pci_int_enb2_s cn58xxp1;
};

union cvmx_pci_int_sum {
	uint64_t u64;
	struct cvmx_pci_int_sum_s {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t dtime1:1;
		uint64_t dtime0:1;
		uint64_t dcnt1:1;
		uint64_t dcnt0:1;
		uint64_t ptime3:1;
		uint64_t ptime2:1;
		uint64_t ptime1:1;
		uint64_t ptime0:1;
		uint64_t pcnt3:1;
		uint64_t pcnt2:1;
		uint64_t pcnt1:1;
		uint64_t pcnt0:1;
		uint64_t rsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t dperr:1;
		uint64_t aperr:1;
		uint64_t serr:1;
		uint64_t tsr_abt:1;
		uint64_t msc_msg:1;
		uint64_t msi_mabt:1;
		uint64_t msi_tabt:1;
		uint64_t msi_per:1;
		uint64_t mr_tto:1;
		uint64_t mr_abt:1;
		uint64_t tr_abt:1;
		uint64_t mr_wtto:1;
		uint64_t mr_wabt:1;
		uint64_t tr_wabt:1;
	} s;
	struct cvmx_pci_int_sum_cn30xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t dtime1:1;
		uint64_t dtime0:1;
		uint64_t dcnt1:1;
		uint64_t dcnt0:1;
		uint64_t reserved_22_24:3;
		uint64_t ptime0:1;
		uint64_t reserved_18_20:3;
		uint64_t pcnt0:1;
		uint64_t rsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t dperr:1;
		uint64_t aperr:1;
		uint64_t serr:1;
		uint64_t tsr_abt:1;
		uint64_t msc_msg:1;
		uint64_t msi_mabt:1;
		uint64_t msi_tabt:1;
		uint64_t msi_per:1;
		uint64_t mr_tto:1;
		uint64_t mr_abt:1;
		uint64_t tr_abt:1;
		uint64_t mr_wtto:1;
		uint64_t mr_wabt:1;
		uint64_t tr_wabt:1;
	} cn30xx;
	struct cvmx_pci_int_sum_cn31xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t dtime1:1;
		uint64_t dtime0:1;
		uint64_t dcnt1:1;
		uint64_t dcnt0:1;
		uint64_t reserved_23_24:2;
		uint64_t ptime1:1;
		uint64_t ptime0:1;
		uint64_t reserved_19_20:2;
		uint64_t pcnt1:1;
		uint64_t pcnt0:1;
		uint64_t rsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t dperr:1;
		uint64_t aperr:1;
		uint64_t serr:1;
		uint64_t tsr_abt:1;
		uint64_t msc_msg:1;
		uint64_t msi_mabt:1;
		uint64_t msi_tabt:1;
		uint64_t msi_per:1;
		uint64_t mr_tto:1;
		uint64_t mr_abt:1;
		uint64_t tr_abt:1;
		uint64_t mr_wtto:1;
		uint64_t mr_wabt:1;
		uint64_t tr_wabt:1;
	} cn31xx;
	struct cvmx_pci_int_sum_s cn38xx;
	struct cvmx_pci_int_sum_s cn38xxp2;
	struct cvmx_pci_int_sum_cn31xx cn50xx;
	struct cvmx_pci_int_sum_s cn58xx;
	struct cvmx_pci_int_sum_s cn58xxp1;
};

union cvmx_pci_int_sum2 {
	uint64_t u64;
	struct cvmx_pci_int_sum2_s {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t dtime1:1;
		uint64_t dtime0:1;
		uint64_t dcnt1:1;
		uint64_t dcnt0:1;
		uint64_t ptime3:1;
		uint64_t ptime2:1;
		uint64_t ptime1:1;
		uint64_t ptime0:1;
		uint64_t pcnt3:1;
		uint64_t pcnt2:1;
		uint64_t pcnt1:1;
		uint64_t pcnt0:1;
		uint64_t rsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t dperr:1;
		uint64_t aperr:1;
		uint64_t serr:1;
		uint64_t tsr_abt:1;
		uint64_t msc_msg:1;
		uint64_t msi_mabt:1;
		uint64_t msi_tabt:1;
		uint64_t msi_per:1;
		uint64_t mr_tto:1;
		uint64_t mr_abt:1;
		uint64_t tr_abt:1;
		uint64_t mr_wtto:1;
		uint64_t mr_wabt:1;
		uint64_t tr_wabt:1;
	} s;
	struct cvmx_pci_int_sum2_cn30xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t dtime1:1;
		uint64_t dtime0:1;
		uint64_t dcnt1:1;
		uint64_t dcnt0:1;
		uint64_t reserved_22_24:3;
		uint64_t ptime0:1;
		uint64_t reserved_18_20:3;
		uint64_t pcnt0:1;
		uint64_t rsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t dperr:1;
		uint64_t aperr:1;
		uint64_t serr:1;
		uint64_t tsr_abt:1;
		uint64_t msc_msg:1;
		uint64_t msi_mabt:1;
		uint64_t msi_tabt:1;
		uint64_t msi_per:1;
		uint64_t mr_tto:1;
		uint64_t mr_abt:1;
		uint64_t tr_abt:1;
		uint64_t mr_wtto:1;
		uint64_t mr_wabt:1;
		uint64_t tr_wabt:1;
	} cn30xx;
	struct cvmx_pci_int_sum2_cn31xx {
		uint64_t reserved_34_63:30;
		uint64_t ill_rd:1;
		uint64_t ill_wr:1;
		uint64_t win_wr:1;
		uint64_t dma1_fi:1;
		uint64_t dma0_fi:1;
		uint64_t dtime1:1;
		uint64_t dtime0:1;
		uint64_t dcnt1:1;
		uint64_t dcnt0:1;
		uint64_t reserved_23_24:2;
		uint64_t ptime1:1;
		uint64_t ptime0:1;
		uint64_t reserved_19_20:2;
		uint64_t pcnt1:1;
		uint64_t pcnt0:1;
		uint64_t rsl_int:1;
		uint64_t ill_rrd:1;
		uint64_t ill_rwr:1;
		uint64_t dperr:1;
		uint64_t aperr:1;
		uint64_t serr:1;
		uint64_t tsr_abt:1;
		uint64_t msc_msg:1;
		uint64_t msi_mabt:1;
		uint64_t msi_tabt:1;
		uint64_t msi_per:1;
		uint64_t mr_tto:1;
		uint64_t mr_abt:1;
		uint64_t tr_abt:1;
		uint64_t mr_wtto:1;
		uint64_t mr_wabt:1;
		uint64_t tr_wabt:1;
	} cn31xx;
	struct cvmx_pci_int_sum2_s cn38xx;
	struct cvmx_pci_int_sum2_s cn38xxp2;
	struct cvmx_pci_int_sum2_cn31xx cn50xx;
	struct cvmx_pci_int_sum2_s cn58xx;
	struct cvmx_pci_int_sum2_s cn58xxp1;
};

union cvmx_pci_msi_rcv {
	uint32_t u32;
	struct cvmx_pci_msi_rcv_s {
		uint32_t reserved_6_31:26;
		uint32_t intr:6;
	} s;
	struct cvmx_pci_msi_rcv_s cn30xx;
	struct cvmx_pci_msi_rcv_s cn31xx;
	struct cvmx_pci_msi_rcv_s cn38xx;
	struct cvmx_pci_msi_rcv_s cn38xxp2;
	struct cvmx_pci_msi_rcv_s cn50xx;
	struct cvmx_pci_msi_rcv_s cn58xx;
	struct cvmx_pci_msi_rcv_s cn58xxp1;
};

union cvmx_pci_pkt_creditsx {
	uint32_t u32;
	struct cvmx_pci_pkt_creditsx_s {
		uint32_t pkt_cnt:16;
		uint32_t ptr_cnt:16;
	} s;
	struct cvmx_pci_pkt_creditsx_s cn30xx;
	struct cvmx_pci_pkt_creditsx_s cn31xx;
	struct cvmx_pci_pkt_creditsx_s cn38xx;
	struct cvmx_pci_pkt_creditsx_s cn38xxp2;
	struct cvmx_pci_pkt_creditsx_s cn50xx;
	struct cvmx_pci_pkt_creditsx_s cn58xx;
	struct cvmx_pci_pkt_creditsx_s cn58xxp1;
};

union cvmx_pci_pkts_sentx {
	uint32_t u32;
	struct cvmx_pci_pkts_sentx_s {
		uint32_t pkt_cnt:32;
	} s;
	struct cvmx_pci_pkts_sentx_s cn30xx;
	struct cvmx_pci_pkts_sentx_s cn31xx;
	struct cvmx_pci_pkts_sentx_s cn38xx;
	struct cvmx_pci_pkts_sentx_s cn38xxp2;
	struct cvmx_pci_pkts_sentx_s cn50xx;
	struct cvmx_pci_pkts_sentx_s cn58xx;
	struct cvmx_pci_pkts_sentx_s cn58xxp1;
};

union cvmx_pci_pkts_sent_int_levx {
	uint32_t u32;
	struct cvmx_pci_pkts_sent_int_levx_s {
		uint32_t pkt_cnt:32;
	} s;
	struct cvmx_pci_pkts_sent_int_levx_s cn30xx;
	struct cvmx_pci_pkts_sent_int_levx_s cn31xx;
	struct cvmx_pci_pkts_sent_int_levx_s cn38xx;
	struct cvmx_pci_pkts_sent_int_levx_s cn38xxp2;
	struct cvmx_pci_pkts_sent_int_levx_s cn50xx;
	struct cvmx_pci_pkts_sent_int_levx_s cn58xx;
	struct cvmx_pci_pkts_sent_int_levx_s cn58xxp1;
};

union cvmx_pci_pkts_sent_timex {
	uint32_t u32;
	struct cvmx_pci_pkts_sent_timex_s {
		uint32_t pkt_time:32;
	} s;
	struct cvmx_pci_pkts_sent_timex_s cn30xx;
	struct cvmx_pci_pkts_sent_timex_s cn31xx;
	struct cvmx_pci_pkts_sent_timex_s cn38xx;
	struct cvmx_pci_pkts_sent_timex_s cn38xxp2;
	struct cvmx_pci_pkts_sent_timex_s cn50xx;
	struct cvmx_pci_pkts_sent_timex_s cn58xx;
	struct cvmx_pci_pkts_sent_timex_s cn58xxp1;
};

union cvmx_pci_read_cmd_6 {
	uint32_t u32;
	struct cvmx_pci_read_cmd_6_s {
		uint32_t reserved_9_31:23;
		uint32_t min_data:6;
		uint32_t prefetch:3;
	} s;
	struct cvmx_pci_read_cmd_6_s cn30xx;
	struct cvmx_pci_read_cmd_6_s cn31xx;
	struct cvmx_pci_read_cmd_6_s cn38xx;
	struct cvmx_pci_read_cmd_6_s cn38xxp2;
	struct cvmx_pci_read_cmd_6_s cn50xx;
	struct cvmx_pci_read_cmd_6_s cn58xx;
	struct cvmx_pci_read_cmd_6_s cn58xxp1;
};

union cvmx_pci_read_cmd_c {
	uint32_t u32;
	struct cvmx_pci_read_cmd_c_s {
		uint32_t reserved_9_31:23;
		uint32_t min_data:6;
		uint32_t prefetch:3;
	} s;
	struct cvmx_pci_read_cmd_c_s cn30xx;
	struct cvmx_pci_read_cmd_c_s cn31xx;
	struct cvmx_pci_read_cmd_c_s cn38xx;
	struct cvmx_pci_read_cmd_c_s cn38xxp2;
	struct cvmx_pci_read_cmd_c_s cn50xx;
	struct cvmx_pci_read_cmd_c_s cn58xx;
	struct cvmx_pci_read_cmd_c_s cn58xxp1;
};

union cvmx_pci_read_cmd_e {
	uint32_t u32;
	struct cvmx_pci_read_cmd_e_s {
		uint32_t reserved_9_31:23;
		uint32_t min_data:6;
		uint32_t prefetch:3;
	} s;
	struct cvmx_pci_read_cmd_e_s cn30xx;
	struct cvmx_pci_read_cmd_e_s cn31xx;
	struct cvmx_pci_read_cmd_e_s cn38xx;
	struct cvmx_pci_read_cmd_e_s cn38xxp2;
	struct cvmx_pci_read_cmd_e_s cn50xx;
	struct cvmx_pci_read_cmd_e_s cn58xx;
	struct cvmx_pci_read_cmd_e_s cn58xxp1;
};

union cvmx_pci_read_timeout {
	uint64_t u64;
	struct cvmx_pci_read_timeout_s {
		uint64_t reserved_32_63:32;
		uint64_t enb:1;
		uint64_t cnt:31;
	} s;
	struct cvmx_pci_read_timeout_s cn30xx;
	struct cvmx_pci_read_timeout_s cn31xx;
	struct cvmx_pci_read_timeout_s cn38xx;
	struct cvmx_pci_read_timeout_s cn38xxp2;
	struct cvmx_pci_read_timeout_s cn50xx;
	struct cvmx_pci_read_timeout_s cn58xx;
	struct cvmx_pci_read_timeout_s cn58xxp1;
};

union cvmx_pci_scm_reg {
	uint64_t u64;
	struct cvmx_pci_scm_reg_s {
		uint64_t reserved_32_63:32;
		uint64_t scm:32;
	} s;
	struct cvmx_pci_scm_reg_s cn30xx;
	struct cvmx_pci_scm_reg_s cn31xx;
	struct cvmx_pci_scm_reg_s cn38xx;
	struct cvmx_pci_scm_reg_s cn38xxp2;
	struct cvmx_pci_scm_reg_s cn50xx;
	struct cvmx_pci_scm_reg_s cn58xx;
	struct cvmx_pci_scm_reg_s cn58xxp1;
};

union cvmx_pci_tsr_reg {
	uint64_t u64;
	struct cvmx_pci_tsr_reg_s {
		uint64_t reserved_36_63:28;
		uint64_t tsr:36;
	} s;
	struct cvmx_pci_tsr_reg_s cn30xx;
	struct cvmx_pci_tsr_reg_s cn31xx;
	struct cvmx_pci_tsr_reg_s cn38xx;
	struct cvmx_pci_tsr_reg_s cn38xxp2;
	struct cvmx_pci_tsr_reg_s cn50xx;
	struct cvmx_pci_tsr_reg_s cn58xx;
	struct cvmx_pci_tsr_reg_s cn58xxp1;
};

union cvmx_pci_win_rd_addr {
	uint64_t u64;
	struct cvmx_pci_win_rd_addr_s {
		uint64_t reserved_49_63:15;
		uint64_t iobit:1;
		uint64_t reserved_0_47:48;
	} s;
	struct cvmx_pci_win_rd_addr_cn30xx {
		uint64_t reserved_49_63:15;
		uint64_t iobit:1;
		uint64_t rd_addr:46;
		uint64_t reserved_0_1:2;
	} cn30xx;
	struct cvmx_pci_win_rd_addr_cn30xx cn31xx;
	struct cvmx_pci_win_rd_addr_cn38xx {
		uint64_t reserved_49_63:15;
		uint64_t iobit:1;
		uint64_t rd_addr:45;
		uint64_t reserved_0_2:3;
	} cn38xx;
	struct cvmx_pci_win_rd_addr_cn38xx cn38xxp2;
	struct cvmx_pci_win_rd_addr_cn30xx cn50xx;
	struct cvmx_pci_win_rd_addr_cn38xx cn58xx;
	struct cvmx_pci_win_rd_addr_cn38xx cn58xxp1;
};

union cvmx_pci_win_rd_data {
	uint64_t u64;
	struct cvmx_pci_win_rd_data_s {
		uint64_t rd_data:64;
	} s;
	struct cvmx_pci_win_rd_data_s cn30xx;
	struct cvmx_pci_win_rd_data_s cn31xx;
	struct cvmx_pci_win_rd_data_s cn38xx;
	struct cvmx_pci_win_rd_data_s cn38xxp2;
	struct cvmx_pci_win_rd_data_s cn50xx;
	struct cvmx_pci_win_rd_data_s cn58xx;
	struct cvmx_pci_win_rd_data_s cn58xxp1;
};

union cvmx_pci_win_wr_addr {
	uint64_t u64;
	struct cvmx_pci_win_wr_addr_s {
		uint64_t reserved_49_63:15;
		uint64_t iobit:1;
		uint64_t wr_addr:45;
		uint64_t reserved_0_2:3;
	} s;
	struct cvmx_pci_win_wr_addr_s cn30xx;
	struct cvmx_pci_win_wr_addr_s cn31xx;
	struct cvmx_pci_win_wr_addr_s cn38xx;
	struct cvmx_pci_win_wr_addr_s cn38xxp2;
	struct cvmx_pci_win_wr_addr_s cn50xx;
	struct cvmx_pci_win_wr_addr_s cn58xx;
	struct cvmx_pci_win_wr_addr_s cn58xxp1;
};

union cvmx_pci_win_wr_data {
	uint64_t u64;
	struct cvmx_pci_win_wr_data_s {
		uint64_t wr_data:64;
	} s;
	struct cvmx_pci_win_wr_data_s cn30xx;
	struct cvmx_pci_win_wr_data_s cn31xx;
	struct cvmx_pci_win_wr_data_s cn38xx;
	struct cvmx_pci_win_wr_data_s cn38xxp2;
	struct cvmx_pci_win_wr_data_s cn50xx;
	struct cvmx_pci_win_wr_data_s cn58xx;
	struct cvmx_pci_win_wr_data_s cn58xxp1;
};

union cvmx_pci_win_wr_mask {
	uint64_t u64;
	struct cvmx_pci_win_wr_mask_s {
		uint64_t reserved_8_63:56;
		uint64_t wr_mask:8;
	} s;
	struct cvmx_pci_win_wr_mask_s cn30xx;
	struct cvmx_pci_win_wr_mask_s cn31xx;
	struct cvmx_pci_win_wr_mask_s cn38xx;
	struct cvmx_pci_win_wr_mask_s cn38xxp2;
	struct cvmx_pci_win_wr_mask_s cn50xx;
	struct cvmx_pci_win_wr_mask_s cn58xx;
	struct cvmx_pci_win_wr_mask_s cn58xxp1;
};

#endif