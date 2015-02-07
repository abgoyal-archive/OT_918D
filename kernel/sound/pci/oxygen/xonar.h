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

#ifndef XONAR_H_INCLUDED
#define XONAR_H_INCLUDED

#include "oxygen.h"

struct xonar_generic {
	unsigned int anti_pop_delay;
	u16 output_enable_bit;
	u8 ext_power_reg;
	u8 ext_power_int_reg;
	u8 ext_power_bit;
	u8 has_power;
};

struct xonar_hdmi {
	u8 params[5];
};

/* generic helper functions */

void xonar_enable_output(struct oxygen *chip);
void xonar_disable_output(struct oxygen *chip);
void xonar_init_ext_power(struct oxygen *chip);
void xonar_init_cs53x1(struct oxygen *chip);
void xonar_set_cs53x1_params(struct oxygen *chip,
			     struct snd_pcm_hw_params *params);
int xonar_gpio_bit_switch_get(struct snd_kcontrol *ctl,
			      struct snd_ctl_elem_value *value);
int xonar_gpio_bit_switch_put(struct snd_kcontrol *ctl,
			      struct snd_ctl_elem_value *value);

/* model-specific card drivers */

int get_xonar_pcm179x_model(struct oxygen *chip,
			    const struct pci_device_id *id);
int get_xonar_cs43xx_model(struct oxygen *chip,
			   const struct pci_device_id *id);
int get_xonar_wm87x6_model(struct oxygen *chip,
			   const struct pci_device_id *id);

/* HDMI helper functions */

void xonar_hdmi_init(struct oxygen *chip, struct xonar_hdmi *data);
void xonar_hdmi_cleanup(struct oxygen *chip);
void xonar_hdmi_resume(struct oxygen *chip, struct xonar_hdmi *hdmi);
void xonar_hdmi_pcm_hardware_filter(unsigned int channel,
				    struct snd_pcm_hardware *hardware);
void xonar_set_hdmi_params(struct oxygen *chip, struct xonar_hdmi *hdmi,
			   struct snd_pcm_hw_params *params);
void xonar_hdmi_uart_input(struct oxygen *chip);

#endif
