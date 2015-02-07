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

#include <linux/module.h>
#include <linux/smp.h>
#include <linux/time.h>
#include <linux/errno.h>
#include <linux/timex.h>
#include <linux/clocksource.h>
#include <asm/io.h>

/* IBM Summit (EXA) Cyclone counter code*/
#define CYCLONE_CBAR_ADDR 0xFEB00CD0
#define CYCLONE_PMCC_OFFSET 0x51A0
#define CYCLONE_MPMC_OFFSET 0x51D0
#define CYCLONE_MPCS_OFFSET 0x51A8
#define CYCLONE_TIMER_FREQ 100000000

int use_cyclone;
void __init cyclone_setup(void)
{
	use_cyclone = 1;
}

static void __iomem *cyclone_mc;

static cycle_t read_cyclone(struct clocksource *cs)
{
	return (cycle_t)readq((void __iomem *)cyclone_mc);
}

static struct clocksource clocksource_cyclone = {
        .name           = "cyclone",
        .rating         = 300,
        .read           = read_cyclone,
        .mask           = (1LL << 40) - 1,
        .mult           = 0, /*to be caluclated*/
        .shift          = 16,
        .flags          = CLOCK_SOURCE_IS_CONTINUOUS,
};

int __init init_cyclone_clock(void)
{
	u64 __iomem *reg;
	u64 base;	/* saved cyclone base address */
	u64 offset;	/* offset from pageaddr to cyclone_timer register */
	int i;
	u32 __iomem *cyclone_timer;	/* Cyclone MPMC0 register */

	if (!use_cyclone)
		return 0;

	printk(KERN_INFO "Summit chipset: Starting Cyclone Counter.\n");

	/* find base address */
	offset = (CYCLONE_CBAR_ADDR);
	reg = ioremap_nocache(offset, sizeof(u64));
	if(!reg){
		printk(KERN_ERR "Summit chipset: Could not find valid CBAR"
				" register.\n");
		use_cyclone = 0;
		return -ENODEV;
	}
	base = readq(reg);
	if(!base){
		printk(KERN_ERR "Summit chipset: Could not find valid CBAR"
				" value.\n");
		use_cyclone = 0;
		return -ENODEV;
	}
	iounmap(reg);

	/* setup PMCC */
	offset = (base + CYCLONE_PMCC_OFFSET);
	reg = ioremap_nocache(offset, sizeof(u64));
	if(!reg){
		printk(KERN_ERR "Summit chipset: Could not find valid PMCC"
				" register.\n");
		use_cyclone = 0;
		return -ENODEV;
	}
	writel(0x00000001,reg);
	iounmap(reg);

	/* setup MPCS */
	offset = (base + CYCLONE_MPCS_OFFSET);
	reg = ioremap_nocache(offset, sizeof(u64));
	if(!reg){
		printk(KERN_ERR "Summit chipset: Could not find valid MPCS"
				" register.\n");
		use_cyclone = 0;
		return -ENODEV;
	}
	writel(0x00000001,reg);
	iounmap(reg);

	/* map in cyclone_timer */
	offset = (base + CYCLONE_MPMC_OFFSET);
	cyclone_timer = ioremap_nocache(offset, sizeof(u32));
	if(!cyclone_timer){
		printk(KERN_ERR "Summit chipset: Could not find valid MPMC"
				" register.\n");
		use_cyclone = 0;
		return -ENODEV;
	}

	/*quick test to make sure its ticking*/
	for(i=0; i<3; i++){
		u32 old = readl(cyclone_timer);
		int stall = 100;
		while(stall--) barrier();
		if(readl(cyclone_timer) == old){
			printk(KERN_ERR "Summit chipset: Counter not counting!"
					" DISABLED\n");
			iounmap(cyclone_timer);
			cyclone_timer = NULL;
			use_cyclone = 0;
			return -ENODEV;
		}
	}
	/* initialize last tick */
	cyclone_mc = cyclone_timer;
	clocksource_cyclone.fsys_mmio = cyclone_timer;
	clocksource_cyclone.mult = clocksource_hz2mult(CYCLONE_TIMER_FREQ,
						clocksource_cyclone.shift);
	clocksource_register(&clocksource_cyclone);

	return 0;
}

__initcall(init_cyclone_clock);
