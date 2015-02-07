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

/*********      Definitions for APCI-1500 card  *****/

/* Card Specific information */
#define APCI1500_BOARD_VENDOR_ID           0x10e8
#define APCI1500_ADDRESS_RANGE              4

/* DIGITAL INPUT-OUTPUT DEFINE */

#define  APCI1500_DIGITAL_OP                 	2
#define  APCI1500_DIGITAL_IP                    0
#define  APCI1500_AND               		    2
#define  APCI1500_OR                		    4
#define  APCI1500_OR_PRIORITY       		    6
#define  APCI1500_CLK_SELECT                    0
#define  COUNTER1                               0
#define  COUNTER2                               1
#define  COUNTER3                               2
#define  APCI1500_COUNTER                		0x20
#define  APCI1500_TIMER                  		0
#define  APCI1500_WATCHDOG          		    0
#define  APCI1500_SINGLE            		    0
#define  APCI1500_CONTINUOUS        		    0x80
#define  APCI1500_DISABLE                		0
#define  APCI1500_ENABLE                		1
#define  APCI1500_SOFTWARE_TRIGGER  		    0x4
#define  APCI1500_HARDWARE_TRIGGER  		    0x10
#define  APCI1500_SOFTWARE_GATE     		    0
#define  APCI1500_HARDWARE_GATE     		    0x8
#define  START                       		    0
#define  STOP                       		    1
#define  TRIGGER                       		    2

/*
 * Zillog I/O enumeration
 */
enum {
	APCI1500_Z8536_PORT_C,
	APCI1500_Z8536_PORT_B,
	APCI1500_Z8536_PORT_A,
	APCI1500_Z8536_CONTROL_REGISTER
};

/*
 * Z8536 CIO Internal Address
 */
enum {
	APCI1500_RW_MASTER_INTERRUPT_CONTROL,
	APCI1500_RW_MASTER_CONFIGURATION_CONTROL,
	APCI1500_RW_PORT_A_INTERRUPT_CONTROL,
	APCI1500_RW_PORT_B_INTERRUPT_CONTROL,
	APCI1500_RW_TIMER_COUNTER_INTERRUPT_VECTOR,
	APCI1500_RW_PORT_C_DATA_PCITCH_POLARITY,
	APCI1500_RW_PORT_C_DATA_DIRECTION,
	APCI1500_RW_PORT_C_SPECIAL_IO_CONTROL,

	APCI1500_RW_PORT_A_COMMAND_AND_STATUS,
	APCI1500_RW_PORT_B_COMMAND_AND_STATUS,
	APCI1500_RW_CPT_TMR1_CMD_STATUS,
	APCI1500_RW_CPT_TMR2_CMD_STATUS,
	APCI1500_RW_CPT_TMR3_CMD_STATUS,
	APCI1500_RW_PORT_A_DATA,
	APCI1500_RW_PORT_B_DATA,
	APCI1500_RW_PORT_C_DATA,

	APCI1500_R_CPT_TMR1_VALUE_HIGH,
	APCI1500_R_CPT_TMR1_VALUE_LOW,
	APCI1500_R_CPT_TMR2_VALUE_HIGH,
	APCI1500_R_CPT_TMR2_VALUE_LOW,
	APCI1500_R_CPT_TMR3_VALUE_HIGH,
	APCI1500_R_CPT_TMR3_VALUE_LOW,
	APCI1500_RW_CPT_TMR1_TIME_CST_HIGH,
	APCI1500_RW_CPT_TMR1_TIME_CST_LOW,
	APCI1500_RW_CPT_TMR2_TIME_CST_HIGH,
	APCI1500_RW_CPT_TMR2_TIME_CST_LOW,
	APCI1500_RW_CPT_TMR3_TIME_CST_HIGH,
	APCI1500_RW_CPT_TMR3_TIME_CST_LOW,
	APCI1500_RW_CPT_TMR1_MODE_SPECIFICATION,
	APCI1500_RW_CPT_TMR2_MODE_SPECIFICATION,
	APCI1500_RW_CPT_TMR3_MODE_SPECIFICATION,
	APCI1500_R_CURRENT_VECTOR,

	APCI1500_RW_PORT_A_SPECIFICATION,
	APCI1500_RW_PORT_A_HANDSHAKE_SPECIFICATION,
	APCI1500_RW_PORT_A_DATA_PCITCH_POLARITY,
	APCI1500_RW_PORT_A_DATA_DIRECTION,
	APCI1500_RW_PORT_A_SPECIAL_IO_CONTROL,
	APCI1500_RW_PORT_A_PATTERN_POLARITY,
	APCI1500_RW_PORT_A_PATTERN_TRANSITION,
	APCI1500_RW_PORT_A_PATTERN_MASK,

	APCI1500_RW_PORT_B_SPECIFICATION,
	APCI1500_RW_PORT_B_HANDSHAKE_SPECIFICATION,
	APCI1500_RW_PORT_B_DATA_PCITCH_POLARITY,
	APCI1500_RW_PORT_B_DATA_DIRECTION,
	APCI1500_RW_PORT_B_SPECIAL_IO_CONTROL,
	APCI1500_RW_PORT_B_PATTERN_POLARITY,
	APCI1500_RW_PORT_B_PATTERN_TRANSITION,
	APCI1500_RW_PORT_B_PATTERN_MASK
};

 /*----------DIGITAL INPUT----------------*/
static int i_APCI1500_Initialisation(struct comedi_device *dev, struct comedi_subdevice *s,
				     struct comedi_insn *insn, unsigned int *data);
static int i_APCI1500_ConfigDigitalInputEvent(struct comedi_device *dev,
					      struct comedi_subdevice *s,
					      struct comedi_insn *insn,
					      unsigned int *data);

static int i_APCI1500_StartStopInputEvent(struct comedi_device *dev,
					  struct comedi_subdevice *s,
					  struct comedi_insn *insn, unsigned int *data);
static int i_APCI1500_ReadMoreDigitalInput(struct comedi_device *dev,
					   struct comedi_subdevice *s,
					   struct comedi_insn *insn, unsigned int *data);

/*----------	DIGITAL OUTPUT------------*/
static int i_APCI1500_ConfigDigitalOutputErrorInterrupt(struct comedi_device *dev,
							struct comedi_subdevice *s,
							struct comedi_insn *insn,
							unsigned int *data);
static int i_APCI1500_WriteDigitalOutput(struct comedi_device *dev,
					 struct comedi_subdevice *s,
					 struct comedi_insn *insn, unsigned int *data);

/*----------TIMER----------------*/
static int i_APCI1500_ConfigCounterTimerWatchdog(struct comedi_device *dev,
						 struct comedi_subdevice *s,
						 struct comedi_insn *insn,
						 unsigned int *data);
static int i_APCI1500_StartStopTriggerTimerCounterWatchdog(struct comedi_device *dev,
							   struct comedi_subdevice *s,
							   struct comedi_insn *insn,
							   unsigned int *data);
static int i_APCI1500_ReadCounterTimerWatchdog(struct comedi_device *dev,
					       struct comedi_subdevice *s,
					       struct comedi_insn *insn,
					       unsigned int *data);
static int i_APCI1500_ReadInterruptMask(struct comedi_device *dev,
					struct comedi_subdevice *s,
					struct comedi_insn *insn, unsigned int *data);

/*----------INTERRUPT HANDLER------*/
static void v_APCI1500_Interrupt(int irq, void *d);
static int i_APCI1500_ConfigureInterrupt(struct comedi_device *dev,
					 struct comedi_subdevice *s,
					 struct comedi_insn *insn, unsigned int *data);
/*----------RESET---------------*/
static int i_APCI1500_Reset(struct comedi_device *dev);
