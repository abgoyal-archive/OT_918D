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
 * Do some checking to make sure things are OK
 *
 * Copyright 2007-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <asm/fixed_code.h>
#include <mach/anomaly.h>
#include <asm/clocks.h>

#ifdef CONFIG_BFIN_KERNEL_CLOCK

# if (CONFIG_VCO_HZ > CONFIG_MAX_VCO_HZ)
#  error "VCO selected is more than maximum value. Please change the VCO multipler"
# endif

# if (CONFIG_SCLK_HZ > CONFIG_MAX_SCLK_HZ)
# error "Sclk value selected is more than maximum. Please select a proper value for SCLK multiplier"
# endif

# if (CONFIG_SCLK_HZ < CONFIG_MIN_SCLK_HZ)
# error "Sclk value selected is less than minimum. Please select a proper value for SCLK multiplier"
# endif

# if (ANOMALY_05000273) && (CONFIG_SCLK_HZ * 2 > CONFIG_CCLK_HZ)
# error "ANOMALY 05000273, please make sure CCLK is at least 2x SCLK"
# endif

# if (CONFIG_SCLK_HZ > CONFIG_CCLK_HZ) && (CONFIG_SCLK_HZ != CONFIG_CLKIN_HZ) && (CONFIG_CCLK_HZ != CONFIG_CLKIN_HZ)
# error "Please select sclk less than cclk"
# endif

#endif /* CONFIG_BFIN_KERNEL_CLOCK */

#if CONFIG_BOOT_LOAD < FIXED_CODE_END
# error "The kernel load address must be after the fixed code section"
#endif

#if (CONFIG_BOOT_LOAD & 0x3)
# error "The kernel load address must be 4 byte aligned"
#endif

/* The entire kernel must be able to make a 24bit pcrel call to start of L1 */
#if ((0xffffffff - L1_CODE_START + 1) + CONFIG_BOOT_LOAD) > 0x1000000
# error "The kernel load address is too high; keep it below 10meg for safety"
#endif

#if ANOMALY_05000448
# error You are using a part with anomaly 05000448, this issue causes random memory read/write failures - that means random crashes.
#endif

/* if 220 exists, can not set External Memory WB and L2 not_cached, either External Memory not_cached and L2 WB */
#if ANOMALY_05000220 && \
	((defined(CONFIG_BFIN_EXTMEM_WRITEBACK) && !defined(CONFIG_BFIN_L2_DCACHEABLE)) || \
	 (!defined(CONFIG_BFIN_EXTMEM_DCACHEABLE) && defined(CONFIG_BFIN_L2_WRITEBACK)))
# error You are exposing Anomaly 220 in this config, either config L2 as Write Through, or make External Memory WB.
#endif

#if ANOMALY_05000475 && \
	(defined(CONFIG_BFIN_EXTMEM_WRITEBACK) || defined(CONFIG_BFIN_L2_WRITEBACK))
# error "Anomaly 475 does not allow you to use Write Back cache with L2 or External Memory"
#endif