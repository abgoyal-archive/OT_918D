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
 * include/linux/cpu.h - generic cpu definition
 *
 * This is mainly for topological representation. We define the 
 * basic 'struct cpu' here, which can be embedded in per-arch 
 * definitions of processors.
 *
 * Basic handling of the devices is done in drivers/base/cpu.c
 * and system devices are handled in drivers/base/sys.c. 
 *
 * CPUs are exported via sysfs in the class/cpu/devices/
 * directory. 
 *
 * Per-cpu interfaces can be implemented using a struct device_interface. 
 * See the following for how to do this: 
 * - drivers/base/intf.c 
 * - Documentation/driver-model/interface.txt
 */
#ifndef _LINUX_CPU_H_
#define _LINUX_CPU_H_

#include <linux/sysdev.h>
#include <linux/node.h>
#include <linux/compiler.h>
#include <linux/cpumask.h>

struct cpu {
	int node_id;		/* The node which contains the CPU */
	int hotpluggable;	/* creates sysfs control file if hotpluggable */
	struct sys_device sysdev;
};

extern int register_cpu(struct cpu *cpu, int num);
extern struct sys_device *get_cpu_sysdev(unsigned cpu);

extern int cpu_add_sysdev_attr(struct sysdev_attribute *attr);
extern void cpu_remove_sysdev_attr(struct sysdev_attribute *attr);

extern int cpu_add_sysdev_attr_group(struct attribute_group *attrs);
extern void cpu_remove_sysdev_attr_group(struct attribute_group *attrs);

extern int sched_create_sysfs_power_savings_entries(struct sysdev_class *cls);

#ifdef CONFIG_HOTPLUG_CPU
extern void unregister_cpu(struct cpu *cpu);
extern ssize_t arch_cpu_probe(const char *, size_t);
extern ssize_t arch_cpu_release(const char *, size_t);
#endif
struct notifier_block;

#ifdef CONFIG_SMP
/* Need to know about CPUs going up/down? */
#if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE)
#define cpu_notifier(fn, pri) {					\
	static struct notifier_block fn##_nb __cpuinitdata =	\
		{ .notifier_call = fn, .priority = pri };	\
	register_cpu_notifier(&fn##_nb);			\
}
#else /* #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
#define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)
#endif /* #else #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
#ifdef CONFIG_HOTPLUG_CPU
extern int register_cpu_notifier(struct notifier_block *nb);
extern void unregister_cpu_notifier(struct notifier_block *nb);
#else

#ifndef MODULE
extern int register_cpu_notifier(struct notifier_block *nb);
#else
static inline int register_cpu_notifier(struct notifier_block *nb)
{
	return 0;
}
#endif

static inline void unregister_cpu_notifier(struct notifier_block *nb)
{
}
#endif

int cpu_up(unsigned int cpu);
void notify_cpu_starting(unsigned int cpu);
extern void cpu_maps_update_begin(void);
extern void cpu_maps_update_done(void);

#else	/* CONFIG_SMP */

#define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)

static inline int register_cpu_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline void unregister_cpu_notifier(struct notifier_block *nb)
{
}

static inline void cpu_maps_update_begin(void)
{
}

static inline void cpu_maps_update_done(void)
{
}

#endif /* CONFIG_SMP */
extern struct sysdev_class cpu_sysdev_class;

#ifdef CONFIG_HOTPLUG_CPU
/* Stop CPUs going up and down. */

extern void get_online_cpus(void);
extern void put_online_cpus(void);
#define hotcpu_notifier(fn, pri)	cpu_notifier(fn, pri)
#define register_hotcpu_notifier(nb)	register_cpu_notifier(nb)
#define unregister_hotcpu_notifier(nb)	unregister_cpu_notifier(nb)
int cpu_down(unsigned int cpu);

#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE
extern void cpu_hotplug_driver_lock(void);
extern void cpu_hotplug_driver_unlock(void);
#else
static inline void cpu_hotplug_driver_lock(void)
{
}

static inline void cpu_hotplug_driver_unlock(void)
{
}
#endif

#else		/* CONFIG_HOTPLUG_CPU */

#define get_online_cpus()	do { } while (0)
#define put_online_cpus()	do { } while (0)
#define hotcpu_notifier(fn, pri)	do { (void)(fn); } while (0)
/* These aren't inline functions due to a GCC bug. */
#define register_hotcpu_notifier(nb)	({ (void)(nb); 0; })
#define unregister_hotcpu_notifier(nb)	({ (void)(nb); })
#endif		/* CONFIG_HOTPLUG_CPU */

#ifdef CONFIG_PM_SLEEP_SMP
extern int suspend_cpu_hotplug;

extern int disable_nonboot_cpus(void);
extern void enable_nonboot_cpus(void);
#else /* !CONFIG_PM_SLEEP_SMP */
#define suspend_cpu_hotplug	0

static inline int disable_nonboot_cpus(void) { return 0; }
static inline void enable_nonboot_cpus(void) {}
#endif /* !CONFIG_PM_SLEEP_SMP */

#endif /* _LINUX_CPU_H_ */
