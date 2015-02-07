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
 * Copyright (C) 2009  Cisco Systems, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _ASM_MACH_POWERTV_ASIC_H
#define _ASM_MACH_POWERTV_ASIC_H

#include <linux/ioport.h>
#include <asm/mach-powertv/asic_regs.h>

#define DVR_CAPABLE     (1<<0)
#define PCIE_CAPABLE    (1<<1)
#define FFS_CAPABLE     (1<<2)
#define DISPLAY_CAPABLE (1<<3)

/* Platform Family types
 * For compitability, the new value must be added in the end */
enum family_type {
	FAMILY_8500,
	FAMILY_8500RNG,
	FAMILY_4500,
	FAMILY_1500,
	FAMILY_8600,
	FAMILY_4600,
	FAMILY_4600VZA,
	FAMILY_8600VZB,
	FAMILY_1500VZE,
	FAMILY_1500VZF,
	FAMILIES
};

/* Register maps for each ASIC */
extern const struct register_map calliope_register_map;
extern const struct register_map cronus_register_map;
extern const struct register_map zeus_register_map;

extern struct resource dvr_cronus_resources[];
extern struct resource dvr_zeus_resources[];
extern struct resource non_dvr_calliope_resources[];
extern struct resource non_dvr_cronus_resources[];
extern struct resource non_dvr_cronuslite_resources[];
extern struct resource non_dvr_vz_calliope_resources[];
extern struct resource non_dvr_vze_calliope_resources[];
extern struct resource non_dvr_vzf_calliope_resources[];
extern struct resource non_dvr_zeus_resources[];

extern void powertv_platform_init(void);
extern void platform_alloc_bootmem(void);
extern enum asic_type platform_get_asic(void);
extern enum family_type platform_get_family(void);
extern int platform_supports_dvr(void);
extern int platform_supports_ffs(void);
extern int platform_supports_pcie(void);
extern int platform_supports_display(void);
extern void configure_platform(void);
extern void platform_configure_usb_ehci(void);
extern void platform_unconfigure_usb_ehci(void);
extern void platform_configure_usb_ohci(void);
extern void platform_unconfigure_usb_ohci(void);

/* Platform Resources */
#define ASIC_RESOURCE_GET_EXISTS 1
extern struct resource *asic_resource_get(const char *name);
extern void platform_release_memory(void *baddr, int size);

/* Reboot Cause */
extern void set_reboot_cause(char code, unsigned int data, unsigned int data2);
extern void set_locked_reboot_cause(char code, unsigned int data,
	unsigned int data2);

enum sys_reboot_type {
	sys_unknown_reboot = 0x00,	/* Unknown reboot cause */
	sys_davic_change = 0x01,	/* Reboot due to change in DAVIC
					 * mode */
	sys_user_reboot = 0x02,		/* Reboot initiated by user */
	sys_system_reboot = 0x03,	/* Reboot initiated by OS */
	sys_trap_reboot = 0x04,		/* Reboot due to a CPU trap */
	sys_silent_reboot = 0x05,	/* Silent reboot */
	sys_boot_ldr_reboot = 0x06,	/* Bootloader reboot */
	sys_power_up_reboot = 0x07,	/* Power on bootup.  Older
					 * drivers may report as
					 * userReboot. */
	sys_code_change = 0x08,		/* Reboot to take code change.
					 * Older drivers may report as
					 * userReboot. */
	sys_hardware_reset = 0x09,	/* HW watchdog or front-panel
					 * reset button reset.  Older
					 * drivers may report as
					 * userReboot. */
	sys_watchdogInterrupt = 0x0A	/* Pre-watchdog interrupt */
};

#endif /* _ASM_MACH_POWERTV_ASIC_H */
