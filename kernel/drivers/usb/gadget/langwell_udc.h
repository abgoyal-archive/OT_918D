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
 * Intel Langwell USB Device Controller driver
 * Copyright (C) 2008-2009, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <linux/usb/langwell_udc.h>

#if defined(CONFIG_USB_LANGWELL_OTG)
#include <linux/usb/langwell_otg.h>
#endif


/*-------------------------------------------------------------------------*/

/* driver data structures and utilities */

/*
 * dTD: Device Endpoint Transfer Descriptor
 * describe to the device controller the location and quantity of
 * data to be send/received for given transfer
 */
struct langwell_dtd {
	u32	dtd_next;
/* bits 31:5, next transfer element pointer */
#define	DTD_NEXT(d)	(((d)>>5)&0x7ffffff)
#define	DTD_NEXT_MASK	(0x7ffffff << 5)
/* terminate */
#define	DTD_TERM	BIT(0)
	/* bits 7:0, execution back states */
	u32	dtd_status:8;
#define	DTD_STATUS(d)	(((d)>>0)&0xff)
#define	DTD_STS_ACTIVE	BIT(7)	/* active */
#define	DTD_STS_HALTED	BIT(6)	/* halted */
#define	DTD_STS_DBE	BIT(5)	/* data buffer error */
#define	DTD_STS_TRE	BIT(3)	/* transaction error  */
	/* bits 9:8 */
	u32	dtd_res0:2;
	/* bits 11:10, multipier override */
	u32	dtd_multo:2;
#define	DTD_MULTO	(BIT(11) | BIT(10))
	/* bits 14:12 */
	u32	dtd_res1:3;
	/* bit 15, interrupt on complete */
	u32	dtd_ioc:1;
#define	DTD_IOC		BIT(15)
	/* bits 30:16, total bytes */
	u32	dtd_total:15;
#define	DTD_TOTAL(d)	(((d)>>16)&0x7fff)
#define	DTD_MAX_TRANSFER_LENGTH	0x4000
	/* bit 31 */
	u32	dtd_res2:1;
	/* dTD buffer pointer page 0 to 4 */
	u32	dtd_buf[5];
#define	DTD_OFFSET_MASK	0xfff
/* bits 31:12, buffer pointer */
#define	DTD_BUFFER(d)	(((d)>>12)&0x3ff)
/* bits 11:0, current offset */
#define	DTD_C_OFFSET(d)	(((d)>>0)&0xfff)
/* bits 10:0, frame number */
#define	DTD_FRAME(d)	(((d)>>0)&0x7ff)

	/* driver-private parts */

	/* dtd dma address */
	dma_addr_t		dtd_dma;
	/* next dtd virtual address */
	struct langwell_dtd	*next_dtd_virt;
};


/*
 * dQH: Device Endpoint Queue Head
 * describe where all transfers are managed
 * 48-byte data structure, aligned on 64-byte boundary
 *
 * These are associated with dTD structure
 */
struct langwell_dqh {
	/* endpoint capabilities and characteristics */
	u32	dqh_res0:15;	/* bits 14:0 */
	u32	dqh_ios:1;	/* bit 15, interrupt on setup */
#define	DQH_IOS		BIT(15)
	u32	dqh_mpl:11;	/* bits 26:16, maximum packet length */
#define	DQH_MPL		(0x7ff << 16)
	u32	dqh_res1:2;	/* bits 28:27 */
	u32	dqh_zlt:1;	/* bit 29, zero length termination */
#define	DQH_ZLT		BIT(29)
	u32	dqh_mult:2;	/* bits 31:30 */
#define	DQH_MULT	(BIT(30) | BIT(31))

	/* current dTD pointer */
	u32	dqh_current;	/* locate the transfer in progress */
#define DQH_C_DTD(e)	\
	(((e)>>5)&0x7ffffff)	/* bits 31:5, current dTD pointer */

	/* transfer overlay, hardware parts of a struct langwell_dtd */
	u32	dtd_next;
	u32	dtd_status:8;	/* bits 7:0, execution back states */
	u32	dtd_res0:2;	/* bits 9:8 */
	u32	dtd_multo:2;	/* bits 11:10, multipier override */
	u32	dtd_res1:3;	/* bits 14:12 */
	u32	dtd_ioc:1;	/* bit 15, interrupt on complete */
	u32	dtd_total:15;	/* bits 30:16, total bytes */
	u32	dtd_res2:1;	/* bit 31 */
	u32	dtd_buf[5];	/* dTD buffer pointer page 0 to 4 */

	u32	dqh_res2;
	struct usb_ctrlrequest	dqh_setup;	/* setup packet buffer */
} __attribute__ ((aligned(64)));


/* endpoint data structure */
struct langwell_ep {
	struct usb_ep		ep;
	dma_addr_t		dma;
	struct langwell_udc	*dev;
	unsigned long		irqs;
	struct list_head	queue;
	struct langwell_dqh	*dqh;
	const struct usb_endpoint_descriptor	*desc;
	char			name[14];
	unsigned		stopped:1,
				ep_type:2,
				ep_num:8;
};


/* request data structure */
struct langwell_request {
	struct usb_request	req;
	struct langwell_dtd	*dtd, *head, *tail;
	struct langwell_ep	*ep;
	dma_addr_t		dtd_dma;
	struct list_head	queue;
	unsigned		dtd_count;
	unsigned		mapped:1;
};


/* ep0 transfer state */
enum ep0_state {
	WAIT_FOR_SETUP,
	DATA_STATE_XMIT,
	DATA_STATE_NEED_ZLP,
	WAIT_FOR_OUT_STATUS,
	DATA_STATE_RECV,
};


/* device suspend state */
enum lpm_state {
	LPM_L0,	/* on */
	LPM_L1,	/* LPM L1 sleep */
	LPM_L2,	/* suspend */
	LPM_L3,	/* off */
};


/* device data structure */
struct langwell_udc {
	/* each pci device provides one gadget, several endpoints */
	struct usb_gadget	gadget;
	spinlock_t		lock;	/* device lock */
	struct langwell_ep	*ep;
	struct usb_gadget_driver	*driver;
	struct otg_transceiver	*transceiver;
	u8			dev_addr;
	u32			usb_state;
	u32			resume_state;
	u32			bus_reset;
	enum lpm_state		lpm_state;
	enum ep0_state		ep0_state;
	u32			ep0_dir;
	u16			dciversion;
	unsigned		ep_max;
	unsigned		devcap:1,
				enabled:1,
				region:1,
				got_irq:1,
				powered:1,
				remote_wakeup:1,
				rate:1,
				is_reset:1,
				softconnected:1,
				vbus_active:1,
				suspended:1,
				stopped:1,
				lpm:1;	/* LPM capability */

	/* pci state used to access those endpoints */
	struct pci_dev		*pdev;

	/* Langwell otg transceiver */
	struct langwell_otg	*lotg;

	/* control registers */
	struct langwell_cap_regs	__iomem	*cap_regs;
	struct langwell_op_regs		__iomem	*op_regs;

	struct usb_ctrlrequest	local_setup_buff;
	struct langwell_dqh	*ep_dqh;
	size_t			ep_dqh_size;
	dma_addr_t		ep_dqh_dma;

	/* ep0 status request */
	struct langwell_request	*status_req;

	/* dma pool */
	struct dma_pool		*dtd_pool;

	/* make sure release() is done */
	struct completion	*done;
};

