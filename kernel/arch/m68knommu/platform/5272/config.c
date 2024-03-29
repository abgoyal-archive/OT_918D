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

/***************************************************************************/

/*
 *	linux/arch/m68knommu/platform/5272/config.c
 *
 *	Copyright (C) 1999-2002, Greg Ungerer (gerg@snapgear.com)
 *	Copyright (C) 2001-2002, SnapGear Inc. (www.snapgear.com)
 */

/***************************************************************************/

#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/init.h>
#include <linux/io.h>
#include <asm/machdep.h>
#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#include <asm/mcfuart.h>

/***************************************************************************/

/*
 *	Some platforms need software versions of the GPIO data registers.
 */
unsigned short ppdata;
unsigned char ledbank = 0xff;

/***************************************************************************/

static struct mcf_platform_uart m5272_uart_platform[] = {
	{
		.mapbase	= MCF_MBAR + MCFUART_BASE1,
		.irq		= MCF_IRQ_UART1,
	},
	{
		.mapbase 	= MCF_MBAR + MCFUART_BASE2,
		.irq		= MCF_IRQ_UART2,
	},
	{ },
};

static struct platform_device m5272_uart = {
	.name			= "mcfuart",
	.id			= 0,
	.dev.platform_data	= m5272_uart_platform,
};

static struct resource m5272_fec_resources[] = {
	{
		.start		= MCF_MBAR + 0x840,
		.end		= MCF_MBAR + 0x840 + 0x1cf,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= MCF_IRQ_ERX,
		.end		= MCF_IRQ_ERX,
		.flags		= IORESOURCE_IRQ,
	},
	{
		.start		= MCF_IRQ_ETX,
		.end		= MCF_IRQ_ETX,
		.flags		= IORESOURCE_IRQ,
	},
	{
		.start		= MCF_IRQ_ENTC,
		.end		= MCF_IRQ_ENTC,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device m5272_fec = {
	.name			= "fec",
	.id			= 0,
	.num_resources		= ARRAY_SIZE(m5272_fec_resources),
	.resource		= m5272_fec_resources,
};

static struct platform_device *m5272_devices[] __initdata = {
	&m5272_uart,
	&m5272_fec,
};

/***************************************************************************/

static void __init m5272_uart_init_line(int line, int irq)
{
	u32 v;

	if ((line >= 0) && (line < 2)) {
		/* Enable the output lines for the serial ports */
		v = readl(MCF_MBAR + MCFSIM_PBCNT);
		v = (v & ~0x000000ff) | 0x00000055;
		writel(v, MCF_MBAR + MCFSIM_PBCNT);

		v = readl(MCF_MBAR + MCFSIM_PDCNT);
		v = (v & ~0x000003fc) | 0x000002a8;
		writel(v, MCF_MBAR + MCFSIM_PDCNT);
	}
}

static void __init m5272_uarts_init(void)
{
	const int nrlines = ARRAY_SIZE(m5272_uart_platform);
	int line;

	for (line = 0; (line < nrlines); line++)
		m5272_uart_init_line(line, m5272_uart_platform[line].irq);
}

/***************************************************************************/

static void m5272_cpu_reset(void)
{
	local_irq_disable();
	/* Set watchdog to reset, and enabled */
	__raw_writew(0, MCF_MBAR + MCFSIM_WIRR);
	__raw_writew(1, MCF_MBAR + MCFSIM_WRRR);
	__raw_writew(0, MCF_MBAR + MCFSIM_WCR);
	for (;;)
		/* wait for watchdog to timeout */;
}

/***************************************************************************/

void __init config_BSP(char *commandp, int size)
{
#if defined (CONFIG_MOD5272)
	volatile unsigned char	*pivrp;

	/* Set base of device vectors to be 64 */
	pivrp = (volatile unsigned char *) (MCF_MBAR + MCFSIM_PIVR);
	*pivrp = 0x40;
#endif

#if defined(CONFIG_NETtel) || defined(CONFIG_SCALES)
	/* Copy command line from FLASH to local buffer... */
	memcpy(commandp, (char *) 0xf0004000, size);
	commandp[size-1] = 0;
#elif defined(CONFIG_CANCam)
	/* Copy command line from FLASH to local buffer... */
	memcpy(commandp, (char *) 0xf0010000, size);
	commandp[size-1] = 0;
#endif

	mach_reset = m5272_cpu_reset;
}

/***************************************************************************/

static int __init init_BSP(void)
{
	m5272_uarts_init();
	platform_add_devices(m5272_devices, ARRAY_SIZE(m5272_devices));
	return 0;
}

arch_initcall(init_BSP);

/***************************************************************************/
