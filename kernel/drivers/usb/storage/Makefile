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


#
# Makefile for the USB Mass Storage device drivers.
#
# 15 Aug 2000, Christoph Hellwig <hch@infradead.org>
# Rewritten to use lists instead of if-statements.
#

EXTRA_CFLAGS	:= -Idrivers/scsi

obj-$(CONFIG_USB_STORAGE)	+= usb-storage.o

usb-storage-obj-$(CONFIG_USB_STORAGE_DEBUG)	+= debug.o

usb-storage-objs :=	scsiglue.o protocol.o transport.o usb.o \
			initializers.o sierra_ms.o option_ms.o $(usb-storage-obj-y)

ifeq ($(CONFIG_USB_LIBUSUAL),)
	usb-storage-objs	+= usual-tables.o
else
	obj-$(CONFIG_USB)	+= usb-libusual.o
	usb-libusual-objs	:= libusual.o usual-tables.o
endif

obj-$(CONFIG_USB_STORAGE_ALAUDA)	+= ums-alauda.o
obj-$(CONFIG_USB_STORAGE_CYPRESS_ATACB) += ums-cypress.o
obj-$(CONFIG_USB_STORAGE_DATAFAB)	+= ums-datafab.o
obj-$(CONFIG_USB_STORAGE_FREECOM)	+= ums-freecom.o
obj-$(CONFIG_USB_STORAGE_ISD200)	+= ums-isd200.o
obj-$(CONFIG_USB_STORAGE_JUMPSHOT)	+= ums-jumpshot.o
obj-$(CONFIG_USB_STORAGE_KARMA)		+= ums-karma.o
obj-$(CONFIG_USB_STORAGE_ONETOUCH)	+= ums-onetouch.o
obj-$(CONFIG_USB_STORAGE_SDDR09)	+= ums-sddr09.o
obj-$(CONFIG_USB_STORAGE_SDDR55)	+= ums-sddr55.o
obj-$(CONFIG_USB_STORAGE_USBAT)		+= ums-usbat.o

ums-alauda-objs		:= alauda.o
ums-cypress-objs	:= cypress_atacb.o
ums-datafab-objs	:= datafab.o
ums-freecom-objs	:= freecom.o
ums-isd200-objs		:= isd200.o
ums-jumpshot-objs	:= jumpshot.o
ums-karma-objs		:= karma.o
ums-onetouch-objs	:= onetouch.o
ums-sddr09-objs		:= sddr09.o
ums-sddr55-objs		:= sddr55.o
ums-usbat-objs		:= shuttle_usbat.o
