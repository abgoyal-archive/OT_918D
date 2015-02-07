# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


obj-$(CONFIG_RT2860)	+= rt2860sta.o

# TODO: all of these should be removed
EXTRA_CFLAGS += -DLINUX -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT
EXTRA_CFLAGS += -DRTMP_MAC_PCI -DRTMP_PCI_SUPPORT -DRT2860
EXTRA_CFLAGS += -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DRT30xx -DRT3090
EXTRA_CFLAGS += -DDBG

rt2860sta-objs :=	\
	common/crypt_md5.o	\
	common/crypt_sha2.o	\
	common/crypt_hmac.o	\
	common/mlme.o		\
	common/cmm_wep.o	\
	common/action.o		\
	common/cmm_data.o	\
	common/rtmp_init.o	\
	common/cmm_tkip.o	\
	common/cmm_aes.o	\
	common/cmm_sync.o	\
	common/eeprom.o		\
	common/cmm_sanity.o	\
	common/cmm_info.o	\
	common/cmm_cfg.o	\
	common/cmm_wpa.o	\
	common/dfs.o		\
	common/spectrum.o	\
	common/rtmp_timer.o	\
	common/rt_channel.o	\
	common/cmm_asic.o	\
	sta/assoc.o		\
	sta/auth.o		\
	sta/auth_rsp.o		\
	sta/sync.o		\
	sta/sanity.o		\
	sta/rtmp_data.o		\
	sta/connect.o		\
	sta/wpa.o		\
	rt_linux.o		\
	rt_main_dev.o		\
	sta_ioctl.o		\
	common/ba_action.o	\
	pci_main_dev.o		\
	rt_pci_rbus.o		\
	common/cmm_mac_pci.o	\
	common/cmm_data_pci.o	\
	common/ee_prom.o	\
	common/rtmp_mcu.o	\
	common/ee_efuse.o	\
	chips/rt30xx.o		\
	common/rt_rf.o		\
	chips/rt3090.o
