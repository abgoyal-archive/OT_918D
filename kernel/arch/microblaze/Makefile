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


ifeq ($(CONFIG_MMU),y)
UTS_SYSNAME = -DUTS_SYSNAME=\"Linux\"
else
UTS_SYSNAME = -DUTS_SYSNAME=\"uClinux\"
endif

# What CPU vesion are we building for, and crack it open
# as major.minor.rev
CPU_VER   := $(shell echo $(CONFIG_XILINX_MICROBLAZE0_HW_VER))
CPU_MAJOR := $(shell echo $(CPU_VER) | cut -d '.' -f 1)
CPU_MINOR := $(shell echo $(CPU_VER) | cut -d '.' -f 2)
CPU_REV   := $(shell echo $(CPU_VER) | cut -d '.' -f 3)

export CPU_VER CPU_MAJOR CPU_MINOR CPU_REV

# Use cpu-related CONFIG_ vars to set compile options.
# The various CONFIG_XILINX cpu features options are integers 0/1/2...
# rather than bools y/n

# Work out HW multipler support.  This is icky.
# 1. Spartan2 has no HW multiplers.
# 2. MicroBlaze v3.x always uses them, except in Spartan 2
# 3. All other FPGa/CPU ver combos, we can trust the CONFIG_ settings
ifeq (,$(findstring spartan2,$(CONFIG_XILINX_MICROBLAZE0_FAMILY)))
  ifeq ($(CPU_MAJOR),3)
    CPUFLAGS-1 += -mno-xl-soft-mul
  else
    # USE_HW_MUL can be 0, 1, or 2, defining a heirarchy of HW Mul support.
    CPUFLAGS-$(subst 1,,$(CONFIG_XILINX_MICROBLAZE0_USE_HW_MUL)) += -mxl-multiply-high
    CPUFLAGS-$(CONFIG_XILINX_MICROBLAZE0_USE_HW_MUL) += -mno-xl-soft-mul
  endif
endif
CPUFLAGS-$(CONFIG_XILINX_MICROBLAZE0_USE_DIV) += -mno-xl-soft-div
CPUFLAGS-$(CONFIG_XILINX_MICROBLAZE0_USE_BARREL) += -mxl-barrel-shift
CPUFLAGS-$(CONFIG_XILINX_MICROBLAZE0_USE_PCMP_INSTR) += -mxl-pattern-compare

CPUFLAGS-1 += $(call cc-option,-mcpu=v$(CPU_VER))

# r31 holds current when in kernel mode
KBUILD_CFLAGS += -ffixed-r31 $(CPUFLAGS-1) $(CPUFLAGS-2)

LDFLAGS		:=
LDFLAGS_vmlinux	:=

LIBGCC := $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

head-y := arch/microblaze/kernel/head.o
libs-y += arch/microblaze/lib/
libs-y += $(LIBGCC)
core-y += arch/microblaze/kernel/
core-y += arch/microblaze/mm/
core-y += arch/microblaze/platform/
core-$(CONFIG_PCI) += arch/microblaze/pci/

drivers-$(CONFIG_OPROFILE) += arch/microblaze/oprofile/

boot := arch/microblaze/boot

# Are we making a simpleImage.<boardname> target? If so, crack out the boardname
DTB:=$(subst simpleImage.,,$(filter simpleImage.%, $(MAKECMDGOALS)))

ifneq ($(DTB),)
	core-y	+= $(boot)/
endif

# defines filename extension depending memory management type
ifeq ($(CONFIG_MMU),)
MMU := -nommu
endif

export MMU DTB

all: linux.bin

BOOT_TARGETS = linux.bin linux.bin.gz simpleImage.%

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

$(BOOT_TARGETS): vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

define archhelp
  echo '* linux.bin    - Create raw binary'
  echo '  linux.bin.gz - Create compressed raw binary'
  echo '  simpleImage.<dt> - ELF image with $(arch)/boot/dts/<dt>.dts linked in'
  echo '                   - stripped elf with fdt blob'
  echo '  simpleImage.<dt>.unstrip - full ELF image with fdt blob'
  echo '  *_defconfig      - Select default config from arch/microblaze/configs'
  echo ''
  echo '  Targets with <dt> embed a device tree blob inside the image'
  echo '  These targets support board with firmware that does not'
  echo '  support passing a device tree directly. Replace <dt> with the'
  echo '  name of a dts file from the arch/microblaze/boot/dts/ directory'
  echo '  (minus the .dts extension).'
endef

MRPROPER_FILES += $(boot)/simpleImage.*
