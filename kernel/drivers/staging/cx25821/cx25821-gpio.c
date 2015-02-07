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
 *  Driver for the Conexant CX25821 PCIe bridge
 *
 *  Copyright (C) 2009 Conexant Systems Inc.
 *  Authors  <shu.lin@conexant.com>, <hiep.huynh@conexant.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cx25821.h"

/********************* GPIO stuffs *********************/
void cx25821_set_gpiopin_direction(struct cx25821_dev *dev,
				   int pin_number, int pin_logic_value)
{
	int bit = pin_number;
	u32 gpio_oe_reg = GPIO_LO_OE;
	u32 gpio_register = 0;
	u32 value = 0;

	/* Check for valid pinNumber */
	if (pin_number >= 47)
		return;

	if (pin_number > 31) {
		bit = pin_number - 31;
		gpio_oe_reg = GPIO_HI_OE;
	}
	/* Here we will make sure that the GPIOs 0 and 1 are output. keep the
	 * rest as is */
	gpio_register = cx_read(gpio_oe_reg);

	if (pin_logic_value == 1)
		value = gpio_register | Set_GPIO_Bit(bit);
	else
		value = gpio_register & Clear_GPIO_Bit(bit);

	cx_write(gpio_oe_reg, value);
}

static void cx25821_set_gpiopin_logicvalue(struct cx25821_dev *dev,
					   int pin_number, int pin_logic_value)
{
	int bit = pin_number;
	u32 gpio_reg = GPIO_LO;
	u32 value = 0;

	/* Check for valid pinNumber */
	if (pin_number >= 47)
		return;

	/* change to output direction */
	cx25821_set_gpiopin_direction(dev, pin_number, 0);

	if (pin_number > 31) {
		bit = pin_number - 31;
		gpio_reg = GPIO_HI;
	}

	value = cx_read(gpio_reg);

	if (pin_logic_value == 0)
		value &= Clear_GPIO_Bit(bit);
	else
		value |= Set_GPIO_Bit(bit);

	cx_write(gpio_reg, value);
}

void cx25821_gpio_init(struct cx25821_dev *dev)
{
	if (dev == NULL)
		return;

	switch (dev->board) {
	case CX25821_BOARD_CONEXANT_ATHENA10:
	default:
		/* set GPIO 5 to select the path for Medusa/Athena */
		cx25821_set_gpiopin_logicvalue(dev, 5, 1);
		mdelay(20);
		break;
	}

}
