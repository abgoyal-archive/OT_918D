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
 * Platform data for WM8904
 *
 * Copyright 2009 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#ifndef __MFD_WM8994_PDATA_H__
#define __MFD_WM8994_PDATA_H__

/* Used to enable configuration of a GPIO to all zeros */
#define WM8904_GPIO_NO_CONFIG 0x8000

/*
 * R6 (0x06) - Mic Bias Control 0
 */
#define WM8904_MICDET_THR_MASK                  0x0070  /* MICDET_THR - [6:4] */
#define WM8904_MICDET_THR_SHIFT                      4  /* MICDET_THR - [6:4] */
#define WM8904_MICDET_THR_WIDTH                      3  /* MICDET_THR - [6:4] */
#define WM8904_MICSHORT_THR_MASK                0x000C  /* MICSHORT_THR - [3:2] */
#define WM8904_MICSHORT_THR_SHIFT                    2  /* MICSHORT_THR - [3:2] */
#define WM8904_MICSHORT_THR_WIDTH                    2  /* MICSHORT_THR - [3:2] */
#define WM8904_MICDET_ENA                       0x0002  /* MICDET_ENA */
#define WM8904_MICDET_ENA_MASK                  0x0002  /* MICDET_ENA */
#define WM8904_MICDET_ENA_SHIFT                      1  /* MICDET_ENA */
#define WM8904_MICDET_ENA_WIDTH                      1  /* MICDET_ENA */
#define WM8904_MICBIAS_ENA                      0x0001  /* MICBIAS_ENA */
#define WM8904_MICBIAS_ENA_MASK                 0x0001  /* MICBIAS_ENA */
#define WM8904_MICBIAS_ENA_SHIFT                     0  /* MICBIAS_ENA */
#define WM8904_MICBIAS_ENA_WIDTH                     1  /* MICBIAS_ENA */

/*
 * R7 (0x07) - Mic Bias Control 1
 */
#define WM8904_MIC_DET_FILTER_ENA               0x8000  /* MIC_DET_FILTER_ENA */
#define WM8904_MIC_DET_FILTER_ENA_MASK          0x8000  /* MIC_DET_FILTER_ENA */
#define WM8904_MIC_DET_FILTER_ENA_SHIFT             15  /* MIC_DET_FILTER_ENA */
#define WM8904_MIC_DET_FILTER_ENA_WIDTH              1  /* MIC_DET_FILTER_ENA */
#define WM8904_MIC_SHORT_FILTER_ENA             0x4000  /* MIC_SHORT_FILTER_ENA */
#define WM8904_MIC_SHORT_FILTER_ENA_MASK        0x4000  /* MIC_SHORT_FILTER_ENA */
#define WM8904_MIC_SHORT_FILTER_ENA_SHIFT           14  /* MIC_SHORT_FILTER_ENA */
#define WM8904_MIC_SHORT_FILTER_ENA_WIDTH            1  /* MIC_SHORT_FILTER_ENA */
#define WM8904_MICBIAS_SEL_MASK                 0x0007  /* MICBIAS_SEL - [2:0] */
#define WM8904_MICBIAS_SEL_SHIFT                     0  /* MICBIAS_SEL - [2:0] */
#define WM8904_MICBIAS_SEL_WIDTH                     3  /* MICBIAS_SEL - [2:0] */


/*
 * R121 (0x79) - GPIO Control 1
 */
#define WM8904_GPIO1_PU                         0x0020  /* GPIO1_PU */
#define WM8904_GPIO1_PU_MASK                    0x0020  /* GPIO1_PU */
#define WM8904_GPIO1_PU_SHIFT                        5  /* GPIO1_PU */
#define WM8904_GPIO1_PU_WIDTH                        1  /* GPIO1_PU */
#define WM8904_GPIO1_PD                         0x0010  /* GPIO1_PD */
#define WM8904_GPIO1_PD_MASK                    0x0010  /* GPIO1_PD */
#define WM8904_GPIO1_PD_SHIFT                        4  /* GPIO1_PD */
#define WM8904_GPIO1_PD_WIDTH                        1  /* GPIO1_PD */
#define WM8904_GPIO1_SEL_MASK                   0x000F  /* GPIO1_SEL - [3:0] */
#define WM8904_GPIO1_SEL_SHIFT                       0  /* GPIO1_SEL - [3:0] */
#define WM8904_GPIO1_SEL_WIDTH                       4  /* GPIO1_SEL - [3:0] */

/*
 * R122 (0x7A) - GPIO Control 2
 */
#define WM8904_GPIO2_PU                         0x0020  /* GPIO2_PU */
#define WM8904_GPIO2_PU_MASK                    0x0020  /* GPIO2_PU */
#define WM8904_GPIO2_PU_SHIFT                        5  /* GPIO2_PU */
#define WM8904_GPIO2_PU_WIDTH                        1  /* GPIO2_PU */
#define WM8904_GPIO2_PD                         0x0010  /* GPIO2_PD */
#define WM8904_GPIO2_PD_MASK                    0x0010  /* GPIO2_PD */
#define WM8904_GPIO2_PD_SHIFT                        4  /* GPIO2_PD */
#define WM8904_GPIO2_PD_WIDTH                        1  /* GPIO2_PD */
#define WM8904_GPIO2_SEL_MASK                   0x000F  /* GPIO2_SEL - [3:0] */
#define WM8904_GPIO2_SEL_SHIFT                       0  /* GPIO2_SEL - [3:0] */
#define WM8904_GPIO2_SEL_WIDTH                       4  /* GPIO2_SEL - [3:0] */

/*
 * R123 (0x7B) - GPIO Control 3
 */
#define WM8904_GPIO3_PU                         0x0020  /* GPIO3_PU */
#define WM8904_GPIO3_PU_MASK                    0x0020  /* GPIO3_PU */
#define WM8904_GPIO3_PU_SHIFT                        5  /* GPIO3_PU */
#define WM8904_GPIO3_PU_WIDTH                        1  /* GPIO3_PU */
#define WM8904_GPIO3_PD                         0x0010  /* GPIO3_PD */
#define WM8904_GPIO3_PD_MASK                    0x0010  /* GPIO3_PD */
#define WM8904_GPIO3_PD_SHIFT                        4  /* GPIO3_PD */
#define WM8904_GPIO3_PD_WIDTH                        1  /* GPIO3_PD */
#define WM8904_GPIO3_SEL_MASK                   0x000F  /* GPIO3_SEL - [3:0] */
#define WM8904_GPIO3_SEL_SHIFT                       0  /* GPIO3_SEL - [3:0] */
#define WM8904_GPIO3_SEL_WIDTH                       4  /* GPIO3_SEL - [3:0] */

/*
 * R124 (0x7C) - GPIO Control 4
 */
#define WM8904_GPI7_ENA                         0x0200  /* GPI7_ENA */
#define WM8904_GPI7_ENA_MASK                    0x0200  /* GPI7_ENA */
#define WM8904_GPI7_ENA_SHIFT                        9  /* GPI7_ENA */
#define WM8904_GPI7_ENA_WIDTH                        1  /* GPI7_ENA */
#define WM8904_GPI8_ENA                         0x0100  /* GPI8_ENA */
#define WM8904_GPI8_ENA_MASK                    0x0100  /* GPI8_ENA */
#define WM8904_GPI8_ENA_SHIFT                        8  /* GPI8_ENA */
#define WM8904_GPI8_ENA_WIDTH                        1  /* GPI8_ENA */
#define WM8904_GPIO_BCLK_MODE_ENA               0x0080  /* GPIO_BCLK_MODE_ENA */
#define WM8904_GPIO_BCLK_MODE_ENA_MASK          0x0080  /* GPIO_BCLK_MODE_ENA */
#define WM8904_GPIO_BCLK_MODE_ENA_SHIFT              7  /* GPIO_BCLK_MODE_ENA */
#define WM8904_GPIO_BCLK_MODE_ENA_WIDTH              1  /* GPIO_BCLK_MODE_ENA */
#define WM8904_GPIO_BCLK_SEL_MASK               0x000F  /* GPIO_BCLK_SEL - [3:0] */
#define WM8904_GPIO_BCLK_SEL_SHIFT                   0  /* GPIO_BCLK_SEL - [3:0] */
#define WM8904_GPIO_BCLK_SEL_WIDTH                   4  /* GPIO_BCLK_SEL - [3:0] */

#define WM8904_MIC_REGS  2
#define WM8904_GPIO_REGS 4
#define WM8904_DRC_REGS  4
#define WM8904_EQ_REGS   25

/**
 * DRC configurations are specified with a label and a set of register
 * values to write (the enable bits will be ignored).  At runtime an
 * enumerated control will be presented for each DRC block allowing
 * the user to choose the configration to use.
 *
 * Configurations may be generated by hand or by using the DRC control
 * panel provided by the WISCE - see  http://www.wolfsonmicro.com/wisce/
 * for details.
 */
struct wm8904_drc_cfg {
	const char *name;
	u16 regs[WM8904_DRC_REGS];
};

/**
 * ReTune Mobile configurations are specified with a label, sample
 * rate and set of values to write (the enable bits will be ignored).
 *
 * Configurations are expected to be generated using the ReTune Mobile
 * control panel in WISCE - see http://www.wolfsonmicro.com/wisce/
 */
struct wm8904_retune_mobile_cfg {
	const char *name;
	unsigned int rate;
	u16 regs[WM8904_EQ_REGS];
};

struct wm8904_pdata {
	int num_drc_cfgs;
	struct wm8904_drc_cfg *drc_cfgs;

	int num_retune_mobile_cfgs;
	struct wm8904_retune_mobile_cfg *retune_mobile_cfgs;

	u32 gpio_cfg[WM8904_GPIO_REGS];
	u32 mic_cfg[WM8904_MIC_REGS];
};

#endif
