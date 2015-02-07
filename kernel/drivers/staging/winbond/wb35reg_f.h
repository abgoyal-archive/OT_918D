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

#ifndef __WINBOND_WB35REG_F_H
#define __WINBOND_WB35REG_F_H

#include "wbhal_s.h"

/*
 * ====================================
 * Interface function declare
 * ====================================
 */
unsigned char Wb35Reg_initial(struct hw_data *hw_data);
void Uxx_power_on_procedure(struct hw_data *hw_data);
void Uxx_power_off_procedure(struct hw_data *hw_data);
void Uxx_ReadEthernetAddress(struct hw_data *hw_data);
void Dxx_initial(struct hw_data *hw_data);
void Mxx_initial(struct hw_data *hw_data);
void RFSynthesizer_initial(struct hw_data *hw_data);
void RFSynthesizer_SwitchingChannel(struct hw_data *hw_data, struct chan_info channel);
void BBProcessor_initial(struct hw_data *hw_data);
void BBProcessor_RateChanging(struct hw_data *hw_data, u8 rate);
u8 RFSynthesizer_SetPowerIndex(struct hw_data *hw_data, u8 power_index);
u8 RFSynthesizer_SetMaxim2828_24Power(struct hw_data *, u8 index);
u8 RFSynthesizer_SetMaxim2828_50Power(struct hw_data *, u8 index);
u8 RFSynthesizer_SetMaxim2827_24Power(struct hw_data *, u8 index);
u8 RFSynthesizer_SetMaxim2827_50Power(struct hw_data *, u8 index);
u8 RFSynthesizer_SetMaxim2825Power(struct hw_data *, u8 index);
u8 RFSynthesizer_SetAiroha2230Power(struct hw_data *, u8 index);
u8 RFSynthesizer_SetAiroha7230Power(struct hw_data *, u8 index);
u8 RFSynthesizer_SetWinbond242Power(struct hw_data *, u8 index);
void GetTxVgaFromEEPROM(struct hw_data *hw_data);
void EEPROMTxVgaAdjust(struct hw_data *hw_data);

#define RFWriteControlData(_A, _V) Wb35Reg_Write(_A, 0x0864, _V)

void Wb35Reg_destroy(struct hw_data *hw_data);

unsigned char Wb35Reg_Read(struct hw_data *hw_data, u16 register_no, u32 *register_value);
unsigned char Wb35Reg_ReadSync(struct hw_data *hw_data, u16 register_no, u32 *register_value);
unsigned char Wb35Reg_Write(struct hw_data *hw_data, u16 register_no, u32 register_value);
unsigned char Wb35Reg_WriteSync(struct hw_data *hw_data, u16 register_no, u32 register_value);
unsigned char Wb35Reg_WriteWithCallbackValue(struct hw_data *hw_data,
							 u16 register_no,
							 u32 register_value,
							 s8 *value,
							 s8 len);
unsigned char Wb35Reg_BurstWrite(struct hw_data *hw_data,
					u16 register_no,
					u32 *register_data,
					u8 number_of_data,
					u8 flag);

void Wb35Reg_EP0VM(struct hw_data *hw_data);
void Wb35Reg_EP0VM_start(struct hw_data *hw_data);
void Wb35Reg_EP0VM_complete(struct urb *urb);

u32 BitReverse(u32 data, u32 data_length);

void CardGetMulticastBit(u8 address[MAC_ADDR_LENGTH], u8 *byte, u8 *value);
u32 CardComputeCrc(u8 *buffer, u32 length);

void Wb35Reg_phy_calibration(struct hw_data *hw_data);
void Wb35Reg_Update(struct hw_data *hw_data, u16 register_no, u32 register_value);
unsigned char adjust_TXVGA_for_iq_mag(struct hw_data *hw_data);

#endif
