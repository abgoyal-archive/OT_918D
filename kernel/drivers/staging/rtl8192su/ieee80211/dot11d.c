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

//-----------------------------------------------------------------------------
//	File:
//		Dot11d.c
//
//	Description:
//		Implement 802.11d.
//
//-----------------------------------------------------------------------------

#include "dot11d.h"

void
Dot11d_Init(struct ieee80211_device *ieee)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(ieee);

	pDot11dInfo->bEnabled = 0;

	pDot11dInfo->State = DOT11D_STATE_NONE;
	pDot11dInfo->CountryIeLen = 0;
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER+1);
	RESET_CIE_WATCHDOG(ieee);

	printk("Dot11d_Init()\n");
}

//
//	Description:
//		Reset to the state as we are just entering a regulatory domain.
//
void
Dot11d_Reset(struct ieee80211_device *ieee)
{
	u32 i;
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(ieee);

	// Clear old channel map
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER+1);
	// Set new channel map
	for (i=1; i<=11; i++) {
		(pDot11dInfo->channel_map)[i] = 1;
	}
	for (i=12; i<=14; i++) {
		(pDot11dInfo->channel_map)[i] = 2;
	}

	pDot11dInfo->State = DOT11D_STATE_NONE;
	pDot11dInfo->CountryIeLen = 0;
	RESET_CIE_WATCHDOG(ieee);

	//printk("Dot11d_Reset()\n");
}

//
//	Description:
//		Update country IE from Beacon or Probe Resopnse
//		and configure PHY for operation in the regulatory domain.
//
//	TODO:
//		Configure Tx power.
//
//	Assumption:
//		1. IS_DOT11D_ENABLE() is TRUE.
//		2. Input IE is an valid one.
//
void
Dot11d_UpdateCountryIe(
	struct ieee80211_device *dev,
	u8 *		pTaddr,
	u16	CoutryIeLen,
	u8 * pCoutryIe
	)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 i, j, NumTriples, MaxChnlNum;
	PCHNL_TXPOWER_TRIPLE pTriple;

	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER+1);
	MaxChnlNum = 0;
	NumTriples = (CoutryIeLen - 3) / 3; // skip 3-byte country string.
	pTriple = (PCHNL_TXPOWER_TRIPLE)(pCoutryIe + 3);
	for(i = 0; i < NumTriples; i++)
	{
		if(MaxChnlNum >= pTriple->FirstChnl)
		{ // It is not in a monotonically increasing order, so stop processing.
			printk("Dot11d_UpdateCountryIe(): Invalid country IE, skip it........1\n");
			return;
		}
		if(MAX_CHANNEL_NUMBER < (pTriple->FirstChnl + pTriple->NumChnls))
		{ // It is not a valid set of channel id, so stop processing.
			printk("Dot11d_UpdateCountryIe(): Invalid country IE, skip it........2\n");
			return;
		}

		for(j = 0 ; j < pTriple->NumChnls; j++)
		{
			pDot11dInfo->channel_map[pTriple->FirstChnl + j] = 1;
			pDot11dInfo->MaxTxPwrDbmList[pTriple->FirstChnl + j] = pTriple->MaxTxPowerInDbm;
			MaxChnlNum = pTriple->FirstChnl + j;
		}

		pTriple = (PCHNL_TXPOWER_TRIPLE)((u8*)pTriple + 3);
	}
#if 1
	//printk("Dot11d_UpdateCountryIe(): Channel List:\n");
	printk("Channel List:");
	for(i=1; i<= MAX_CHANNEL_NUMBER; i++)
		if(pDot11dInfo->channel_map[i] > 0)
			printk(" %d", i);
	printk("\n");
#endif

	UPDATE_CIE_SRC(dev, pTaddr);

	pDot11dInfo->CountryIeLen = CoutryIeLen;
	memcpy(pDot11dInfo->CountryIeBuf, pCoutryIe,CoutryIeLen);
	pDot11dInfo->State = DOT11D_STATE_LEARNED;
}


u8
DOT11D_GetMaxTxPwrInDbm(
	struct ieee80211_device *dev,
	u8 Channel
	)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 MaxTxPwrInDbm = 255;

	if(MAX_CHANNEL_NUMBER < Channel)
	{
		printk("DOT11D_GetMaxTxPwrInDbm(): Invalid Channel\n");
		return MaxTxPwrInDbm;
	}
	if(pDot11dInfo->channel_map[Channel])
	{
		MaxTxPwrInDbm = pDot11dInfo->MaxTxPwrDbmList[Channel];
	}

	return MaxTxPwrInDbm;
}


void
DOT11D_ScanComplete(
	struct ieee80211_device * dev
	)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);

	switch(pDot11dInfo->State)
	{
	case DOT11D_STATE_LEARNED:
		pDot11dInfo->State = DOT11D_STATE_DONE;
		break;

	case DOT11D_STATE_DONE:
		if( GET_CIE_WATCHDOG(dev) == 0 )
		{ // Reset country IE if previous one is gone.
			Dot11d_Reset(dev);
		}
		break;
	case DOT11D_STATE_NONE:
		break;
	}
}

int IsLegalChannel(
	struct ieee80211_device * dev,
	u8 channel
)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);

	if(MAX_CHANNEL_NUMBER < channel)
	{
		printk("IsLegalChannel(): Invalid Channel\n");
		return 0;
	}
	if(pDot11dInfo->channel_map[channel] > 0)
		return 1;
	return 0;
}

int ToLegalChannel(
	struct ieee80211_device * dev,
	u8 channel
)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 default_chn = 0;
	u32 i = 0;

	for (i=1; i<= MAX_CHANNEL_NUMBER; i++)
	{
		if(pDot11dInfo->channel_map[i] > 0)
		{
			default_chn = i;
			break;
		}
	}

	if(MAX_CHANNEL_NUMBER < channel)
	{
		printk("IsLegalChannel(): Invalid Channel\n");
		return default_chn;
	}

	if(pDot11dInfo->channel_map[channel] > 0)
		return channel;

	return default_chn;
}
