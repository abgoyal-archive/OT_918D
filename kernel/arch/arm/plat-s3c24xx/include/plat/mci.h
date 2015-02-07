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

#ifndef _ARCH_MCI_H
#define _ARCH_MCI_H

/**
 * struct s3c24xx_mci_pdata - sd/mmc controller platform data
 * @no_wprotect: Set this to indicate there is no write-protect switch.
 * @no_detect: Set this if there is no detect switch.
 * @wprotect_invert: Invert the default sense of the write protect switch.
 * @detect_invert: Invert the default sense of the write protect switch.
 * @use_dma: Set to allow the use of DMA.
 * @gpio_detect: GPIO number for the card detect line.
 * @gpio_wprotect: GPIO number for the write protect line.
 * @ocr_avail: The mask of the available power states, non-zero to use.
 * @set_power: Callback to control the power mode.
 *
 * The @gpio_detect is used for card detection when @no_wprotect is unset,
 * and the default sense is that 0 returned from gpio_get_value() means
 * that a card is inserted. If @detect_invert is set, then the value from
 * gpio_get_value() is inverted, which makes 1 mean card inserted.
 *
 * The driver will use @gpio_wprotect to signal whether the card is write
 * protected if @no_wprotect is not set. A 0 returned from gpio_get_value()
 * means the card is read/write, and 1 means read-only. The @wprotect_invert
 * will invert the value returned from gpio_get_value().
 *
 * Card power is set by @ocr_availa, using MCC_VDD_ constants if it is set
 * to a non-zero value, otherwise the default of 3.2-3.4V is used.
 */
struct s3c24xx_mci_pdata {
	unsigned int	no_wprotect : 1;
	unsigned int	no_detect : 1;
	unsigned int	wprotect_invert : 1;
	unsigned int	detect_invert : 1;   /* set => detect active high. */
	unsigned int	use_dma : 1;

	unsigned int	gpio_detect;
	unsigned int	gpio_wprotect;
	unsigned long	ocr_avail;
	void		(*set_power)(unsigned char power_mode,
				     unsigned short vdd);
};

/**
 * s3c24xx_mci_set_platdata - set platform data for mmc/sdi device
 * @pdata: The platform data
 *
 * Copy the platform data supplied by @pdata so that this can be marked
 * __initdata.
 */
extern void s3c24xx_mci_set_platdata(struct s3c24xx_mci_pdata *pdata);

#endif /* _ARCH_NCI_H */
