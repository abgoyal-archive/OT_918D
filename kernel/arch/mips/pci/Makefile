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
# Makefile for the PCI specific kernel interface routines under Linux.
#

obj-y				+= pci.o

#
# PCI bus host bridge specific code
#
obj-$(CONFIG_MIPS_BONITO64)	+= ops-bonito64.o
obj-$(CONFIG_PCI_GT64XXX_PCI0)	+= ops-gt64xxx_pci0.o
obj-$(CONFIG_MIPS_MSC)		+= ops-msc.o
obj-$(CONFIG_MIPS_NILE4)	+= ops-nile4.o
obj-$(CONFIG_SOC_TX3927)	+= ops-tx3927.o
obj-$(CONFIG_PCI_VR41XX)	+= ops-vr41xx.o pci-vr41xx.o
obj-$(CONFIG_NEC_MARKEINS)	+= ops-emma2rh.o pci-emma2rh.o fixup-emma2rh.o
obj-$(CONFIG_PCI_TX4927)	+= ops-tx4927.o
obj-$(CONFIG_BCM47XX)		+= pci-bcm47xx.o
obj-$(CONFIG_BCM63XX)		+= pci-bcm63xx.o fixup-bcm63xx.o \
					ops-bcm63xx.o

#
# These are still pretty much in the old state, watch, go blind.
#
obj-$(CONFIG_LASAT)		+= pci-lasat.o
obj-$(CONFIG_MIPS_COBALT)	+= fixup-cobalt.o
obj-$(CONFIG_SOC_AU1500)	+= fixup-au1000.o ops-au1000.o
obj-$(CONFIG_SOC_AU1550)	+= fixup-au1000.o ops-au1000.o
obj-$(CONFIG_SOC_PNX8550)	+= fixup-pnx8550.o ops-pnx8550.o
obj-$(CONFIG_LEMOTE_FULOONG2E)	+= fixup-fuloong2e.o ops-loongson2.o
obj-$(CONFIG_LEMOTE_MACH2F)	+= fixup-lemote2f.o ops-loongson2.o
obj-$(CONFIG_MIPS_MALTA)	+= fixup-malta.o
obj-$(CONFIG_PMC_MSP7120_GW)	+= fixup-pmcmsp.o ops-pmcmsp.o
obj-$(CONFIG_PMC_MSP7120_EVAL)	+= fixup-pmcmsp.o ops-pmcmsp.o
obj-$(CONFIG_PMC_MSP7120_FPGA)	+= fixup-pmcmsp.o ops-pmcmsp.o
obj-$(CONFIG_PMC_YOSEMITE)	+= fixup-yosemite.o ops-titan.o ops-titan-ht.o \
				   pci-yosemite.o
obj-$(CONFIG_SGI_IP27)		+= ops-bridge.o pci-ip27.o
obj-$(CONFIG_SGI_IP32)		+= fixup-ip32.o ops-mace.o pci-ip32.o
obj-$(CONFIG_SIBYTE_SB1250)	+= fixup-sb1250.o pci-sb1250.o
obj-$(CONFIG_SIBYTE_BCM112X)	+= fixup-sb1250.o pci-sb1250.o
obj-$(CONFIG_SIBYTE_BCM1x80)	+= pci-bcm1480.o pci-bcm1480ht.o
obj-$(CONFIG_SNI_RM)		+= fixup-sni.o ops-sni.o
obj-$(CONFIG_TANBAC_TB0219)	+= fixup-tb0219.o
obj-$(CONFIG_TANBAC_TB0226)	+= fixup-tb0226.o
obj-$(CONFIG_TANBAC_TB0287)	+= fixup-tb0287.o
obj-$(CONFIG_TOSHIBA_JMR3927)	+= fixup-jmr3927.o
obj-$(CONFIG_SOC_TX4927)	+= pci-tx4927.o
obj-$(CONFIG_SOC_TX4938)	+= pci-tx4938.o
obj-$(CONFIG_SOC_TX4939)	+= pci-tx4939.o
obj-$(CONFIG_TOSHIBA_RBTX4927)	+= fixup-rbtx4927.o
obj-$(CONFIG_TOSHIBA_RBTX4938)	+= fixup-rbtx4938.o
obj-$(CONFIG_VICTOR_MPC30X)	+= fixup-mpc30x.o
obj-$(CONFIG_ZAO_CAPCELLA)	+= fixup-capcella.o
obj-$(CONFIG_WR_PPMC)		+= fixup-wrppmc.o
obj-$(CONFIG_MIKROTIK_RB532)	+= pci-rc32434.o ops-rc32434.o fixup-rc32434.o
obj-$(CONFIG_CPU_CAVIUM_OCTEON)	+= pci-octeon.o pcie-octeon.o

ifdef CONFIG_PCI_MSI
obj-$(CONFIG_CPU_CAVIUM_OCTEON)	+= msi-octeon.o
endif
