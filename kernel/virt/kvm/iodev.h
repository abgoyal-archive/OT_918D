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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __KVM_IODEV_H__
#define __KVM_IODEV_H__

#include <linux/kvm_types.h>
#include <asm/errno.h>

struct kvm_io_device;

/**
 * kvm_io_device_ops are called under kvm slots_lock.
 * read and write handlers return 0 if the transaction has been handled,
 * or non-zero to have it passed to the next device.
 **/
struct kvm_io_device_ops {
	int (*read)(struct kvm_io_device *this,
		    gpa_t addr,
		    int len,
		    void *val);
	int (*write)(struct kvm_io_device *this,
		     gpa_t addr,
		     int len,
		     const void *val);
	void (*destructor)(struct kvm_io_device *this);
};


struct kvm_io_device {
	const struct kvm_io_device_ops *ops;
};

static inline void kvm_iodevice_init(struct kvm_io_device *dev,
				     const struct kvm_io_device_ops *ops)
{
	dev->ops = ops;
}

static inline int kvm_iodevice_read(struct kvm_io_device *dev,
				    gpa_t addr, int l, void *v)
{
	return dev->ops->read ? dev->ops->read(dev, addr, l, v) : -EOPNOTSUPP;
}

static inline int kvm_iodevice_write(struct kvm_io_device *dev,
				     gpa_t addr, int l, const void *v)
{
	return dev->ops->write ? dev->ops->write(dev, addr, l, v) : -EOPNOTSUPP;
}

static inline void kvm_iodevice_destructor(struct kvm_io_device *dev)
{
	if (dev->ops->destructor)
		dev->ops->destructor(dev);
}

#endif /* __KVM_IODEV_H__ */
