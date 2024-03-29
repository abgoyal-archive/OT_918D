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

/* Copyright (c) 2007 Coraid, Inc.  See COPYING for GPL terms. */
/*
 * aoechr.c
 * AoE character device driver
 */

#include <linux/hdreg.h>
#include <linux/blkdev.h>
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/skbuff.h>
#include "aoe.h"

enum {
	//MINOR_STAT = 1, (moved to sysfs)
	MINOR_ERR = 2,
	MINOR_DISCOVER,
	MINOR_INTERFACES,
	MINOR_REVALIDATE,
	MINOR_FLUSH,
	MSGSZ = 2048,
	NMSG = 100,		/* message backlog to retain */
};

struct aoe_chardev {
	ulong minor;
	char name[32];
};

enum { EMFL_VALID = 1 };

struct ErrMsg {
	short flags;
	short len;
	char *msg;
};

static struct ErrMsg emsgs[NMSG];
static int emsgs_head_idx, emsgs_tail_idx;
static struct completion emsgs_comp;
static spinlock_t emsgs_lock;
static int nblocked_emsgs_readers;
static struct class *aoe_class;
static struct aoe_chardev chardevs[] = {
	{ MINOR_ERR, "err" },
	{ MINOR_DISCOVER, "discover" },
	{ MINOR_INTERFACES, "interfaces" },
	{ MINOR_REVALIDATE, "revalidate" },
	{ MINOR_FLUSH, "flush" },
};

static int
discover(void)
{
	aoecmd_cfg(0xffff, 0xff);
	return 0;
}

static int
interfaces(const char __user *str, size_t size)
{
	if (set_aoe_iflist(str, size)) {
		printk(KERN_ERR
			"aoe: could not set interface list: too many interfaces\n");
		return -EINVAL;
	}
	return 0;
}

static int
revalidate(const char __user *str, size_t size)
{
	int major, minor, n;
	ulong flags;
	struct aoedev *d;
	struct sk_buff *skb;
	char buf[16];

	if (size >= sizeof buf)
		return -EINVAL;
	buf[sizeof buf - 1] = '\0';
	if (copy_from_user(buf, str, size))
		return -EFAULT;

	/* should be e%d.%d format */
	n = sscanf(buf, "e%d.%d", &major, &minor);
	if (n != 2) {
		printk(KERN_ERR "aoe: invalid device specification\n");
		return -EINVAL;
	}
	d = aoedev_by_aoeaddr(major, minor);
	if (!d)
		return -EINVAL;
	spin_lock_irqsave(&d->lock, flags);
	aoecmd_cleanslate(d);
loop:
	skb = aoecmd_ata_id(d);
	spin_unlock_irqrestore(&d->lock, flags);
	/* try again if we are able to sleep a bit,
	 * otherwise give up this revalidation
	 */
	if (!skb && !msleep_interruptible(200)) {
		spin_lock_irqsave(&d->lock, flags);
		goto loop;
	}
	if (skb) {
		struct sk_buff_head queue;
		__skb_queue_head_init(&queue);
		__skb_queue_tail(&queue, skb);
		aoenet_xmit(&queue);
	}
	aoecmd_cfg(major, minor);
	return 0;
}

void
aoechr_error(char *msg)
{
	struct ErrMsg *em;
	char *mp;
	ulong flags, n;

	n = strlen(msg);

	spin_lock_irqsave(&emsgs_lock, flags);

	em = emsgs + emsgs_tail_idx;
	if ((em->flags & EMFL_VALID)) {
bail:		spin_unlock_irqrestore(&emsgs_lock, flags);
		return;
	}

	mp = kmalloc(n, GFP_ATOMIC);
	if (mp == NULL) {
		printk(KERN_ERR "aoe: allocation failure, len=%ld\n", n);
		goto bail;
	}

	memcpy(mp, msg, n);
	em->msg = mp;
	em->flags |= EMFL_VALID;
	em->len = n;

	emsgs_tail_idx++;
	emsgs_tail_idx %= ARRAY_SIZE(emsgs);

	spin_unlock_irqrestore(&emsgs_lock, flags);

	if (nblocked_emsgs_readers)
		complete(&emsgs_comp);
}

static ssize_t
aoechr_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offp)
{
	int ret = -EINVAL;

	switch ((unsigned long) filp->private_data) {
	default:
		printk(KERN_INFO "aoe: can't write to that file.\n");
		break;
	case MINOR_DISCOVER:
		ret = discover();
		break;
	case MINOR_INTERFACES:
		ret = interfaces(buf, cnt);
		break;
	case MINOR_REVALIDATE:
		ret = revalidate(buf, cnt);
		break;
	case MINOR_FLUSH:
		ret = aoedev_flush(buf, cnt);
	}
	if (ret == 0)
		ret = cnt;
	return ret;
}

static int
aoechr_open(struct inode *inode, struct file *filp)
{
	int n, i;

	lock_kernel();
	n = iminor(inode);
	filp->private_data = (void *) (unsigned long) n;

	for (i = 0; i < ARRAY_SIZE(chardevs); ++i)
		if (chardevs[i].minor == n) {
			unlock_kernel();
			return 0;
		}
	unlock_kernel();
	return -EINVAL;
}

static int
aoechr_rel(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t
aoechr_read(struct file *filp, char __user *buf, size_t cnt, loff_t *off)
{
	unsigned long n;
	char *mp;
	struct ErrMsg *em;
	ssize_t len;
	ulong flags;

	n = (unsigned long) filp->private_data;
	if (n != MINOR_ERR)
		return -EFAULT;

	spin_lock_irqsave(&emsgs_lock, flags);

	for (;;) {
		em = emsgs + emsgs_head_idx;
		if ((em->flags & EMFL_VALID) != 0)
			break;
		if (filp->f_flags & O_NDELAY) {
			spin_unlock_irqrestore(&emsgs_lock, flags);
			return -EAGAIN;
		}
		nblocked_emsgs_readers++;

		spin_unlock_irqrestore(&emsgs_lock, flags);

		n = wait_for_completion_interruptible(&emsgs_comp);

		spin_lock_irqsave(&emsgs_lock, flags);

		nblocked_emsgs_readers--;

		if (n) {
			spin_unlock_irqrestore(&emsgs_lock, flags);
			return -ERESTARTSYS;
		}
	}
	if (em->len > cnt) {
		spin_unlock_irqrestore(&emsgs_lock, flags);
		return -EAGAIN;
	}
	mp = em->msg;
	len = em->len;
	em->msg = NULL;
	em->flags &= ~EMFL_VALID;

	emsgs_head_idx++;
	emsgs_head_idx %= ARRAY_SIZE(emsgs);

	spin_unlock_irqrestore(&emsgs_lock, flags);

	n = copy_to_user(buf, mp, len);
	kfree(mp);
	return n == 0 ? len : -EFAULT;
}

static const struct file_operations aoe_fops = {
	.write = aoechr_write,
	.read = aoechr_read,
	.open = aoechr_open,
	.release = aoechr_rel,
	.owner = THIS_MODULE,
};

static char *aoe_devnode(struct device *dev, mode_t *mode)
{
	return kasprintf(GFP_KERNEL, "etherd/%s", dev_name(dev));
}

int __init
aoechr_init(void)
{
	int n, i;

	n = register_chrdev(AOE_MAJOR, "aoechr", &aoe_fops);
	if (n < 0) { 
		printk(KERN_ERR "aoe: can't register char device\n");
		return n;
	}
	init_completion(&emsgs_comp);
	spin_lock_init(&emsgs_lock);
	aoe_class = class_create(THIS_MODULE, "aoe");
	if (IS_ERR(aoe_class)) {
		unregister_chrdev(AOE_MAJOR, "aoechr");
		return PTR_ERR(aoe_class);
	}
	aoe_class->devnode = aoe_devnode;

	for (i = 0; i < ARRAY_SIZE(chardevs); ++i)
		device_create(aoe_class, NULL,
			      MKDEV(AOE_MAJOR, chardevs[i].minor), NULL,
			      chardevs[i].name);

	return 0;
}

void
aoechr_exit(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(chardevs); ++i)
		device_destroy(aoe_class, MKDEV(AOE_MAJOR, chardevs[i].minor));
	class_destroy(aoe_class);
	unregister_chrdev(AOE_MAJOR, "aoechr");
}

