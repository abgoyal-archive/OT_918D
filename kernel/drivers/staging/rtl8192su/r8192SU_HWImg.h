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

#ifndef __INC_HAL8192SU_FW_IMG_H
#define __INC_HAL8192SU_FW_IMG_H

#include <linux/types.h>

/*Created on  2009/ 3/ 6,  5:29*/

#define MainArrayLength 1
extern u8 Rtl8192SUFwMainArray[MainArrayLength];
#define DataArrayLength 1
extern u8 Rtl8192SUFwDataArray[DataArrayLength];
#define PHY_REG_2T2RArrayLength 372
extern u32 Rtl8192SUPHY_REG_2T2RArray[PHY_REG_2T2RArrayLength];
#define PHY_REG_1T2RArrayLength 1
extern u32 Rtl8192SUPHY_REG_1T2RArray[PHY_REG_1T2RArrayLength];
#define PHY_ChangeTo_1T1RArrayLength 48
extern u32 Rtl8192SUPHY_ChangeTo_1T1RArray[PHY_ChangeTo_1T1RArrayLength];
#define PHY_ChangeTo_1T2RArrayLength 45
extern u32 Rtl8192SUPHY_ChangeTo_1T2RArray[PHY_ChangeTo_1T2RArrayLength];
#define PHY_ChangeTo_2T2RArrayLength 45
extern u32 Rtl8192SUPHY_ChangeTo_2T2RArray[PHY_ChangeTo_2T2RArrayLength];
#define PHY_REG_Array_PGLength 21
extern u32 Rtl8192SUPHY_REG_Array_PG[PHY_REG_Array_PGLength];
#define RadioA_1T_ArrayLength 202
extern u32 Rtl8192SURadioA_1T_Array[RadioA_1T_ArrayLength];
#define RadioB_ArrayLength 22
extern u32 Rtl8192SURadioB_Array[RadioB_ArrayLength];
#define RadioA_to1T_ArrayLength 2
extern u32 Rtl8192SURadioA_to1T_Array[RadioA_to1T_ArrayLength];
#define RadioA_to2T_ArrayLength 2
extern u32 Rtl8192SURadioA_to2T_Array[RadioA_to2T_ArrayLength];
#define RadioB_GM_ArrayLength 16
extern u32 Rtl8192SURadioB_GM_Array[RadioB_GM_ArrayLength];
#define MAC_2T_ArrayLength 106
extern u32 Rtl8192SUMAC_2T_Array[MAC_2T_ArrayLength];
#define MACPHY_Array_PGLength 1
extern u32 Rtl8192SUMACPHY_Array_PG[MACPHY_Array_PGLength];
#define AGCTAB_ArrayLength 320
extern u32 Rtl8192SUAGCTAB_Array[AGCTAB_ArrayLength];

#endif //__INC_HAL8192SU_FW_IMG_H

