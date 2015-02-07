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
 *  eseries-gpio.h
 *
 *  Copyright (C) Ian Molton <spyro@f2s.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 */

/* e-series power button */
#define GPIO_ESERIES_POWERBTN     0

/* UDC GPIO definitions */
#define GPIO_E7XX_USB_DISC       13
#define GPIO_E7XX_USB_PULLUP      3

#define GPIO_E800_USB_DISC        4
#define GPIO_E800_USB_PULLUP     84

/* e740 PCMCIA GPIO definitions */
/* Note: PWR1 seems to be inverted */
#define GPIO_E740_PCMCIA_CD0      8
#define GPIO_E740_PCMCIA_CD1     44
#define GPIO_E740_PCMCIA_RDY0    11
#define GPIO_E740_PCMCIA_RDY1     6
#define GPIO_E740_PCMCIA_RST0    27
#define GPIO_E740_PCMCIA_RST1    24
#define GPIO_E740_PCMCIA_PWR0    20
#define GPIO_E740_PCMCIA_PWR1    23

/* e750 PCMCIA GPIO definitions */
#define GPIO_E750_PCMCIA_CD0      8
#define GPIO_E750_PCMCIA_RDY0    12
#define GPIO_E750_PCMCIA_RST0    27
#define GPIO_E750_PCMCIA_PWR0    20

/* e800 PCMCIA GPIO definitions */
#define GPIO_E800_PCMCIA_RST0    69
#define GPIO_E800_PCMCIA_RST1    72
#define GPIO_E800_PCMCIA_PWR0    20
#define GPIO_E800_PCMCIA_PWR1    73

/* e7xx IrDA power control */
#define GPIO_E7XX_IR_OFF         38

/* e740 audio control GPIOs */
#define GPIO_E740_WM9705_nAVDD2  16
#define GPIO_E740_MIC_ON         40
#define GPIO_E740_AMP_ON         41

/* e750 audio control GPIOs */
#define GPIO_E750_HP_AMP_OFF      4
#define GPIO_E750_SPK_AMP_OFF     7
#define GPIO_E750_HP_DETECT      37

/* e800 audio control GPIOs */
#define GPIO_E800_HP_DETECT      81
#define GPIO_E800_HP_AMP_OFF     82
#define GPIO_E800_SPK_AMP_ON     83

/* ASIC related GPIOs */
#define GPIO_ESERIES_TMIO_IRQ        5
#define GPIO_ESERIES_TMIO_PCLR      19
#define GPIO_ESERIES_TMIO_SUSPEND   45
#define GPIO_E800_ANGELX_IRQ      8
