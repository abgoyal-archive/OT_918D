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

/******************************************************************************
 *
 *     (c) Copyright  2008, RealTEK Technologies Inc. All Rights Reserved.
 *
 * Module:	Efuse.h	( Header File)
 *
 * Note:
 *
 * Function:
 *
 * Export:
 *
 * Abbrev:
 *
 * History:
 * Data			Who		Remark
 *
 * 09/23/2008	MHC		Porting Efuse R/W API from WMAC.
 * 11/10/2008	MHC		Porting Efuse.h from 8712 SDIO.
 *						1. We muse redefine the header file to fit our coding
 *						   style.
 *						2. THe API we export to other module, we must redefine
 *						   for 8192S series.
 *
 *
******************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef __INC_EFUSE_H
#define __INC_EFUSE_H

// Roger porting for 8192SU
#define		EFUSE_FOR_92SU		1

/*--------------------------Define Parameters-------------------------------*/
#define		EFUSE_MAC_LEN					0x200

#define		EFUSE_INIT_MAP				0
#define		EFUSE_MODIFY_MAP				1

#define		EFUSE_CLK_CTRL			EFUSE_CTRL
#define 	EFUSE_BIT(x)  (1 << (x))

// From 8712!!!!!!!!
#define		PG_STATE_HEADER 	0x01
#define		PG_STATE_WORD_0		0x02
#define		PG_STATE_WORD_1		0x04
#define		PG_STATE_WORD_2		0x08
#define		PG_STATE_WORD_3		0x10
#define		PG_STATE_DATA		0x20

#define		PG_SWBYTE_H			0x01
#define		PG_SWBYTE_L			0x02

/*--------------------------Define Parameters-------------------------------*/


/*------------------------------Define structure----------------------------*/

/*------------------------------Define structure----------------------------*/


/*------------------------Export global variable----------------------------*/
/*------------------------Export global variable----------------------------*/

/*------------------------Export Marco Definition---------------------------*/

/*------------------------Export Marco Definition---------------------------*/


/*--------------------------Exported Function prototype---------------------*/
extern	void
EFUSE_Initialize(struct net_device* dev);
extern	u8
EFUSE_Read1Byte(struct net_device* dev,u16 Address);
extern	void
EFUSE_Write1Byte(struct net_device* dev,u16 Address,u8 Value);

#ifdef EFUSE_FOR_92SU
extern 	void
ReadEFuse(struct net_device* dev,u16 _offset,u16 _size_byte,u8* pbuf);
extern	void
ReadEFuseByte(struct net_device* dev,u16  _offset,u8  *pbuf);
#endif	// #if (EFUSE_FOR_92SU == 1)

extern	void
EFUSE_ShadowRead(struct net_device* dev,unsigned char Type,unsigned short Offset,u32 *Value);
extern	void
EFUSE_ShadowWrite(struct net_device* dev,unsigned char Type,unsigned short Offset,u32 Value);
extern	void
EFUSE_ShadowUpdate(struct net_device* dev);
extern	void
EFUSE_ShadowMapUpdate(struct net_device* dev);

extern	bool
EFUSE_ProgramMap(struct net_device* dev,char* pFileName, u8 TableType);		// 0=Shadow 1=Real Efuse
/*--------------------------Exported Function prototype---------------------*/

/* End of Efuse.h */

#endif //__INC_EFUSE_H
