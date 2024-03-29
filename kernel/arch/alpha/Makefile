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
# alpha/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1994 by Linus Torvalds
#

NM := $(NM) -B

LDFLAGS_vmlinux	:= -static -N #-relax
CHECKFLAGS	+= -D__alpha__ -m64
cflags-y	:= -pipe -mno-fp-regs -ffixed-8 -msmall-data
cflags-y	+= $(call cc-option, -fno-jump-tables)

cpuflags-$(CONFIG_ALPHA_EV4)		:= -mcpu=ev4
cpuflags-$(CONFIG_ALPHA_EV5)		:= -mcpu=ev5
cpuflags-$(CONFIG_ALPHA_EV56)		:= -mcpu=ev56
cpuflags-$(CONFIG_ALPHA_POLARIS)	:= -mcpu=pca56
cpuflags-$(CONFIG_ALPHA_SX164)		:= -mcpu=pca56
cpuflags-$(CONFIG_ALPHA_EV6)		:= -mcpu=ev6
cpuflags-$(CONFIG_ALPHA_EV67)		:= -mcpu=ev67
# If GENERIC, make sure to turn off any instruction set extensions that
# the host compiler might have on by default.  Given that EV4 and EV5
# have the same instruction set, prefer EV5 because an EV5 schedule is
# more likely to keep an EV4 processor busy than vice-versa.
cpuflags-$(CONFIG_ALPHA_GENERIC)	:= -mcpu=ev5

cflags-y				+= $(cpuflags-y)


# For TSUNAMI, we must have the assembler not emulate our instructions.
# The same is true for IRONGATE, POLARIS, PYXIS.
# BWX is most important, but we don't really want any emulation ever.
KBUILD_CFLAGS += $(cflags-y) -Wa,-mev6

head-y := arch/alpha/kernel/head.o

core-y				+= arch/alpha/kernel/ arch/alpha/mm/
core-$(CONFIG_MATHEMU)		+= arch/alpha/math-emu/
drivers-$(CONFIG_OPROFILE)	+= arch/alpha/oprofile/
libs-y				+= arch/alpha/lib/

# export what is needed by arch/alpha/boot/Makefile
LIBS_Y := $(patsubst %/, %/lib.a, $(libs-y))
export LIBS_Y

boot := arch/alpha/boot

#Default target when executing make with no arguments
all boot: $(boot)/vmlinux.gz

$(boot)/vmlinux.gz: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

bootimage bootpfile bootpzfile: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

define archhelp
  echo '* boot		- Compressed kernel image (arch/alpha/boot/vmlinux.gz)'
  echo '  bootimage	- SRM bootable image (arch/alpha/boot/bootimage)'
  echo '  bootpfile	- BOOTP bootable image (arch/alpha/boot/bootpfile)'
  echo '  bootpzfile	- compressed kernel BOOTP image (arch/alpha/boot/bootpzfile)'
endef
