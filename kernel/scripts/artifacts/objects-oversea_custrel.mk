# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

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
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


# please see binary/Makefile for usage


obj_to_install := \
 ../mediatek/source/kernel/drivers/combo/common/core:mtkstp.o \
 ../mediatek/source/kernel/drivers/combo/common/core:mtkwmt.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:stp_chrdev_wmt.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:stp_drv.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:stp_psm.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:stp_sdio.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:stp_uart.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:wmt_lib.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:wmt_lib_conf.o \
 ../mediatek/source/kernel/drivers/combo/common/linux/pri:wmt_lib_plat.o \
 ../mediatek/platform/mt6573/kernel/drivers/mflexvideo:mt6573_mflexvideo_kernel_driver.o \
 ../mediatek/platform/mt6573/kernel/drivers/mflexvideo/codec/common/hal/linux/kernel:hal_hw.o \
 ../mediatek/platform/mt6573/kernel/drivers/mflexvideo/codec/common/val/linux/kernel:val_hw.o \
 ../mediatek/platform/mt6573/kernel/drivers/mflexvideo/codec/common/val/linux/kernel:val.o \
 ../mediatek/platform/mt6573/kernel/drivers/mflexvideo/codec/common/val/linux/kernel:mfv_load_packet.o \
 ../mediatek/platform/mt6573/kernel/drivers/mflexvideo/codec/common/api/src:mfv_drv_base.o

ifeq ($(MTK_WAPI_SUPPORT),yes)
obj_to_install +=  ../mediatek/source/kernel/drivers/net/mt592x/wlan:gl_sec.o \

endif

file_to_touch := \
  arch/arm/mach-mt3351/Kconfig \
  arch/arm/mach-mt3351/Makefile \

