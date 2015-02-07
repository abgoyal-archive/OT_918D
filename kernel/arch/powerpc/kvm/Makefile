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
# Makefile for Kernel-based Virtual Machine module
#

subdir-ccflags-$(CONFIG_PPC_WERROR) := -Werror

EXTRA_CFLAGS += -Ivirt/kvm -Iarch/powerpc/kvm

common-objs-y = $(addprefix ../../../virt/kvm/, kvm_main.o coalesced_mmio.o)

CFLAGS_44x_tlb.o  := -I.
CFLAGS_e500_tlb.o := -I.
CFLAGS_emulate.o  := -I.

common-objs-y += powerpc.o emulate.o
obj-$(CONFIG_KVM_EXIT_TIMING) += timing.o
obj-$(CONFIG_KVM_BOOK3S_HANDLER) += book3s_exports.o

AFLAGS_booke_interrupts.o := -I$(obj)

kvm-440-objs := \
	$(common-objs-y) \
	booke.o \
	booke_emulate.o \
	booke_interrupts.o \
	44x.o \
	44x_tlb.o \
	44x_emulate.o
kvm-objs-$(CONFIG_KVM_440) := $(kvm-440-objs)

kvm-e500-objs := \
	$(common-objs-y) \
	booke.o \
	booke_emulate.o \
	booke_interrupts.o \
	e500.o \
	e500_tlb.o \
	e500_emulate.o
kvm-objs-$(CONFIG_KVM_E500) := $(kvm-e500-objs)

kvm-book3s_64-objs := \
	$(common-objs-y) \
	fpu.o \
	book3s_paired_singles.o \
	book3s.o \
	book3s_emulate.o \
	book3s_interrupts.o \
	book3s_64_mmu_host.o \
	book3s_64_mmu.o \
	book3s_32_mmu.o
kvm-objs-$(CONFIG_KVM_BOOK3S_64) := $(kvm-book3s_64-objs)

kvm-book3s_32-objs := \
	$(common-objs-y) \
	fpu.o \
	book3s_paired_singles.o \
	book3s.o \
	book3s_emulate.o \
	book3s_interrupts.o \
	book3s_32_mmu_host.o \
	book3s_32_mmu.o
kvm-objs-$(CONFIG_KVM_BOOK3S_32) := $(kvm-book3s_32-objs)

kvm-objs := $(kvm-objs-m) $(kvm-objs-y)

obj-$(CONFIG_KVM_440) += kvm.o
obj-$(CONFIG_KVM_E500) += kvm.o
obj-$(CONFIG_KVM_BOOK3S_64) += kvm.o
obj-$(CONFIG_KVM_BOOK3S_32) += kvm.o

