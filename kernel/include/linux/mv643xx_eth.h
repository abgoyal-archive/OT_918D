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
 * MV-643XX ethernet platform device data definition file.
 */

#ifndef __LINUX_MV643XX_ETH_H
#define __LINUX_MV643XX_ETH_H

#include <linux/mbus.h>

#define MV643XX_ETH_SHARED_NAME		"mv643xx_eth"
#define MV643XX_ETH_NAME		"mv643xx_eth_port"
#define MV643XX_ETH_SHARED_REGS		0x2000
#define MV643XX_ETH_SHARED_REGS_SIZE	0x2000
#define MV643XX_ETH_BAR_4		0x2220
#define MV643XX_ETH_SIZE_REG_4		0x2224
#define MV643XX_ETH_BASE_ADDR_ENABLE_REG	0x2290

struct mv643xx_eth_shared_platform_data {
	struct mbus_dram_target_info	*dram;
	struct platform_device	*shared_smi;
	unsigned int		t_clk;
	/*
	 * Max packet size for Tx IP/Layer 4 checksum, when set to 0, default
	 * limit of 9KiB will be used.
	 */
	int			tx_csum_limit;
};

#define MV643XX_ETH_PHY_ADDR_DEFAULT	0
#define MV643XX_ETH_PHY_ADDR(x)		(0x80 | (x))
#define MV643XX_ETH_PHY_NONE		0xff

struct mv643xx_eth_platform_data {
	/*
	 * Pointer back to our parent instance, and our port number.
	 */
	struct platform_device	*shared;
	int			port_number;

	/*
	 * Whether a PHY is present, and if yes, at which address.
	 */
	int			phy_addr;

	/*
	 * Use this MAC address if it is valid, overriding the
	 * address that is already in the hardware.
	 */
	u8			mac_addr[6];

	/*
	 * If speed is 0, autonegotiation is enabled.
	 *   Valid values for speed: 0, SPEED_10, SPEED_100, SPEED_1000.
	 *   Valid values for duplex: DUPLEX_HALF, DUPLEX_FULL.
	 */
	int			speed;
	int			duplex;

	/*
	 * How many RX/TX queues to use.
	 */
	int			rx_queue_count;
	int			tx_queue_count;

	/*
	 * Override default RX/TX queue sizes if nonzero.
	 */
	int			rx_queue_size;
	int			tx_queue_size;

	/*
	 * Use on-chip SRAM for RX/TX descriptors if size is nonzero
	 * and sufficient to contain all descriptors for the requested
	 * ring sizes.
	 */
	unsigned long		rx_sram_addr;
	int			rx_sram_size;
	unsigned long		tx_sram_addr;
	int			tx_sram_size;
};


#endif
