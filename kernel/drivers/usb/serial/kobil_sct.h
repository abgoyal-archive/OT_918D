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

#define SUSBCRequest_SetBaudRateParityAndStopBits       1
#define SUSBCR_SBR_MASK				0xFF00
#define SUSBCR_SBR_1200				0x0100
#define SUSBCR_SBR_9600				0x0200
#define SUSBCR_SBR_19200			0x0400
#define SUSBCR_SBR_28800			0x0800
#define SUSBCR_SBR_38400			0x1000
#define SUSBCR_SBR_57600			0x2000
#define SUSBCR_SBR_115200			0x4000

#define SUSBCR_SPASB_MASK			0x0070
#define SUSBCR_SPASB_NoParity			0x0010
#define SUSBCR_SPASB_OddParity			0x0020
#define SUSBCR_SPASB_EvenParity			0x0040

#define SUSBCR_SPASB_STPMASK			0x0003
#define SUSBCR_SPASB_1StopBit			0x0001
#define SUSBCR_SPASB_2StopBits			0x0002

#define SUSBCRequest_SetStatusLinesOrQueues	2
#define SUSBCR_SSL_SETRTS			0x0001
#define SUSBCR_SSL_CLRRTS			0x0002
#define SUSBCR_SSL_SETDTR			0x0004
#define SUSBCR_SSL_CLRDTR			0x0010

/* Kill the pending/current writes to the comm port. */
#define SUSBCR_SSL_PURGE_TXABORT		0x0100
/* Kill the pending/current reads to the comm port. */
#define SUSBCR_SSL_PURGE_RXABORT		0x0200
/* Kill the transmit queue if there. */
#define SUSBCR_SSL_PURGE_TXCLEAR		0x0400
/* Kill the typeahead buffer if there. */
#define SUSBCR_SSL_PURGE_RXCLEAR		0x0800

#define SUSBCRequest_GetStatusLineState		4
/* Any Character received */
#define SUSBCR_GSL_RXCHAR			0x0001
/* Transmitt Queue Empty */
#define SUSBCR_GSL_TXEMPTY			0x0004
/* CTS changed state */
#define SUSBCR_GSL_CTS				0x0008
/* DSR changed state */
#define SUSBCR_GSL_DSR				0x0010
/* RLSD changed state */
#define SUSBCR_GSL_RLSD				0x0020
/* BREAK received */
#define SUSBCR_GSL_BREAK			0x0040
/* Line status error occurred */
#define SUSBCR_GSL_ERR				0x0080
/* Ring signal detected */
#define SUSBCR_GSL_RING				0x0100

#define SUSBCRequest_Misc			8
/* use a predefined reset sequence */
#define SUSBCR_MSC_ResetReader			0x0001
/* use a predefined sequence to reset the internal queues */
#define SUSBCR_MSC_ResetAllQueues		0x0002

#define SUSBCRequest_GetMisc			0x10

/*
 * get the firmware version from device, coded like this 0xHHLLBBPP with
 * HH = Firmware Version High Byte
 * LL = Firmware Version Low Byte
 * BB = Build Number
 * PP = Further Attributes
 */
#define SUSBCR_MSC_GetFWVersion			0x0001

/*
 * get the hardware version from device coded like this 0xHHLLPPRR with
 * HH = Software Version High Byte
 * LL = Software Version Low Byte
 * PP = Further Attributes
 * RR = Reserved for the hardware ID
 */
#define SUSBCR_MSC_GetHWVersion			0x0002
