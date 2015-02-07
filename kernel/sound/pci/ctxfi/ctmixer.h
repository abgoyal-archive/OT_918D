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

/**
 * Copyright (C) 2008, Creative Technology Ltd. All Rights Reserved.
 *
 * This source file is released under GPL v2 license (no other versions).
 * See the COPYING file included in the main directory of this source
 * distribution for the license terms and conditions.
 *
 * @File	ctmixer.h
 *
 * @Brief
 * This file contains the definition of the mixer device functions.
 *
 * @Author	Liu Chun
 * @Date 	Mar 28 2008
 *
 */

#ifndef CTMIXER_H
#define CTMIXER_H

#include "ctatc.h"
#include "ctresource.h"

#define INIT_VOL	0x1c00

enum MIXER_PORT_T {
	MIX_WAVE_FRONT,
	MIX_WAVE_REAR,
	MIX_WAVE_CENTLFE,
	MIX_WAVE_SURROUND,
	MIX_SPDIF_OUT,
	MIX_PCMO_FRONT,
	MIX_MIC_IN,
	MIX_LINE_IN,
	MIX_SPDIF_IN,
	MIX_PCMI_FRONT,
	MIX_PCMI_REAR,
	MIX_PCMI_CENTLFE,
	MIX_PCMI_SURROUND,

	NUM_MIX_PORTS
};

/* alsa mixer descriptor */
struct ct_mixer {
	struct ct_atc *atc;

	void **amixers;		/* amixer resources for volume control */
	void **sums;		/* sum resources for signal collection */
	unsigned int switch_state; /* A bit-map to indicate state of switches */

	int (*get_output_ports)(struct ct_mixer *mixer, enum MIXER_PORT_T type,
				  struct rsc **rleft, struct rsc **rright);

	int (*set_input_left)(struct ct_mixer *mixer,
			      enum MIXER_PORT_T type, struct rsc *rsc);
	int (*set_input_right)(struct ct_mixer *mixer,
			       enum MIXER_PORT_T type, struct rsc *rsc);
#ifdef CONFIG_PM
	int (*resume)(struct ct_mixer *mixer);
#endif
};

int ct_alsa_mix_create(struct ct_atc *atc,
		       enum CTALSADEVS device,
		       const char *device_name);
int ct_mixer_create(struct ct_atc *atc, struct ct_mixer **rmixer);
int ct_mixer_destroy(struct ct_mixer *mixer);

#endif /* CTMIXER_H */
