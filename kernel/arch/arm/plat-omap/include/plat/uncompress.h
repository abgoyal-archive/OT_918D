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
 * arch/arm/plat-omap/include/mach/uncompress.h
 *
 * Serial port stubs for kernel decompress status messages
 *
 * Initially based on:
 * linux-2.4.15-rmk1-dsplinux1.6/arch/arm/plat-omap/include/mach1510/uncompress.h
 * Copyright (C) 2000 RidgeRun, Inc.
 * Author: Greg Lonnon <glonnon@ridgerun.com>
 *
 * Rewritten by:
 * Author: <source@mvista.com>
 * 2004 (c) MontaVista Software, Inc.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/types.h>
#include <linux/serial_reg.h>

#include <asm/memory.h>
#include <asm/mach-types.h>

#include <plat/serial.h>

static volatile u8 *uart_base;
static int uart_shift;

/*
 * Store the DEBUG_LL uart number into memory.
 * See also debug-macro.S, and serial.c for related code.
 */
static void set_omap_uart_info(unsigned char port)
{
	*(volatile u32 *)OMAP_UART_INFO = port;
}

static void putc(int c)
{
	if (!uart_base)
		return;

	while (!(uart_base[UART_LSR << uart_shift] & UART_LSR_THRE))
		barrier();
	uart_base[UART_TX << uart_shift] = c;
}

static inline void flush(void)
{
}

/*
 * Macros to configure UART1 and debug UART
 */
#define _DEBUG_LL_ENTRY(mach, dbg_uart, dbg_shft, dbg_id)		\
	if (machine_is_##mach()) {					\
		uart_base = (volatile u8 *)(dbg_uart);			\
		uart_shift = (dbg_shft);				\
		port = (dbg_id);					\
		set_omap_uart_info(port);				\
		break;							\
	}

#define DEBUG_LL_OMAP7XX(p, mach)					\
	_DEBUG_LL_ENTRY(mach, OMAP1_UART##p##_BASE, OMAP7XX_PORT_SHIFT,	\
		OMAP1UART##p)

#define DEBUG_LL_OMAP1(p, mach)						\
	_DEBUG_LL_ENTRY(mach, OMAP1_UART##p##_BASE, OMAP_PORT_SHIFT,	\
		OMAP1UART##p)

#define DEBUG_LL_OMAP2(p, mach)						\
	_DEBUG_LL_ENTRY(mach, OMAP2_UART##p##_BASE, OMAP_PORT_SHIFT,	\
		OMAP2UART##p)

#define DEBUG_LL_OMAP3(p, mach)						\
	_DEBUG_LL_ENTRY(mach, OMAP3_UART##p##_BASE, OMAP_PORT_SHIFT,	\
		OMAP3UART##p)

#define DEBUG_LL_OMAP4(p, mach)						\
	_DEBUG_LL_ENTRY(mach, OMAP4_UART##p##_BASE, OMAP_PORT_SHIFT,	\
		OMAP4UART##p)

/* Zoom2/3 shift is different for UART1 and external port */
#define DEBUG_LL_ZOOM(mach)						\
	_DEBUG_LL_ENTRY(mach, ZOOM_UART_BASE, ZOOM_PORT_SHIFT, ZOOM_UART)

static inline void __arch_decomp_setup(unsigned long arch_id)
{
	int port = 0;

	/*
	 * Initialize the port based on the machine ID from the bootloader.
	 * Note that we're using macros here instead of switch statement
	 * as machine_is functions are optimized out for the boards that
	 * are not selected.
	 */
	do {
		/* omap7xx/8xx based boards using UART1 with shift 0 */
		DEBUG_LL_OMAP7XX(1, herald);
		DEBUG_LL_OMAP7XX(1, omap_perseus2);

		/* omap15xx/16xx based boards using UART1 */
		DEBUG_LL_OMAP1(1, ams_delta);
		DEBUG_LL_OMAP1(1, nokia770);
		DEBUG_LL_OMAP1(1, omap_h2);
		DEBUG_LL_OMAP1(1, omap_h3);
		DEBUG_LL_OMAP1(1, omap_innovator);
		DEBUG_LL_OMAP1(1, omap_osk);
		DEBUG_LL_OMAP1(1, omap_palmte);
		DEBUG_LL_OMAP1(1, omap_palmz71);

		/* omap15xx/16xx based boards using UART2 */
		DEBUG_LL_OMAP1(2, omap_palmtt);

		/* omap15xx/16xx based boards using UART3 */
		DEBUG_LL_OMAP1(3, sx1);

		/* omap2 based boards using UART1 */
		DEBUG_LL_OMAP2(1, omap2evm);
		DEBUG_LL_OMAP2(1, omap_2430sdp);
		DEBUG_LL_OMAP2(1, omap_apollon);
		DEBUG_LL_OMAP2(1, omap_h4);

		/* omap2 based boards using UART3 */
		DEBUG_LL_OMAP2(3, nokia_n800);
		DEBUG_LL_OMAP2(3, nokia_n810);
		DEBUG_LL_OMAP2(3, nokia_n810_wimax);

		/* omap3 based boards using UART1 */
		DEBUG_LL_OMAP2(1, omap3evm);
		DEBUG_LL_OMAP3(1, omap_3430sdp);
		DEBUG_LL_OMAP3(1, omap_3630sdp);

		/* omap3 based boards using UART3 */
		DEBUG_LL_OMAP3(3, cm_t35);
		DEBUG_LL_OMAP3(3, igep0020);
		DEBUG_LL_OMAP3(3, nokia_rx51);
		DEBUG_LL_OMAP3(3, omap3517evm);
		DEBUG_LL_OMAP3(3, omap3_beagle);
		DEBUG_LL_OMAP3(3, omap3_pandora);
		DEBUG_LL_OMAP3(3, omap_ldp);
		DEBUG_LL_OMAP3(3, overo);
		DEBUG_LL_OMAP3(3, touchbook);

		/* omap4 based boards using UART3 */
		DEBUG_LL_OMAP4(3, omap_4430sdp);

		/* zoom2/3 external uart */
		DEBUG_LL_ZOOM(omap_zoom2);
		DEBUG_LL_ZOOM(omap_zoom3);

	} while (0);
}

#define arch_decomp_setup()	__arch_decomp_setup(arch_id)

/*
 * nothing to do
 */
#define arch_decomp_wdog()
