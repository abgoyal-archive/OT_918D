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

#ifndef __WINBOND_SCAN_S_H
#define __WINBOND_SCAN_S_H

#include <linux/types.h>
#include "localpara.h"

/*
 * SCAN task global CONSTANTS, STRUCTURES, variables
 */

/* define the msg type of SCAN module */
#define SCANMSG_SCAN_REQ		0x01
#define SCANMSG_BEACON			0x02
#define SCANMSG_PROBE_RESPONSE		0x03
#define SCANMSG_TIMEOUT			0x04
#define SCANMSG_TXPROBE_FAIL		0x05
#define SCANMSG_ENABLE_BGSCAN		0x06
#define SCANMSG_STOP_SCAN		0x07

/*
 * BSS Type =>conform to
 * IBSS             : ToDS/FromDS = 00
 * Infrastructure   : ToDS/FromDS = 01
 */
#define IBSS_NET			0
#define ESS_NET				1
#define ANYBSS_NET			2

/* Scan Type */
#define ACTIVE_SCAN			0
#define PASSIVE_SCAN			1

/* Global data structures, Initial Scan & Background Scan */
typedef struct _SCAN_REQ_PARA {	/* mandatory parameters for SCAN request */

	u32			ScanType;	/* passive/active scan */

	u8			reserved_1[2];

	struct SSID_Element	sSSID; /* 34B. scan only for this SSID */
	u8			reserved_2[2];

} SCAN_REQ_PARA, *psSCAN_REQ_PARA;

typedef struct _SCAN_PARAMETERS {
	u16		wState;
	u16		iCurrentChannelIndex;

	SCAN_REQ_PARA	sScanReq;

	u8		BSSID[MAC_ADDR_LENGTH + 2]; /* scan only for this BSSID */

	u32		BssType;	/* scan only for this BSS type */

	u16		ProbeDelay;
	u16		MinChannelTime;

	u16		MaxChannelTime;
	u16		reserved_1;

	s32		iBgScanPeriod;	/* XP: 5 sec */

	u8		boBgScan;	/* Wb: enable BG scan, For XP, this value must be FALSE */
	u8		boFastScan;	/* Wb: reserved */
	u8		boCCAbusy;	/* Wb: HWMAC CCA busy status */
	u8		reserved_2;

	struct timer_list timer;

	u32		ScanTimeStamp;	/* Increase 1 per background scan(1 minute) */
	u32		BssTimeStamp;	/* Increase 1 per connect status check */
	u32		RxNumPerAntenna[2];

	u8		AntennaToggle;
	u8		boInTimerHandler;
	u8		boTimerActive;	/* Wb: reserved */
	u8		boSave;

	u32		BScanEnable; /* Background scan enable. Default is On */
} SCAN_PARAMETERS, *psSCAN_PARAMETERS;

/* Encapsulate 'adapter' data structure */
#define psSCAN		(&(adapter->sScanPara))
#define psSCANREQ	(&(adapter->sScanPara.sScanReq))

/*
 * ===========================================================
 *	scan.h
 *		Define the related definitions of scan module
 *
 * ===========================================================
 */

/* Define the state of scan module */
#define SCAN_INACTIVE			0
#define WAIT_PROBE_DELAY		1
#define WAIT_RESPONSE_MIN		2
#define WAIT_RESPONSE_MAX_ACTIVE	3
#define WAIT_BEACON_MAX_PASSIVE		4
#define SCAN_COMPLETE			5
#define BG_SCAN				6
#define BG_SCANNING			7


/*
 * The value will load from EEPROM
 * If 0xff is set in EEPOM, the driver will use SCAN_MAX_CHNL_TIME instead.
 * The definition is in WbHal.h
 */
#endif
