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
# Makefile for the linux kernel.
#

# Common support (must be linked before board specific support)
obj-y				+= clock.o devices.o generic.o irq.o \
				   time.o reset.o
obj-$(CONFIG_PM)		+= pm.o sleep.o standby.o

ifeq ($(CONFIG_CPU_FREQ),y)
obj-$(CONFIG_PXA25x)		+= cpufreq-pxa2xx.o
obj-$(CONFIG_PXA27x)		+= cpufreq-pxa2xx.o
obj-$(CONFIG_PXA3xx)		+= cpufreq-pxa3xx.o
endif

# Generic drivers that other drivers may depend upon

# SoC-specific code
obj-$(CONFIG_PXA25x)		+= mfp-pxa2xx.o pxa2xx.o pxa25x.o
obj-$(CONFIG_PXA27x)		+= mfp-pxa2xx.o pxa2xx.o pxa27x.o
obj-$(CONFIG_PXA3xx)		+= mfp-pxa3xx.o pxa3xx.o smemc.o
obj-$(CONFIG_CPU_PXA300)	+= pxa300.o
obj-$(CONFIG_CPU_PXA320)	+= pxa320.o
obj-$(CONFIG_CPU_PXA930)	+= pxa930.o

# NOTE: keep the order of boards in accordance to their order in Kconfig

# Intel/Marvell Dev Platforms
obj-$(CONFIG_ARCH_LUBBOCK)	+= lubbock.o
obj-$(CONFIG_MACH_MAINSTONE)	+= mainstone.o
obj-$(CONFIG_MACH_ZYLONITE300)	+= zylonite.o zylonite_pxa300.o
obj-$(CONFIG_MACH_ZYLONITE320)	+= zylonite.o zylonite_pxa320.o
obj-$(CONFIG_MACH_LITTLETON)	+= littleton.o
obj-$(CONFIG_MACH_TAVOREVB)	+= tavorevb.o
obj-$(CONFIG_MACH_SAAR)		+= saar.o

# 3rd Party Dev Platforms
obj-$(CONFIG_ARCH_PXA_IDP)	+= idp.o
obj-$(CONFIG_ARCH_VIPER)	+= viper.o
obj-$(CONFIG_MACH_ARCOM_ZEUS)	+= zeus.o
obj-$(CONFIG_MACH_BALLOON3)	+= balloon3.o
obj-$(CONFIG_MACH_CSB726)	+= csb726.o
obj-$(CONFIG_CSB726_CSB701)	+= csb701.o
obj-$(CONFIG_MACH_ARMCORE)      += cm-x2xx.o cm-x255.o cm-x270.o
ifeq ($(CONFIG_PCI),y)
obj-$(CONFIG_MACH_ARMCORE)	+= cm-x2xx-pci.o
endif
obj-$(CONFIG_MACH_EM_X270)	+= em-x270.o
obj-$(CONFIG_MACH_CM_X300)      += cm-x300.o
obj-$(CONFIG_MACH_CAPC7117)     += capc7117.o mxm8x10.o
obj-$(CONFIG_ARCH_GUMSTIX)	+= gumstix.o
obj-$(CONFIG_GUMSTIX_AM200EPD)	+= am200epd.o
obj-$(CONFIG_GUMSTIX_AM300EPD)	+= am300epd.o
obj-$(CONFIG_MACH_INTELMOTE2)   += imote2.o
obj-$(CONFIG_MACH_STARGATE2)	+= stargate2.o
obj-$(CONFIG_MACH_XCEP)         += xcep.o
obj-$(CONFIG_MACH_TRIZEPS4)	+= trizeps4.o
obj-$(CONFIG_MACH_LOGICPD_PXA270)	+= lpd270.o
obj-$(CONFIG_MACH_PCM027)		+= pcm027.o
obj-$(CONFIG_MACH_PCM990_BASEBOARD)	+= pcm990-baseboard.o
obj-$(CONFIG_MACH_COLIBRI)	+= colibri-pxa270.o
obj-$(CONFIG_MACH_COLIBRI300)	+= colibri-pxa3xx.o colibri-pxa300.o
obj-$(CONFIG_MACH_COLIBRI320)	+= colibri-pxa3xx.o colibri-pxa320.o
obj-$(CONFIG_MACH_VPAC270)	+= vpac270.o

# End-user Products
obj-$(CONFIG_MACH_H4700)	+= hx4700.o
obj-$(CONFIG_MACH_H5000)	+= h5000.o
obj-$(CONFIG_MACH_HIMALAYA)	+= himalaya.o
obj-$(CONFIG_MACH_MAGICIAN)	+= magician.o
obj-$(CONFIG_MACH_MIOA701)	+= mioa701.o mioa701_bootresume.o
obj-$(CONFIG_PXA_EZX)           += ezx.o
obj-$(CONFIG_MACH_MP900C)	+= mp900.o
obj-$(CONFIG_MACH_PALMTE2)	+= palmte2.o
obj-$(CONFIG_MACH_PALMTC)	+= palmtc.o
obj-$(CONFIG_MACH_PALMT5)	+= palmt5.o
obj-$(CONFIG_MACH_PALMTX)	+= palmtx.o
obj-$(CONFIG_MACH_PALMZ72)	+= palmz72.o
obj-$(CONFIG_MACH_PALMLD)	+= palmld.o
obj-$(CONFIG_PALM_TREO)		+= palmtreo.o
obj-$(CONFIG_PXA_SHARP_C7xx)	+= corgi.o sharpsl_pm.o corgi_pm.o
obj-$(CONFIG_PXA_SHARP_Cxx00)	+= spitz.o sharpsl_pm.o spitz_pm.o
obj-$(CONFIG_MACH_POODLE)	+= poodle.o
obj-$(CONFIG_MACH_TOSA)		+= tosa.o
obj-$(CONFIG_MACH_ICONTROL)     += icontrol.o mxm8x10.o
obj-$(CONFIG_ARCH_PXA_ESERIES)	+= eseries.o
obj-$(CONFIG_MACH_E330)		+= e330.o
obj-$(CONFIG_MACH_E350)		+= e350.o
obj-$(CONFIG_MACH_E740)		+= e740.o
obj-$(CONFIG_MACH_E750)		+= e750.o
obj-$(CONFIG_MACH_E400)		+= e400.o
obj-$(CONFIG_MACH_E800)		+= e800.o
obj-$(CONFIG_MACH_RAUMFELD_RC)		+= raumfeld.o
obj-$(CONFIG_MACH_RAUMFELD_CONNECTOR)	+= raumfeld.o
obj-$(CONFIG_MACH_RAUMFELD_SPEAKER)	+= raumfeld.o
obj-$(CONFIG_MACH_ZIPIT2)	+= z2.o

# Support for blinky lights
led-y := leds.o
led-$(CONFIG_ARCH_LUBBOCK)	+= leds-lubbock.o
led-$(CONFIG_MACH_MAINSTONE)	+= leds-mainstone.o
led-$(CONFIG_ARCH_PXA_IDP)	+= leds-idp.o

obj-$(CONFIG_LEDS)		+= $(led-y)

obj-$(CONFIG_TOSA_BT)		+= tosa-bt.o
