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

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/topology.h>
#include <linux/cpu.h>
#include <linux/range.h>

#include <asm/pci_x86.h>

#include <asm/pci-direct.h>

#include "bus_numa.h"

/*
 * This discovers the pcibus <-> node mapping on AMD K8.
 * also get peer root bus resource for io,mmio
 */

struct pci_hostbridge_probe {
	u32 bus;
	u32 slot;
	u32 vendor;
	u32 device;
};

static struct pci_hostbridge_probe pci_probes[] __initdata = {
	{ 0, 0x18, PCI_VENDOR_ID_AMD, 0x1100 },
	{ 0, 0x18, PCI_VENDOR_ID_AMD, 0x1200 },
	{ 0xff, 0, PCI_VENDOR_ID_AMD, 0x1200 },
	{ 0, 0x18, PCI_VENDOR_ID_AMD, 0x1300 },
};

static u64 __initdata fam10h_mmconf_start;
static u64 __initdata fam10h_mmconf_end;
static void __init get_pci_mmcfg_amd_fam10h_range(void)
{
	u32 address;
	u64 base, msr;
	unsigned segn_busn_bits;

	/* assume all cpus from fam10h have mmconf */
        if (boot_cpu_data.x86 < 0x10)
		return;

	address = MSR_FAM10H_MMIO_CONF_BASE;
	rdmsrl(address, msr);

	/* mmconfig is not enable */
	if (!(msr & FAM10H_MMIO_CONF_ENABLE))
		return;

	base = msr & (FAM10H_MMIO_CONF_BASE_MASK<<FAM10H_MMIO_CONF_BASE_SHIFT);

	segn_busn_bits = (msr >> FAM10H_MMIO_CONF_BUSRANGE_SHIFT) &
			 FAM10H_MMIO_CONF_BUSRANGE_MASK;

	fam10h_mmconf_start = base;
	fam10h_mmconf_end = base + (1ULL<<(segn_busn_bits + 20)) - 1;
}

#define RANGE_NUM 16

/**
 * early_fill_mp_bus_to_node()
 * called before pcibios_scan_root and pci_scan_bus
 * fills the mp_bus_to_cpumask array based according to the LDT Bus Number
 * Registers found in the K8 northbridge
 */
static int __init early_fill_mp_bus_info(void)
{
	int i;
	int j;
	unsigned bus;
	unsigned slot;
	int node;
	int link;
	int def_node;
	int def_link;
	struct pci_root_info *info;
	u32 reg;
	struct resource *res;
	u64 start;
	u64 end;
	struct range range[RANGE_NUM];
	u64 val;
	u32 address;
	bool found;

	if (!early_pci_allowed())
		return -1;

	found = false;
	for (i = 0; i < ARRAY_SIZE(pci_probes); i++) {
		u32 id;
		u16 device;
		u16 vendor;

		bus = pci_probes[i].bus;
		slot = pci_probes[i].slot;
		id = read_pci_config(bus, slot, 0, PCI_VENDOR_ID);

		vendor = id & 0xffff;
		device = (id>>16) & 0xffff;
		if (pci_probes[i].vendor == vendor &&
		    pci_probes[i].device == device) {
			found = true;
			break;
		}
	}

	if (!found)
		return 0;

	pci_root_num = 0;
	for (i = 0; i < 4; i++) {
		int min_bus;
		int max_bus;
		reg = read_pci_config(bus, slot, 1, 0xe0 + (i << 2));

		/* Check if that register is enabled for bus range */
		if ((reg & 7) != 3)
			continue;

		min_bus = (reg >> 16) & 0xff;
		max_bus = (reg >> 24) & 0xff;
		node = (reg >> 4) & 0x07;
#ifdef CONFIG_NUMA
		for (j = min_bus; j <= max_bus; j++)
			set_mp_bus_to_node(j, node);
#endif
		link = (reg >> 8) & 0x03;

		info = &pci_root_info[pci_root_num];
		info->bus_min = min_bus;
		info->bus_max = max_bus;
		info->node = node;
		info->link = link;
		sprintf(info->name, "PCI Bus #%02x", min_bus);
		pci_root_num++;
	}

	/* get the default node and link for left over res */
	reg = read_pci_config(bus, slot, 0, 0x60);
	def_node = (reg >> 8) & 0x07;
	reg = read_pci_config(bus, slot, 0, 0x64);
	def_link = (reg >> 8) & 0x03;

	memset(range, 0, sizeof(range));
	add_range(range, RANGE_NUM, 0, 0, 0xffff + 1);
	/* io port resource */
	for (i = 0; i < 4; i++) {
		reg = read_pci_config(bus, slot, 1, 0xc0 + (i << 3));
		if (!(reg & 3))
			continue;

		start = reg & 0xfff000;
		reg = read_pci_config(bus, slot, 1, 0xc4 + (i << 3));
		node = reg & 0x07;
		link = (reg >> 4) & 0x03;
		end = (reg & 0xfff000) | 0xfff;

		/* find the position */
		for (j = 0; j < pci_root_num; j++) {
			info = &pci_root_info[j];
			if (info->node == node && info->link == link)
				break;
		}
		if (j == pci_root_num)
			continue; /* not found */

		info = &pci_root_info[j];
		printk(KERN_DEBUG "node %d link %d: io port [%llx, %llx]\n",
		       node, link, start, end);

		/* kernel only handle 16 bit only */
		if (end > 0xffff)
			end = 0xffff;
		update_res(info, start, end, IORESOURCE_IO, 1);
		subtract_range(range, RANGE_NUM, start, end + 1);
	}
	/* add left over io port range to def node/link, [0, 0xffff] */
	/* find the position */
	for (j = 0; j < pci_root_num; j++) {
		info = &pci_root_info[j];
		if (info->node == def_node && info->link == def_link)
			break;
	}
	if (j < pci_root_num) {
		info = &pci_root_info[j];
		for (i = 0; i < RANGE_NUM; i++) {
			if (!range[i].end)
				continue;

			update_res(info, range[i].start, range[i].end - 1,
				   IORESOURCE_IO, 1);
		}
	}

	memset(range, 0, sizeof(range));
	/* 0xfd00000000-0xffffffffff for HT */
	end = cap_resource((0xfdULL<<32) - 1);
	end++;
	add_range(range, RANGE_NUM, 0, 0, end);

	/* need to take out [0, TOM) for RAM*/
	address = MSR_K8_TOP_MEM1;
	rdmsrl(address, val);
	end = (val & 0xffffff800000ULL);
	printk(KERN_INFO "TOM: %016llx aka %lldM\n", end, end>>20);
	if (end < (1ULL<<32))
		subtract_range(range, RANGE_NUM, 0, end);

	/* get mmconfig */
	get_pci_mmcfg_amd_fam10h_range();
	/* need to take out mmconf range */
	if (fam10h_mmconf_end) {
		printk(KERN_DEBUG "Fam 10h mmconf [%llx, %llx]\n", fam10h_mmconf_start, fam10h_mmconf_end);
		subtract_range(range, RANGE_NUM, fam10h_mmconf_start,
				 fam10h_mmconf_end + 1);
	}

	/* mmio resource */
	for (i = 0; i < 8; i++) {
		reg = read_pci_config(bus, slot, 1, 0x80 + (i << 3));
		if (!(reg & 3))
			continue;

		start = reg & 0xffffff00; /* 39:16 on 31:8*/
		start <<= 8;
		reg = read_pci_config(bus, slot, 1, 0x84 + (i << 3));
		node = reg & 0x07;
		link = (reg >> 4) & 0x03;
		end = (reg & 0xffffff00);
		end <<= 8;
		end |= 0xffff;

		/* find the position */
		for (j = 0; j < pci_root_num; j++) {
			info = &pci_root_info[j];
			if (info->node == node && info->link == link)
				break;
		}
		if (j == pci_root_num)
			continue; /* not found */

		info = &pci_root_info[j];

		printk(KERN_DEBUG "node %d link %d: mmio [%llx, %llx]",
		       node, link, start, end);
		/*
		 * some sick allocation would have range overlap with fam10h
		 * mmconf range, so need to update start and end.
		 */
		if (fam10h_mmconf_end) {
			int changed = 0;
			u64 endx = 0;
			if (start >= fam10h_mmconf_start &&
			    start <= fam10h_mmconf_end) {
				start = fam10h_mmconf_end + 1;
				changed = 1;
			}

			if (end >= fam10h_mmconf_start &&
			    end <= fam10h_mmconf_end) {
				end = fam10h_mmconf_start - 1;
				changed = 1;
			}

			if (start < fam10h_mmconf_start &&
			    end > fam10h_mmconf_end) {
				/* we got a hole */
				endx = fam10h_mmconf_start - 1;
				update_res(info, start, endx, IORESOURCE_MEM, 0);
				subtract_range(range, RANGE_NUM, start,
						 endx + 1);
				printk(KERN_CONT " ==> [%llx, %llx]", start, endx);
				start = fam10h_mmconf_end + 1;
				changed = 1;
			}
			if (changed) {
				if (start <= end) {
					printk(KERN_CONT " %s [%llx, %llx]", endx ? "and" : "==>", start, end);
				} else {
					printk(KERN_CONT "%s\n", endx?"":" ==> none");
					continue;
				}
			}
		}

		update_res(info, cap_resource(start), cap_resource(end),
				 IORESOURCE_MEM, 1);
		subtract_range(range, RANGE_NUM, start, end + 1);
		printk(KERN_CONT "\n");
	}

	/* need to take out [4G, TOM2) for RAM*/
	/* SYS_CFG */
	address = MSR_K8_SYSCFG;
	rdmsrl(address, val);
	/* TOP_MEM2 is enabled? */
	if (val & (1<<21)) {
		/* TOP_MEM2 */
		address = MSR_K8_TOP_MEM2;
		rdmsrl(address, val);
		end = (val & 0xffffff800000ULL);
		printk(KERN_INFO "TOM2: %016llx aka %lldM\n", end, end>>20);
		subtract_range(range, RANGE_NUM, 1ULL<<32, end);
	}

	/*
	 * add left over mmio range to def node/link ?
	 * that is tricky, just record range in from start_min to 4G
	 */
	for (j = 0; j < pci_root_num; j++) {
		info = &pci_root_info[j];
		if (info->node == def_node && info->link == def_link)
			break;
	}
	if (j < pci_root_num) {
		info = &pci_root_info[j];

		for (i = 0; i < RANGE_NUM; i++) {
			if (!range[i].end)
				continue;

			update_res(info, cap_resource(range[i].start),
				   cap_resource(range[i].end - 1),
				   IORESOURCE_MEM, 1);
		}
	}

	for (i = 0; i < pci_root_num; i++) {
		int res_num;
		int busnum;

		info = &pci_root_info[i];
		res_num = info->res_num;
		busnum = info->bus_min;
		printk(KERN_DEBUG "bus: [%02x, %02x] on node %x link %x\n",
		       info->bus_min, info->bus_max, info->node, info->link);
		for (j = 0; j < res_num; j++) {
			res = &info->res[j];
			printk(KERN_DEBUG "bus: %02x index %x %pR\n",
				       busnum, j, res);
		}
	}

	return 0;
}

#define ENABLE_CF8_EXT_CFG      (1ULL << 46)

static void enable_pci_io_ecs(void *unused)
{
	u64 reg;
	rdmsrl(MSR_AMD64_NB_CFG, reg);
	if (!(reg & ENABLE_CF8_EXT_CFG)) {
		reg |= ENABLE_CF8_EXT_CFG;
		wrmsrl(MSR_AMD64_NB_CFG, reg);
	}
}

static int __cpuinit amd_cpu_notify(struct notifier_block *self,
				    unsigned long action, void *hcpu)
{
	int cpu = (long)hcpu;
	switch (action) {
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
		smp_call_function_single(cpu, enable_pci_io_ecs, NULL, 0);
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}

static struct notifier_block __cpuinitdata amd_cpu_notifier = {
	.notifier_call	= amd_cpu_notify,
};

static int __init pci_io_ecs_init(void)
{
	int cpu;

	/* assume all cpus from fam10h have IO ECS */
        if (boot_cpu_data.x86 < 0x10)
		return 0;

	register_cpu_notifier(&amd_cpu_notifier);
	for_each_online_cpu(cpu)
		amd_cpu_notify(&amd_cpu_notifier, (unsigned long)CPU_ONLINE,
			       (void *)(long)cpu);
	pci_probe |= PCI_HAS_IO_ECS;

	return 0;
}

static int __init amd_postcore_init(void)
{
	if (boot_cpu_data.x86_vendor != X86_VENDOR_AMD)
		return 0;

	early_fill_mp_bus_info();
	pci_io_ecs_init();

	return 0;
}

postcore_initcall(amd_postcore_init);
