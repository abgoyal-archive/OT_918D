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

#ifndef B43_PIO_H_
#define B43_PIO_H_

#include "b43.h"

#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/list.h>
#include <linux/skbuff.h>


/*** Registers for PIO queues up to revision 7. ***/
/* TX queue. */
#define B43_PIO_TXCTL			0x00
#define  B43_PIO_TXCTL_WRITELO		0x0001
#define  B43_PIO_TXCTL_WRITEHI		0x0002
#define  B43_PIO_TXCTL_EOF		0x0004
#define  B43_PIO_TXCTL_FREADY		0x0008
#define  B43_PIO_TXCTL_FLUSHREQ		0x0020
#define  B43_PIO_TXCTL_FLUSHPEND	0x0040
#define  B43_PIO_TXCTL_SUSPREQ		0x0080
#define  B43_PIO_TXCTL_QSUSP		0x0100
#define  B43_PIO_TXCTL_COMMCNT		0xFC00
#define  B43_PIO_TXCTL_COMMCNT_SHIFT	10
#define B43_PIO_TXDATA			0x02
#define B43_PIO_TXQBUFSIZE		0x04
/* RX queue. */
#define B43_PIO_RXCTL			0x00
#define  B43_PIO_RXCTL_FRAMERDY		0x0001
#define  B43_PIO_RXCTL_DATARDY		0x0002
#define B43_PIO_RXDATA			0x02

/*** Registers for PIO queues revision 8 and later. ***/
/* TX queue */
#define B43_PIO8_TXCTL			0x00
#define  B43_PIO8_TXCTL_0_7		0x00000001
#define  B43_PIO8_TXCTL_8_15		0x00000002
#define  B43_PIO8_TXCTL_16_23		0x00000004
#define  B43_PIO8_TXCTL_24_31		0x00000008
#define  B43_PIO8_TXCTL_EOF		0x00000010
#define  B43_PIO8_TXCTL_FREADY		0x00000080
#define  B43_PIO8_TXCTL_SUSPREQ		0x00000100
#define  B43_PIO8_TXCTL_QSUSP		0x00000200
#define  B43_PIO8_TXCTL_FLUSHREQ	0x00000400
#define  B43_PIO8_TXCTL_FLUSHPEND	0x00000800
#define B43_PIO8_TXDATA			0x04
/* RX queue */
#define B43_PIO8_RXCTL			0x00
#define  B43_PIO8_RXCTL_FRAMERDY	0x00000001
#define  B43_PIO8_RXCTL_DATARDY		0x00000002
#define B43_PIO8_RXDATA			0x04


/* The maximum number of TX-packets the HW can handle. */
#define B43_PIO_MAX_NR_TXPACKETS	32


struct b43_pio_txpacket {
	/* Pointer to the TX queue we belong to. */
	struct b43_pio_txqueue *queue;
	/* The TX data packet. */
	struct sk_buff *skb;
	/* Index in the (struct b43_pio_txqueue)->packets array. */
	u8 index;

	struct list_head list;
};

struct b43_pio_txqueue {
	struct b43_wldev *dev;
	u16 mmio_base;

	/* The device queue buffer size in bytes. */
	u16 buffer_size;
	/* The number of used bytes in the device queue buffer. */
	u16 buffer_used;
	/* The number of packets that can still get queued.
	 * This is decremented on queueing a packet and incremented
	 * after receiving the transmit status. */
	u16 free_packet_slots;

	/* True, if the mac80211 queue was stopped due to overflow at TX. */
	bool stopped;
	/* Our b43 queue index number */
	u8 index;
	/* The mac80211 QoS queue priority. */
	u8 queue_prio;

	/* Buffer for TX packet meta data. */
	struct b43_pio_txpacket packets[B43_PIO_MAX_NR_TXPACKETS];
	struct list_head packets_list;

	/* Shortcut to the 802.11 core revision. This is to
	 * avoid horrible pointer dereferencing in the fastpaths. */
	u8 rev;
};

struct b43_pio_rxqueue {
	struct b43_wldev *dev;
	u16 mmio_base;

	/* Shortcut to the 802.11 core revision. This is to
	 * avoid horrible pointer dereferencing in the fastpaths. */
	u8 rev;
};


static inline u16 b43_piotx_read16(struct b43_pio_txqueue *q, u16 offset)
{
	return b43_read16(q->dev, q->mmio_base + offset);
}

static inline u32 b43_piotx_read32(struct b43_pio_txqueue *q, u16 offset)
{
	return b43_read32(q->dev, q->mmio_base + offset);
}

static inline void b43_piotx_write16(struct b43_pio_txqueue *q,
				     u16 offset, u16 value)
{
	b43_write16(q->dev, q->mmio_base + offset, value);
}

static inline void b43_piotx_write32(struct b43_pio_txqueue *q,
				     u16 offset, u32 value)
{
	b43_write32(q->dev, q->mmio_base + offset, value);
}


static inline u16 b43_piorx_read16(struct b43_pio_rxqueue *q, u16 offset)
{
	return b43_read16(q->dev, q->mmio_base + offset);
}

static inline u32 b43_piorx_read32(struct b43_pio_rxqueue *q, u16 offset)
{
	return b43_read32(q->dev, q->mmio_base + offset);
}

static inline void b43_piorx_write16(struct b43_pio_rxqueue *q,
				     u16 offset, u16 value)
{
	b43_write16(q->dev, q->mmio_base + offset, value);
}

static inline void b43_piorx_write32(struct b43_pio_rxqueue *q,
				     u16 offset, u32 value)
{
	b43_write32(q->dev, q->mmio_base + offset, value);
}


int b43_pio_init(struct b43_wldev *dev);
void b43_pio_free(struct b43_wldev *dev);

int b43_pio_tx(struct b43_wldev *dev, struct sk_buff *skb);
void b43_pio_handle_txstatus(struct b43_wldev *dev,
			     const struct b43_txstatus *status);
void b43_pio_rx(struct b43_pio_rxqueue *q);

void b43_pio_tx_suspend(struct b43_wldev *dev);
void b43_pio_tx_resume(struct b43_wldev *dev);

#endif /* B43_PIO_H_ */
