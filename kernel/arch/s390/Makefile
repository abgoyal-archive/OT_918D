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
# s390/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1994 by Linus Torvalds
#

ifndef CONFIG_64BIT
LD_BFD		:= elf32-s390
LDFLAGS		:= -m elf_s390
KBUILD_CFLAGS	+= -m31
KBUILD_AFLAGS	+= -m31
UTS_MACHINE	:= s390
STACK_SIZE	:= 8192
CHECKFLAGS	+= -D__s390__ -msize-long
else
LD_BFD		:= elf64-s390
LDFLAGS		:= -m elf64_s390
MODFLAGS	+= -fpic -D__PIC__
KBUILD_CFLAGS	+= -m64
KBUILD_AFLAGS	+= -m64
UTS_MACHINE	:= s390x
STACK_SIZE	:= 16384
CHECKFLAGS	+= -D__s390__ -D__s390x__
endif

export LD_BFD

cflags-$(CONFIG_MARCH_G5)   += $(call cc-option,-march=g5)
cflags-$(CONFIG_MARCH_Z900) += $(call cc-option,-march=z900)
cflags-$(CONFIG_MARCH_Z990) += $(call cc-option,-march=z990)
cflags-$(CONFIG_MARCH_Z9_109) += $(call cc-option,-march=z9-109)
cflags-$(CONFIG_MARCH_Z10) += $(call cc-option,-march=z10)

#KBUILD_IMAGE is necessary for make rpm
KBUILD_IMAGE	:=arch/s390/boot/image

#
# Prevent tail-call optimizations, to get clearer backtraces:
#
cflags-$(CONFIG_FRAME_POINTER) += -fno-optimize-sibling-calls

# old style option for packed stacks
ifeq ($(call cc-option-yn,-mkernel-backchain),y)
cflags-$(CONFIG_PACK_STACK)  += -mkernel-backchain -D__PACK_STACK
aflags-$(CONFIG_PACK_STACK)  += -D__PACK_STACK
cflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
aflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
ifdef CONFIG_SMALL_STACK
STACK_SIZE := $(shell echo $$(($(STACK_SIZE)/2)) )
endif
endif

# new style option for packed stacks
ifeq ($(call cc-option-yn,-mpacked-stack),y)
cflags-$(CONFIG_PACK_STACK)  += -mpacked-stack -D__PACK_STACK
aflags-$(CONFIG_PACK_STACK)  += -D__PACK_STACK
cflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
aflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
ifdef CONFIG_SMALL_STACK
STACK_SIZE := $(shell echo $$(($(STACK_SIZE)/2)) )
endif
endif

ifeq ($(call cc-option-yn,-mstack-size=8192 -mstack-guard=128),y)
cflags-$(CONFIG_CHECK_STACK) += -mstack-size=$(STACK_SIZE)
ifneq ($(call cc-option-yn,-mstack-size=8192),y)
cflags-$(CONFIG_CHECK_STACK) += -mstack-guard=$(CONFIG_STACK_GUARD)
endif
endif

ifeq ($(call cc-option-yn,-mwarn-dynamicstack),y)
cflags-$(CONFIG_WARN_STACK) += -mwarn-dynamicstack
cflags-$(CONFIG_WARN_STACK) += -mwarn-framesize=$(CONFIG_WARN_STACK_SIZE)
endif

KBUILD_CFLAGS	+= -mbackchain -msoft-float $(cflags-y)
KBUILD_CFLAGS	+= -pipe -fno-strength-reduce -Wno-sign-compare
KBUILD_AFLAGS	+= $(aflags-y)

OBJCOPYFLAGS	:= -O binary
LDFLAGS_vmlinux := -e start

head-y		:= arch/s390/kernel/head.o
head-y		+= arch/s390/kernel/$(if $(CONFIG_64BIT),head64.o,head31.o)
head-y		+= arch/s390/kernel/init_task.o

core-y		+= arch/s390/mm/ arch/s390/kernel/ arch/s390/crypto/ \
		   arch/s390/appldata/ arch/s390/hypfs/ arch/s390/kvm/

libs-y		+= arch/s390/lib/
drivers-y	+= drivers/s390/
drivers-$(CONFIG_MATHEMU) += arch/s390/math-emu/

# must be linked after kernel
drivers-$(CONFIG_OPROFILE)	+= arch/s390/oprofile/

boot		:= arch/s390/boot

all: image bzImage

install: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

image bzImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

zfcpdump:
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

vdso_install:
ifeq ($(CONFIG_64BIT),y)
	$(Q)$(MAKE) $(build)=arch/$(ARCH)/kernel/vdso64 $@
endif
	$(Q)$(MAKE) $(build)=arch/$(ARCH)/kernel/vdso32 $@

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

# Don't use tabs in echo arguments
define archhelp
  echo  '* image           - Kernel image for IPL ($(boot)/image)'
  echo	'* bzImage         - Compressed kernel image for IPL ($(boot)/bzImage)'
endef
