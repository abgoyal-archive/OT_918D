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


# arch/arm/mach-s3c64xx/Makefile
#
# Copyright 2008 Openmoko, Inc.
# Copyright 2008 Simtec Electronics
#
# Licensed under GPLv2

obj-y				:=
obj-m				:=
obj-n				:=
obj-				:=

# Core files
obj-y				+= cpu.o
obj-y				+= clock.o
obj-y				+= gpiolib.o

# Core support for S3C6400 system

obj-$(CONFIG_CPU_S3C6400)	+= s3c6400.o
obj-$(CONFIG_CPU_S3C6410)	+= s3c6410.o

obj-y				+= irq.o
obj-y				+= irq-eint.o

# CPU frequency scaling

obj-$(CONFIG_CPU_FREQ_S3C64XX)  += cpufreq.o

# DMA support

obj-$(CONFIG_S3C64XX_DMA)	+= dma.o

# Device setup

obj-$(CONFIG_S3C64XX_SETUP_I2C0) += setup-i2c0.o
obj-$(CONFIG_S3C64XX_SETUP_I2C1) += setup-i2c1.o
obj-$(CONFIG_S3C64XX_SETUP_SDHCI) += setup-sdhci.o
obj-$(CONFIG_S3C64XX_SETUP_FB_24BPP) += setup-fb-24bpp.o
obj-$(CONFIG_S3C64XX_SETUP_SDHCI_GPIO) += setup-sdhci-gpio.o

# PM

obj-$(CONFIG_PM)		+= pm.o
obj-$(CONFIG_PM)		+= sleep.o
obj-$(CONFIG_PM)		+= irq-pm.o

# Machine support

obj-$(CONFIG_MACH_ANW6410)	+= mach-anw6410.o
obj-$(CONFIG_MACH_SMDK6400)	+= mach-smdk6400.o
obj-$(CONFIG_MACH_SMDK6410)	+= mach-smdk6410.o
obj-$(CONFIG_MACH_NCP)		+= mach-ncp.o
obj-$(CONFIG_MACH_HMT)		+= mach-hmt.o
obj-$(CONFIG_MACH_SMARTQ)	+= mach-smartq.o
obj-$(CONFIG_MACH_SMARTQ5)	+= mach-smartq5.o
obj-$(CONFIG_MACH_SMARTQ7)	+= mach-smartq7.o

# device support

obj-y				+= dev-uart.o
obj-y				+= dev-audio.o
obj-$(CONFIG_S3C64XX_DEV_SPI)	+= dev-spi.o
obj-$(CONFIG_S3C64XX_DEV_TS)	+= dev-ts.o
obj-$(CONFIG_S3C64XX_DEV_ONENAND1)	+= dev-onenand1.o
