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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 Maxime Bizon <mbizon@freebox.fr>
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/bootmem.h>
#include <linux/ioport.h>
#include <linux/pm.h>
#include <asm/bootinfo.h>
#include <asm/time.h>
#include <asm/reboot.h>
#include <asm/cacheflush.h>
#include <bcm63xx_board.h>
#include <bcm63xx_cpu.h>
#include <bcm63xx_regs.h>
#include <bcm63xx_io.h>

void bcm63xx_machine_halt(void)
{
	printk(KERN_INFO "System halted\n");
	while (1)
		;
}

static void bcm6348_a1_reboot(void)
{
	u32 reg;

	/* soft reset all blocks */
	printk(KERN_INFO "soft-reseting all blocks ...\n");
	reg = bcm_perf_readl(PERF_SOFTRESET_REG);
	reg &= ~SOFTRESET_6348_ALL;
	bcm_perf_writel(reg, PERF_SOFTRESET_REG);
	mdelay(10);

	reg = bcm_perf_readl(PERF_SOFTRESET_REG);
	reg |= SOFTRESET_6348_ALL;
	bcm_perf_writel(reg, PERF_SOFTRESET_REG);
	mdelay(10);

	/* Jump to the power on address. */
	printk(KERN_INFO "jumping to reset vector.\n");
	/* set high vectors (base at 0xbfc00000 */
	set_c0_status(ST0_BEV | ST0_ERL);
	/* run uncached in kseg0 */
	change_c0_config(CONF_CM_CMASK, CONF_CM_UNCACHED);
	__flush_cache_all();
	/* remove all wired TLB entries */
	write_c0_wired(0);
	__asm__ __volatile__(
		"jr\t%0"
		:
		: "r" (0xbfc00000));
	while (1)
		;
}

void bcm63xx_machine_reboot(void)
{
	u32 reg;

	/* mask and clear all external irq */
	reg = bcm_perf_readl(PERF_EXTIRQ_CFG_REG);
	reg &= ~EXTIRQ_CFG_MASK_ALL;
	reg |= EXTIRQ_CFG_CLEAR_ALL;
	bcm_perf_writel(reg, PERF_EXTIRQ_CFG_REG);

	if (BCMCPU_IS_6348() && (bcm63xx_get_cpu_rev() == 0xa1))
		bcm6348_a1_reboot();

	printk(KERN_INFO "triggering watchdog soft-reset...\n");
	reg = bcm_perf_readl(PERF_SYS_PLL_CTL_REG);
	reg |= SYS_PLL_SOFT_RESET;
	bcm_perf_writel(reg, PERF_SYS_PLL_CTL_REG);
	while (1)
		;
}

static void __bcm63xx_machine_reboot(char *p)
{
	bcm63xx_machine_reboot();
}

/*
 * return system type in /proc/cpuinfo
 */
const char *get_system_type(void)
{
	static char buf[128];
	snprintf(buf, sizeof(buf), "bcm63xx/%s (0x%04x/0x%04X)",
		 board_get_name(),
		 bcm63xx_get_cpu_id(), bcm63xx_get_cpu_rev());
	return buf;
}

void __init plat_time_init(void)
{
	mips_hpt_frequency = bcm63xx_get_cpu_freq() / 2;
}

void __init plat_mem_setup(void)
{
	add_memory_region(0, bcm63xx_get_memory_size(), BOOT_MEM_RAM);

	_machine_halt = bcm63xx_machine_halt;
	_machine_restart = __bcm63xx_machine_reboot;
	pm_power_off = bcm63xx_machine_halt;

	set_io_port_base(0);
	ioport_resource.start = 0;
	ioport_resource.end = ~0;

	board_setup();
}

int __init bcm63xx_register_devices(void)
{
	return board_register_devices();
}

arch_initcall(bcm63xx_register_devices);
