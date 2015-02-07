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

#ifndef BCM63XX_PCMCIA_H_
#define BCM63XX_PCMCIA_H_

#include <linux/types.h>
#include <linux/timer.h>
#include <pcmcia/ss.h>
#include <bcm63xx_dev_pcmcia.h>

/* socket polling rate in ms */
#define BCM63XX_PCMCIA_POLL_RATE	500

enum {
	CARD_CARDBUS = (1 << 0),
	CARD_PCCARD = (1 << 1),
	CARD_5V = (1 << 2),
	CARD_3V = (1 << 3),
	CARD_XV = (1 << 4),
	CARD_YV = (1 << 5),
};

struct bcm63xx_pcmcia_socket {
	struct pcmcia_socket socket;

	/* platform specific data */
	struct bcm63xx_pcmcia_platform_data *pd;

	/* all regs access are protected by this spinlock */
	spinlock_t lock;

	/* pcmcia registers resource */
	struct resource *reg_res;

	/* base remapped address of registers */
	void __iomem *base;

	/* whether a card is detected at the moment */
	int card_detected;

	/* type of detected card (mask of above enum) */
	u8 card_type;

	/* keep last socket status to implement event reporting */
	unsigned int old_status;

	/* backup of requested socket state */
	socket_state_t requested_state;

	/* timer used for socket status polling */
	struct timer_list timer;

	/* attribute/common memory resources */
	struct resource *attr_res;
	struct resource *common_res;
	struct resource *io_res;

	/* base address of io memory */
	void __iomem *io_base;
};

#endif /* BCM63XX_PCMCIA_H_ */
