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
 *  fixup-tb0226.c, The TANBAC TB0226 specific PCI fixups.
 *
 *  Copyright (C) 2002-2005  Yoichi Yuasa <yuasa@linux-mips.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/init.h>
#include <linux/pci.h>

#include <asm/vr41xx/giu.h>
#include <asm/vr41xx/tb0226.h>

int __init pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	int irq = -1;

	switch (slot) {
	case 12:
		vr41xx_set_irq_trigger(GD82559_1_PIN,
				       IRQ_TRIGGER_LEVEL,
				       IRQ_SIGNAL_THROUGH);
		vr41xx_set_irq_level(GD82559_1_PIN, IRQ_LEVEL_LOW);
		irq = GD82559_1_IRQ;
		break;
	case 13:
		vr41xx_set_irq_trigger(GD82559_2_PIN,
				       IRQ_TRIGGER_LEVEL,
				       IRQ_SIGNAL_THROUGH);
		vr41xx_set_irq_level(GD82559_2_PIN, IRQ_LEVEL_LOW);
		irq = GD82559_2_IRQ;
		break;
	case 14:
		switch (pin) {
		case 1:
			vr41xx_set_irq_trigger(UPD720100_INTA_PIN,
					       IRQ_TRIGGER_LEVEL,
					       IRQ_SIGNAL_THROUGH);
			vr41xx_set_irq_level(UPD720100_INTA_PIN,
					     IRQ_LEVEL_LOW);
			irq = UPD720100_INTA_IRQ;
			break;
		case 2:
			vr41xx_set_irq_trigger(UPD720100_INTB_PIN,
					       IRQ_TRIGGER_LEVEL,
					       IRQ_SIGNAL_THROUGH);
			vr41xx_set_irq_level(UPD720100_INTB_PIN,
					     IRQ_LEVEL_LOW);
			irq = UPD720100_INTB_IRQ;
			break;
		case 3:
			vr41xx_set_irq_trigger(UPD720100_INTC_PIN,
					       IRQ_TRIGGER_LEVEL,
					       IRQ_SIGNAL_THROUGH);
			vr41xx_set_irq_level(UPD720100_INTC_PIN,
					     IRQ_LEVEL_LOW);
			irq = UPD720100_INTC_IRQ;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return irq;
}

/* Do platform specific device initialization at pci_enable_device() time */
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}
