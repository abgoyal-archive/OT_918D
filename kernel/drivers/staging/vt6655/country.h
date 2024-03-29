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
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *
 * File: country.h
 *
 * Purpose: Country Code information
 *
 * Author: Lucas Lin
 *
 * Date: Dec 23, 2004
 *
 */

#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include "ttype.h"

/*---------------------  Export Definitions -------------------------*/
/************************************************************************
 * The definition here should be complied with the INF country order
 * Please check with VNWL.inf/VNWL64.inf/VNWL*.inf
 ************************************************************************/
typedef enum _COUNTRY_CODE {
    CCODE_FCC = 0,
    CCODE_TELEC,
    CCODE_ETSI,
    CCODE_RESV3,
    CCODE_RESV4,
    CCODE_RESV5,
    CCODE_RESV6,
    CCODE_RESV7,
    CCODE_RESV8,
    CCODE_RESV9,
    CCODE_RESVa,
    CCODE_RESVb,
    CCODE_RESVc,
    CCODE_RESVd,
    CCODE_RESVe,
    CCODE_ALLBAND,
    CCODE_ALBANIA,
    CCODE_ALGERIA,
    CCODE_ARGENTINA,
    CCODE_ARMENIA,
    CCODE_AUSTRALIA,
    CCODE_AUSTRIA,
    CCODE_AZERBAIJAN,
    CCODE_BAHRAIN,
    CCODE_BELARUS,
    CCODE_BELGIUM,
    CCODE_BELIZE,
    CCODE_BOLIVIA,
    CCODE_BRAZIL,
    CCODE_BRUNEI_DARUSSALAM,
    CCODE_BULGARIA,
    CCODE_CANADA,
    CCODE_CHILE,
    CCODE_CHINA,
    CCODE_COLOMBIA,
    CCODE_COSTA_RICA,
    CCODE_CROATIA,
    CCODE_CYPRUS,
    CCODE_CZECH,
    CCODE_DENMARK,
    CCODE_DOMINICAN_REPUBLIC,
    CCODE_ECUADOR,
    CCODE_EGYPT,
    CCODE_EL_SALVADOR,
    CCODE_ESTONIA,
    CCODE_FINLAND,
    CCODE_FRANCE,
    CCODE_GERMANY,
    CCODE_GREECE,
    CCODE_GEORGIA,
    CCODE_GUATEMALA,
    CCODE_HONDURAS,
    CCODE_HONG_KONG,
    CCODE_HUNGARY,
    CCODE_ICELAND,
    CCODE_INDIA,
    CCODE_INDONESIA,
    CCODE_IRAN,
    CCODE_IRELAND,
    CCODE_ITALY,
    CCODE_ISRAEL,
    CCODE_JAPAN,
    CCODE_JORDAN,
    CCODE_KAZAKHSTAN,
    CCODE_KUWAIT,
    CCODE_LATVIA,
    CCODE_LEBANON,
    CCODE_LEICHTENSTEIN,
    CCODE_LITHUANIA,
    CCODE_LUXEMBURG,
    CCODE_MACAU,
    CCODE_MACEDONIA,
    CCODE_MALTA,
    CCODE_MALAYSIA,
    CCODE_MEXICO,
    CCODE_MONACO,
    CCODE_MOROCCO,
    CCODE_NETHERLANDS,
    CCODE_NEW_ZEALAND,
    CCODE_NORTH_KOREA,
    CCODE_NORWAY,
    CCODE_OMAN,
    CCODE_PAKISTAN,
    CCODE_PANAMA,
    CCODE_PERU,
    CCODE_PHILIPPINES,
    CCODE_POLAND,
    CCODE_PORTUGAL,
    CCODE_PUERTO_RICO,
    CCODE_QATAR,
    CCODE_ROMANIA,
    CCODE_RUSSIA,
    CCODE_SAUDI_ARABIA,
    CCODE_SINGAPORE,
    CCODE_SLOVAKIA,
    CCODE_SLOVENIA,
    CCODE_SOUTH_AFRICA,
    CCODE_SOUTH_KOREA,
    CCODE_SPAIN,
    CCODE_SWEDEN,
    CCODE_SWITZERLAND,
    CCODE_SYRIA,
    CCODE_TAIWAN,
    CCODE_THAILAND,
    CCODE_TRINIDAD_TOBAGO,
    CCODE_TUNISIA,
    CCODE_TURKEY,
    CCODE_UK,
    CCODE_UKRAINE,
    CCODE_UNITED_ARAB_EMIRATES,
    CCODE_UNITED_STATES,
    CCODE_URUGUAY,
    CCODE_UZBEKISTAN,
    CCODE_VENEZUELA,
    CCODE_VIETNAM,
    CCODE_YEMEN,
    CCODE_ZIMBABWE,
    CCODE_JAPAN_W52_W53,
    CCODE_MAX
} COUNTRY_CODE;

typedef struct tagSCountryTable
{
    BYTE    byChannelCountryCode;             /* The country code         */
    CHAR    chCountryCode[2];
    BYTE    bChannelIdxList[CB_MAX_CHANNEL];  /* Available channels Index */
    BYTE    byPower[CB_MAX_CHANNEL];
}   SCountryTable, *PSCountryTable;

/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/
extern SCountryTable ChannelRuleTab[CCODE_MAX+1];

/*---------------------  Export Functions  --------------------------*/

#endif  /* __COUNTRY_H__ */
