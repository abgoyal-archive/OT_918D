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
 * sma cpu5 watchdog driver
 *
 * Copyright (C) 2003 Heiko Ronsdorf <hero@ihg.uni-duisburg.de>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/timer.h>
#include <linux/completion.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/watchdog.h>

/* adjustable parameters */

static int verbose;
static int port = 0x91;
static int ticks = 10000;
static spinlock_t cpu5wdt_lock;

#define PFX			"cpu5wdt: "

#define CPU5WDT_EXTENT          0x0A

#define CPU5WDT_STATUS_REG      0x00
#define CPU5WDT_TIME_A_REG      0x02
#define CPU5WDT_TIME_B_REG      0x03
#define CPU5WDT_MODE_REG        0x04
#define CPU5WDT_TRIGGER_REG     0x07
#define CPU5WDT_ENABLE_REG      0x08
#define CPU5WDT_RESET_REG       0x09

#define CPU5WDT_INTERVAL	(HZ/10+1)

/* some device data */

static struct {
	struct completion stop;
	int running;
	struct timer_list timer;
	int queue;
	int default_ticks;
	unsigned long inuse;
} cpu5wdt_device;

/* generic helper functions */

static void cpu5wdt_trigger(unsigned long unused)
{
	if (verbose > 2)
		printk(KERN_DEBUG PFX "trigger at %i ticks\n", ticks);

	if (cpu5wdt_device.running)
		ticks--;

	spin_lock(&cpu5wdt_lock);
	/* keep watchdog alive */
	outb(1, port + CPU5WDT_TRIGGER_REG);

	/* requeue?? */
	if (cpu5wdt_device.queue && ticks)
		mod_timer(&cpu5wdt_device.timer, jiffies + CPU5WDT_INTERVAL);
	else {
		/* ticks doesn't matter anyway */
		complete(&cpu5wdt_device.stop);
	}
	spin_unlock(&cpu5wdt_lock);

}

static void cpu5wdt_reset(void)
{
	ticks = cpu5wdt_device.default_ticks;

	if (verbose)
		printk(KERN_DEBUG PFX "reset (%i ticks)\n", (int) ticks);

}

static void cpu5wdt_start(void)
{
	unsigned long flags;

	spin_lock_irqsave(&cpu5wdt_lock, flags);
	if (!cpu5wdt_device.queue) {
		cpu5wdt_device.queue = 1;
		outb(0, port + CPU5WDT_TIME_A_REG);
		outb(0, port + CPU5WDT_TIME_B_REG);
		outb(1, port + CPU5WDT_MODE_REG);
		outb(0, port + CPU5WDT_RESET_REG);
		outb(0, port + CPU5WDT_ENABLE_REG);
		mod_timer(&cpu5wdt_device.timer, jiffies + CPU5WDT_INTERVAL);
	}
	/* if process dies, counter is not decremented */
	cpu5wdt_device.running++;
	spin_unlock_irqrestore(&cpu5wdt_lock, flags);
}

static int cpu5wdt_stop(void)
{
	unsigned long flags;

	spin_lock_irqsave(&cpu5wdt_lock, flags);
	if (cpu5wdt_device.running)
		cpu5wdt_device.running = 0;
	ticks = cpu5wdt_device.default_ticks;
	spin_unlock_irqrestore(&cpu5wdt_lock, flags);
	if (verbose)
		printk(KERN_CRIT PFX "stop not possible\n");
	return -EIO;
}

/* filesystem operations */

static int cpu5wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &cpu5wdt_device.inuse))
		return -EBUSY;
	return nonseekable_open(inode, file);
}

static int cpu5wdt_release(struct inode *inode, struct file *file)
{
	clear_bit(0, &cpu5wdt_device.inuse);
	return 0;
}

static long cpu5wdt_ioctl(struct file *file, unsigned int cmd,
						unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	unsigned int value;
	static const struct watchdog_info ident = {
		.options = WDIOF_CARDRESET,
		.identity = "CPU5 WDT",
	};

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		if (copy_to_user(argp, &ident, sizeof(ident)))
			return -EFAULT;
		break;
	case WDIOC_GETSTATUS:
		value = inb(port + CPU5WDT_STATUS_REG);
		value = (value >> 2) & 1;
		return put_user(value, p);
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);
	case WDIOC_SETOPTIONS:
		if (get_user(value, p))
			return -EFAULT;
		if (value & WDIOS_ENABLECARD)
			cpu5wdt_start();
		if (value & WDIOS_DISABLECARD)
			cpu5wdt_stop();
		break;
	case WDIOC_KEEPALIVE:
		cpu5wdt_reset();
		break;
	default:
		return -ENOTTY;
	}
	return 0;
}

static ssize_t cpu5wdt_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
	if (!count)
		return -EIO;
	cpu5wdt_reset();
	return count;
}

static const struct file_operations cpu5wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.unlocked_ioctl	= cpu5wdt_ioctl,
	.open		= cpu5wdt_open,
	.write		= cpu5wdt_write,
	.release	= cpu5wdt_release,
};

static struct miscdevice cpu5wdt_misc = {
	.minor	= WATCHDOG_MINOR,
	.name	= "watchdog",
	.fops	= &cpu5wdt_fops,
};

/* init/exit function */

static int __devinit cpu5wdt_init(void)
{
	unsigned int val;
	int err;

	if (verbose)
		printk(KERN_DEBUG PFX
				"port=0x%x, verbose=%i\n", port, verbose);

	init_completion(&cpu5wdt_device.stop);
	spin_lock_init(&cpu5wdt_lock);
	cpu5wdt_device.queue = 0;
	setup_timer(&cpu5wdt_device.timer, cpu5wdt_trigger, 0);
	cpu5wdt_device.default_ticks = ticks;

	if (!request_region(port, CPU5WDT_EXTENT, PFX)) {
		printk(KERN_ERR PFX "request_region failed\n");
		err = -EBUSY;
		goto no_port;
	}

	/* watchdog reboot? */
	val = inb(port + CPU5WDT_STATUS_REG);
	val = (val >> 2) & 1;
	if (!val)
		printk(KERN_INFO PFX "sorry, was my fault\n");

	err = misc_register(&cpu5wdt_misc);
	if (err < 0) {
		printk(KERN_ERR PFX "misc_register failed\n");
		goto no_misc;
	}


	printk(KERN_INFO PFX "init success\n");
	return 0;

no_misc:
	release_region(port, CPU5WDT_EXTENT);
no_port:
	return err;
}

static int __devinit cpu5wdt_init_module(void)
{
	return cpu5wdt_init();
}

static void __devexit cpu5wdt_exit(void)
{
	if (cpu5wdt_device.queue) {
		cpu5wdt_device.queue = 0;
		wait_for_completion(&cpu5wdt_device.stop);
	}

	misc_deregister(&cpu5wdt_misc);

	release_region(port, CPU5WDT_EXTENT);

}

static void __devexit cpu5wdt_exit_module(void)
{
	cpu5wdt_exit();
}

/* module entry points */

module_init(cpu5wdt_init_module);
module_exit(cpu5wdt_exit_module);

MODULE_AUTHOR("Heiko Ronsdorf <hero@ihg.uni-duisburg.de>");
MODULE_DESCRIPTION("sma cpu5 watchdog driver");
MODULE_SUPPORTED_DEVICE("sma cpu5 watchdog");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);

module_param(port, int, 0);
MODULE_PARM_DESC(port, "base address of watchdog card, default is 0x91");

module_param(verbose, int, 0);
MODULE_PARM_DESC(verbose, "be verbose, default is 0 (no)");

module_param(ticks, int, 0);
MODULE_PARM_DESC(ticks, "count down ticks, default is 10000");