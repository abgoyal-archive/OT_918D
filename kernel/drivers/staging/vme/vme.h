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

#ifndef _VME_H_
#define _VME_H_

/* Resource Type */
enum vme_resource_type {
	VME_MASTER,
	VME_SLAVE,
	VME_DMA,
	VME_LM
};

/* VME Address Spaces */
typedef u32 vme_address_t;
#define VME_A16		0x1
#define VME_A24		0x2
#define	VME_A32		0x4
#define VME_A64		0x8
#define VME_CRCSR	0x10
#define VME_USER1	0x20
#define VME_USER2	0x40
#define VME_USER3	0x80
#define VME_USER4	0x100

#define VME_A16_MAX	0x10000ULL
#define VME_A24_MAX	0x1000000ULL
#define VME_A32_MAX	0x100000000ULL
#define VME_A64_MAX	0x10000000000000000ULL
#define VME_CRCSR_MAX	0x1000000ULL


/* VME Cycle Types */
typedef u32 vme_cycle_t;
#define VME_SCT		0x1
#define VME_BLT		0x2
#define VME_MBLT	0x4
#define VME_2eVME	0x8
#define VME_2eSST	0x10
#define VME_2eSSTB	0x20

#define VME_2eSST160	0x100
#define VME_2eSST267	0x200
#define VME_2eSST320	0x400

#define	VME_SUPER	0x1000
#define	VME_USER	0x2000
#define	VME_PROG	0x4000
#define	VME_DATA	0x8000

/* VME Data Widths */
typedef u32 vme_width_t;
#define VME_D8		0x1
#define VME_D16		0x2
#define VME_D32		0x4
#define VME_D64		0x8

/* Arbitration Scheduling Modes */
typedef u32 vme_arbitration_t;
#define VME_R_ROBIN_MODE	0x1
#define VME_PRIORITY_MODE	0x2

typedef u32 vme_dma_t;
#define VME_DMA_PATTERN			(1<<0)
#define VME_DMA_PCI			(1<<1)
#define VME_DMA_VME			(1<<2)

typedef u32 vme_pattern_t;
#define VME_DMA_PATTERN_BYTE		(1<<0)
#define VME_DMA_PATTERN_WORD		(1<<1)
#define VME_DMA_PATTERN_INCREMENT	(1<<2)

typedef u32 vme_dma_route_t;
#define VME_DMA_VME_TO_MEM		(1<<0)
#define VME_DMA_MEM_TO_VME		(1<<1)
#define VME_DMA_VME_TO_VME		(1<<2)
#define VME_DMA_MEM_TO_MEM		(1<<3)
#define VME_DMA_PATTERN_TO_VME		(1<<4)
#define VME_DMA_PATTERN_TO_MEM		(1<<5)

struct vme_dma_attr {
	vme_dma_t type;
	void *private;
};

struct vme_resource {
	enum vme_resource_type type;
	struct list_head *entry;
};

extern struct bus_type vme_bus_type;

#define VME_SLOT_CURRENT	-1
#define VME_SLOT_ALL		-2

struct vme_device_id {
	int bus;
	int slot;
};

struct vme_driver {
	struct list_head node;
	char *name;
	const struct vme_device_id *bind_table;
	int (*probe)  (struct device *, int, int);
	int (*remove) (struct device *, int, int);
	void (*shutdown) (void);
	struct device_driver    driver;
};

void *vme_alloc_consistent(struct vme_resource *, size_t, dma_addr_t *);
void vme_free_consistent(struct vme_resource *, size_t,  void *,
	dma_addr_t);

size_t vme_get_size(struct vme_resource *);

struct vme_resource *vme_slave_request(struct device *, vme_address_t,
	vme_cycle_t);
int vme_slave_set(struct vme_resource *, int, unsigned long long,
	unsigned long long, dma_addr_t, vme_address_t, vme_cycle_t);
int vme_slave_get(struct vme_resource *, int *, unsigned long long *,
	unsigned long long *, dma_addr_t *, vme_address_t *, vme_cycle_t *);
void vme_slave_free(struct vme_resource *);

struct vme_resource *vme_master_request(struct device *, vme_address_t,
	vme_cycle_t, vme_width_t);
int vme_master_set(struct vme_resource *, int, unsigned long long,
	unsigned long long, vme_address_t, vme_cycle_t, vme_width_t);
int vme_master_get(struct vme_resource *, int *, unsigned long long *,
	unsigned long long *, vme_address_t *, vme_cycle_t *, vme_width_t *);
ssize_t vme_master_read(struct vme_resource *, void *, size_t, loff_t);
ssize_t vme_master_write(struct vme_resource *, void *, size_t, loff_t);
unsigned int vme_master_rmw(struct vme_resource *, unsigned int, unsigned int,
	unsigned int, loff_t);
void vme_master_free(struct vme_resource *);

struct vme_resource *vme_dma_request(struct device *, vme_dma_route_t);
struct vme_dma_list *vme_new_dma_list(struct vme_resource *);
struct vme_dma_attr *vme_dma_pattern_attribute(u32, vme_pattern_t);
struct vme_dma_attr *vme_dma_pci_attribute(dma_addr_t);
struct vme_dma_attr *vme_dma_vme_attribute(unsigned long long, vme_address_t,
	vme_cycle_t, vme_width_t);
void vme_dma_free_attribute(struct vme_dma_attr *);
int vme_dma_list_add(struct vme_dma_list *, struct vme_dma_attr *,
	struct vme_dma_attr *, size_t);
int vme_dma_list_exec(struct vme_dma_list *);
int vme_dma_list_free(struct vme_dma_list *);
int vme_dma_free(struct vme_resource *);

int vme_irq_request(struct device *, int, int,
	void (*callback)(int, int, void *), void *);
void vme_irq_free(struct device *, int, int);
int vme_irq_generate(struct device *, int, int);

struct vme_resource * vme_lm_request(struct device *);
int vme_lm_count(struct vme_resource *);
int vme_lm_set(struct vme_resource *, unsigned long long, vme_address_t,
	vme_cycle_t);
int vme_lm_get(struct vme_resource *, unsigned long long *, vme_address_t *,
	vme_cycle_t *);
int vme_lm_attach(struct vme_resource *, int, void (*callback)(int));
int vme_lm_detach(struct vme_resource *, int);
void vme_lm_free(struct vme_resource *);

int vme_slot_get(struct device *);

int vme_register_driver(struct vme_driver *);
void vme_unregister_driver(struct vme_driver *);


#endif /* _VME_H_ */

