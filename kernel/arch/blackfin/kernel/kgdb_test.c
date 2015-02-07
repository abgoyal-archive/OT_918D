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
 * arch/blackfin/kernel/kgdb_test.c - Blackfin kgdb tests
 *
 * Copyright 2005-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

#include <asm/current.h>
#include <asm/uaccess.h>
#include <asm/system.h>

#include <asm/blackfin.h>

/* Symbols are here for kgdb test to poke directly */
static char cmdline[256];
static size_t len;

#ifndef CONFIG_SMP
static int num1 __attribute__((l1_data));

void kgdb_l1_test(void) __attribute__((l1_text));

void kgdb_l1_test(void)
{
	pr_alert("L1(before change) : data variable addr = 0x%p, data value is %d\n", &num1, num1);
	pr_alert("L1 : code function addr = 0x%p\n", kgdb_l1_test);
	num1 = num1 + 10;
	pr_alert("L1(after change) : data variable addr = 0x%p, data value is %d\n", &num1, num1);
}
#endif

#if L2_LENGTH

static int num2 __attribute__((l2));
void kgdb_l2_test(void) __attribute__((l2));

void kgdb_l2_test(void)
{
	pr_alert("L2(before change) : data variable addr = 0x%p, data value is %d\n", &num2, num2);
	pr_alert("L2 : code function addr = 0x%p\n", kgdb_l2_test);
	num2 = num2 + 20;
	pr_alert("L2(after change) : data variable addr = 0x%p, data value is %d\n", &num2, num2);
}

#endif


int kgdb_test(char *name, int len, int count, int z)
{
	pr_alert("kgdb name(%d): %s, %d, %d\n", len, name, count, z);
	count = z;
	return count;
}

static ssize_t
kgdb_test_proc_read(struct file *file, char __user *buf,
                    size_t count, loff_t *ppos)
{
	kgdb_test("hello world!", 12, 0x55, 0x10);
#ifndef CONFIG_SMP
	kgdb_l1_test();
#endif
#if L2_LENGTH
	kgdb_l2_test();
#endif

	return 0;
}

static ssize_t
kgdb_test_proc_write(struct file *file, const char __user *buffer,
                     size_t count, loff_t *pos)
{
	len = min_t(size_t, 255, count);
	memcpy(cmdline, buffer, count);
	cmdline[len] = 0;

	return len;
}

static const struct file_operations kgdb_test_proc_fops = {
	.owner = THIS_MODULE,
	.read  = kgdb_test_proc_read,
	.write = kgdb_test_proc_write,
};

static int __init kgdbtest_init(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create("kgdbtest", 0, NULL, &kgdb_test_proc_fops);
	if (entry == NULL)
		return -ENOMEM;

	return 0;
}

static void __exit kgdbtest_exit(void)
{
	remove_proc_entry("kgdbtest", NULL);
}

module_init(kgdbtest_init);
module_exit(kgdbtest_exit);
MODULE_LICENSE("GPL");
