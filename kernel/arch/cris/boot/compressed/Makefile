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
# arch/cris/boot/compressed/Makefile
#

asflags-y += $(LINUXINCLUDE)
ccflags-y += -O2 $(LINUXINCLUDE)

# asflags-$(CONFIG_ETRAX_ARCH_V32) += -I$(srctree)/include/asm/mach \
# 				    -I$(srctree)/include/asm/arch
# ccflags-$(CONFIG_ETRAX_ARCH_V32) += -O2 -I$(srctree)/include/asm/mach
# 					-I$(srctree)/include/asm/arch

arch-$(CONFIG_ETRAX_ARCH_V10) = v10
arch-$(CONFIG_ETRAX_ARCH_V32) = v32

ldflags-y += -T $(srctree)/$(src)/decompress_$(arch-y).lds

OBJECTS-$(CONFIG_ETRAX_ARCH_V32) = $(obj)/head_v32.o
OBJECTS-$(CONFIG_ETRAX_ARCH_V10) = $(obj)/head_v10.o
OBJECTS= $(OBJECTS-y) $(obj)/misc.o
OBJCOPYFLAGS = -O binary --remove-section=.bss

quiet_cmd_image = BUILD   $@
cmd_image = cat $(obj)/decompress.bin $(obj)/piggy.gz > $@

targets := vmlinux piggy.gz decompress.o decompress.bin

$(obj)/decompress.o: $(OBJECTS) FORCE
	$(call if_changed,ld)

$(obj)/decompress.bin: $(obj)/decompress.o FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux: $(obj)/piggy.gz $(obj)/decompress.bin FORCE
	$(call if_changed,image)

$(obj)/piggy.gz: $(obj)/../Image FORCE
	$(call if_changed,gzip)
