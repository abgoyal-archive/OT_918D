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
 * Copyright (C) 2004,2005  ADDI-DATA GmbH for the source code of this module.
 *
 *	ADDI-DATA GmbH
 *	Dieselstrasse 3
 *	D-77833 Ottersweier
 *	Tel: +19(0)7223/9493-0
 *	Fax: +49(0)7223/9493-92
 *	http://www.addi-data-com
 *	info@addi-data.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

/* Card Specific information */
#define APCI3501_BOARD_VENDOR_ID                 0x15B8
#define APCI3501_ADDRESS_RANGE                   255

#define APCI3501_DIGITAL_IP                       0x50
#define APCI3501_DIGITAL_OP                       0x40
#define APCI3501_ANALOG_OUTPUT                    0x00

/* Analog Output related Defines */
#define APCI3501_AO_VOLT_MODE                     0
#define APCI3501_AO_PROG                          4
#define APCI3501_AO_TRIG_SCS                      8
#define UNIPOLAR                                  0
#define BIPOLAR                                   1
#define MODE0                                     0
#define MODE1                                     1
/* ANALOG OUTPUT RANGE */
static struct comedi_lrange range_apci3501_ao = { 2, {
					BIP_RANGE(10),
					UNI_RANGE(10)
					}
};

/* Watchdog Related Defines */

#define APCI3501_WATCHDOG                         0x20
#define APCI3501_TCW_SYNC_ENABLEDISABLE           0
#define APCI3501_TCW_RELOAD_VALUE                 4
#define APCI3501_TCW_TIMEBASE                     8
#define APCI3501_TCW_PROG                         12
#define APCI3501_TCW_TRIG_STATUS                  16
#define APCI3501_TCW_IRQ                          20
#define APCI3501_TCW_WARN_TIMEVAL                 24
#define APCI3501_TCW_WARN_TIMEBASE                28
#define ADDIDATA_TIMER                            0
#define ADDIDATA_WATCHDOG                         2

/* Hardware Layer  functions for Apci3501 */

/* AO */
int i_APCI3501_ConfigAnalogOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data);
int i_APCI3501_WriteAnalogOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data);

/*
* DI for di read INT i_APCI3501_ReadDigitalInput(struct
* comedi_device *dev,struct comedi_subdevice *s,struct comedi_insn
* *insn,unsigned int *data);
*/

int i_APCI3501_ReadDigitalInput(struct comedi_device *dev, struct comedi_subdevice *s,
				struct comedi_insn *insn, unsigned int *data);

/* DO */
int i_APCI3501_ConfigDigitalOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				   struct comedi_insn *insn, unsigned int *data);
int i_APCI3501_WriteDigitalOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data);
int i_APCI3501_ReadDigitalOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data);

/* TIMER
 * timer value is passed as u seconds
 */

int i_APCI3501_ConfigTimerCounterWatchdog(struct comedi_device *dev,
					  struct comedi_subdevice *s,
					  struct comedi_insn *insn, unsigned int *data);
int i_APCI3501_StartStopWriteTimerCounterWatchdog(struct comedi_device *dev,
						  struct comedi_subdevice *s,
						  struct comedi_insn *insn,
						  unsigned int *data);
int i_APCI3501_ReadTimerCounterWatchdog(struct comedi_device *dev,
					struct comedi_subdevice *s,
					struct comedi_insn *insn, unsigned int *data);
/* Interrupt */
void v_APCI3501_Interrupt(int irq, void *d);

/* Reset functions */
int i_APCI3501_Reset(struct comedi_device *dev);
