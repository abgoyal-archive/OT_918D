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

#ifndef __PLAT_GPIO_H
#define __PLAT_GPIO_H

/*
 * We handle the GPIOs by banks, each bank covers up to 32 GPIOs with
 * one set of registers. The register offsets are organized below:
 *
 *           GPLR    GPDR    GPSR    GPCR    GRER    GFER    GEDR
 * BANK 0 - 0x0000  0x000C  0x0018  0x0024  0x0030  0x003C  0x0048
 * BANK 1 - 0x0004  0x0010  0x001C  0x0028  0x0034  0x0040  0x004C
 * BANK 2 - 0x0008  0x0014  0x0020  0x002C  0x0038  0x0044  0x0050
 *
 * BANK 3 - 0x0100  0x010C  0x0118  0x0124  0x0130  0x013C  0x0148
 * BANK 4 - 0x0104  0x0110  0x011C  0x0128  0x0134  0x0140  0x014C
 * BANK 5 - 0x0108  0x0114  0x0120  0x012C  0x0138  0x0144  0x0150
 *
 * NOTE:
 *   BANK 3 is only available on PXA27x and later processors.
 *   BANK 4 and 5 are only available on PXA935
 */

#define GPIO_BANK(n)	(GPIO_REGS_VIRT + BANK_OFF(n))

#define GPLR_OFFSET	0x00
#define GPDR_OFFSET	0x0C
#define GPSR_OFFSET	0x18
#define GPCR_OFFSET	0x24
#define GRER_OFFSET	0x30
#define GFER_OFFSET	0x3C
#define GEDR_OFFSET	0x48

static inline int gpio_get_value(unsigned gpio)
{
	if (__builtin_constant_p(gpio) && (gpio < NR_BUILTIN_GPIO))
		return GPLR(gpio) & GPIO_bit(gpio);
	else
		return __gpio_get_value(gpio);
}

static inline void gpio_set_value(unsigned gpio, int value)
{
	if (__builtin_constant_p(gpio) && (gpio < NR_BUILTIN_GPIO)) {
		if (value)
			GPSR(gpio) = GPIO_bit(gpio);
		else
			GPCR(gpio) = GPIO_bit(gpio);
	} else
		__gpio_set_value(gpio, value);
}

#define gpio_cansleep		__gpio_cansleep

/* NOTE: some PXAs have fewer on-chip GPIOs (like PXA255, with 85).
 * Those cases currently cause holes in the GPIO number space, the
 * actual number of the last GPIO is recorded by 'pxa_last_gpio'.
 */
extern int pxa_last_gpio;

typedef int (*set_wake_t)(unsigned int irq, unsigned int on);

extern void pxa_init_gpio(int mux_irq, int start, int end, set_wake_t fn);
#endif /* __PLAT_GPIO_H */
