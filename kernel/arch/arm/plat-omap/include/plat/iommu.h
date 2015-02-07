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
 * omap iommu: main structures
 *
 * Copyright (C) 2008-2009 Nokia Corporation
 *
 * Written by Hiroshi DOYU <Hiroshi.DOYU@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __MACH_IOMMU_H
#define __MACH_IOMMU_H

struct iotlb_entry {
	u32 da;
	u32 pa;
	u32 pgsz, prsvd, valid;
	union {
		u16 ap;
		struct {
			u32 endian, elsz, mixed;
		};
	};
};

struct iommu {
	const char	*name;
	struct module	*owner;
	struct clk	*clk;
	void __iomem	*regbase;
	struct device	*dev;

	unsigned int	refcount;
	struct mutex	iommu_lock;	/* global for this whole object */

	/*
	 * We don't change iopgd for a situation like pgd for a task,
	 * but share it globally for each iommu.
	 */
	u32		*iopgd;
	spinlock_t	page_table_lock; /* protect iopgd */

	int		nr_tlb_entries;

	struct list_head	mmap;
	struct mutex		mmap_lock; /* protect mmap */

	int (*isr)(struct iommu *obj);

	void *ctx; /* iommu context: registres saved area */
};

struct cr_regs {
	union {
		struct {
			u16 cam_l;
			u16 cam_h;
		};
		u32 cam;
	};
	union {
		struct {
			u16 ram_l;
			u16 ram_h;
		};
		u32 ram;
	};
};

struct iotlb_lock {
	short base;
	short vict;
};

/* architecture specific functions */
struct iommu_functions {
	unsigned long	version;

	int (*enable)(struct iommu *obj);
	void (*disable)(struct iommu *obj);
	u32 (*fault_isr)(struct iommu *obj, u32 *ra);

	void (*tlb_read_cr)(struct iommu *obj, struct cr_regs *cr);
	void (*tlb_load_cr)(struct iommu *obj, struct cr_regs *cr);

	struct cr_regs *(*alloc_cr)(struct iommu *obj, struct iotlb_entry *e);
	int (*cr_valid)(struct cr_regs *cr);
	u32 (*cr_to_virt)(struct cr_regs *cr);
	void (*cr_to_e)(struct cr_regs *cr, struct iotlb_entry *e);
	ssize_t (*dump_cr)(struct iommu *obj, struct cr_regs *cr, char *buf);

	u32 (*get_pte_attr)(struct iotlb_entry *e);

	void (*save_ctx)(struct iommu *obj);
	void (*restore_ctx)(struct iommu *obj);
	ssize_t (*dump_ctx)(struct iommu *obj, char *buf, ssize_t len);
};

struct iommu_platform_data {
	const char *name;
	const char *clk_name;
	const int nr_tlb_entries;
};

#if defined(CONFIG_ARCH_OMAP1)
#error "iommu for this processor not implemented yet"
#else
#include <plat/iommu2.h>
#endif

/*
 * utilities for super page(16MB, 1MB, 64KB and 4KB)
 */

#define iopgsz_max(bytes)			\
	(((bytes) >= SZ_16M) ? SZ_16M :		\
	 ((bytes) >= SZ_1M)  ? SZ_1M  :		\
	 ((bytes) >= SZ_64K) ? SZ_64K :		\
	 ((bytes) >= SZ_4K)  ? SZ_4K  :	0)

#define bytes_to_iopgsz(bytes)				\
	(((bytes) == SZ_16M) ? MMU_CAM_PGSZ_16M :	\
	 ((bytes) == SZ_1M)  ? MMU_CAM_PGSZ_1M  :	\
	 ((bytes) == SZ_64K) ? MMU_CAM_PGSZ_64K :	\
	 ((bytes) == SZ_4K)  ? MMU_CAM_PGSZ_4K  : -1)

#define iopgsz_to_bytes(iopgsz)				\
	(((iopgsz) == MMU_CAM_PGSZ_16M)	? SZ_16M :	\
	 ((iopgsz) == MMU_CAM_PGSZ_1M)	? SZ_1M  :	\
	 ((iopgsz) == MMU_CAM_PGSZ_64K)	? SZ_64K :	\
	 ((iopgsz) == MMU_CAM_PGSZ_4K)	? SZ_4K  : 0)

#define iopgsz_ok(bytes) (bytes_to_iopgsz(bytes) >= 0)

/*
 * global functions
 */
extern u32 iommu_arch_version(void);

extern void iotlb_cr_to_e(struct cr_regs *cr, struct iotlb_entry *e);
extern u32 iotlb_cr_to_virt(struct cr_regs *cr);

extern int load_iotlb_entry(struct iommu *obj, struct iotlb_entry *e);
extern void flush_iotlb_page(struct iommu *obj, u32 da);
extern void flush_iotlb_range(struct iommu *obj, u32 start, u32 end);
extern void flush_iotlb_all(struct iommu *obj);

extern int iopgtable_store_entry(struct iommu *obj, struct iotlb_entry *e);
extern size_t iopgtable_clear_entry(struct iommu *obj, u32 iova);

extern struct iommu *iommu_get(const char *name);
extern void iommu_put(struct iommu *obj);

extern void iommu_save_ctx(struct iommu *obj);
extern void iommu_restore_ctx(struct iommu *obj);

extern int install_iommu_arch(const struct iommu_functions *ops);
extern void uninstall_iommu_arch(const struct iommu_functions *ops);

extern int foreach_iommu_device(void *data,
				int (*fn)(struct device *, void *));

extern ssize_t iommu_dump_ctx(struct iommu *obj, char *buf, ssize_t len);
extern size_t dump_tlb_entries(struct iommu *obj, char *buf, ssize_t len);

#endif /* __MACH_IOMMU_H */
