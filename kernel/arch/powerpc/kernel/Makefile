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
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


#
# Makefile for the linux kernel.
#

CFLAGS_ptrace.o		+= -DUTS_MACHINE='"$(UTS_MACHINE)"'

subdir-ccflags-$(CONFIG_PPC_WERROR) := -Werror

ifeq ($(CONFIG_PPC64),y)
CFLAGS_prom_init.o	+= -mno-minimal-toc
endif
ifeq ($(CONFIG_PPC32),y)
CFLAGS_prom_init.o      += -fPIC
CFLAGS_btext.o		+= -fPIC
endif

ifdef CONFIG_FUNCTION_TRACER
# Do not trace early boot code
CFLAGS_REMOVE_cputable.o = -pg -mno-sched-epilog
CFLAGS_REMOVE_prom_init.o = -pg -mno-sched-epilog
CFLAGS_REMOVE_btext.o = -pg -mno-sched-epilog
CFLAGS_REMOVE_prom.o = -pg -mno-sched-epilog
# do not trace tracer code
CFLAGS_REMOVE_ftrace.o = -pg -mno-sched-epilog
# timers used by tracing
CFLAGS_REMOVE_time.o = -pg -mno-sched-epilog
endif

obj-y				:= cputable.o ptrace.o syscalls.o \
				   irq.o align.o signal_32.o pmc.o vdso.o \
				   init_task.o process.o systbl.o idle.o \
				   signal.o sysfs.o cacheinfo.o
obj-y				+= vdso32/
obj-$(CONFIG_PPC64)		+= setup_64.o sys_ppc32.o \
				   signal_64.o ptrace32.o \
				   paca.o nvram_64.o firmware.o
obj-$(CONFIG_PPC_BOOK3S_64)	+= cpu_setup_ppc970.o cpu_setup_pa6t.o
obj64-$(CONFIG_RELOCATABLE)	+= reloc_64.o
obj-$(CONFIG_PPC_BOOK3E_64)	+= exceptions-64e.o
obj-$(CONFIG_PPC64)		+= vdso64/
obj-$(CONFIG_ALTIVEC)		+= vecemu.o
obj-$(CONFIG_PPC_970_NAP)	+= idle_power4.o
obj-$(CONFIG_PPC_OF)		+= of_device.o of_platform.o prom_parse.o
obj-$(CONFIG_PPC_CLOCK)		+= clock.o
procfs-y			:= proc_powerpc.o
obj-$(CONFIG_PROC_FS)		+= $(procfs-y)
rtaspci-$(CONFIG_PPC64)-$(CONFIG_PCI)	:= rtas_pci.o
obj-$(CONFIG_PPC_RTAS)		+= rtas.o rtas-rtc.o $(rtaspci-y-y)
obj-$(CONFIG_PPC_RTAS_DAEMON)	+= rtasd.o
obj-$(CONFIG_RTAS_FLASH)	+= rtas_flash.o
obj-$(CONFIG_RTAS_PROC)		+= rtas-proc.o
obj-$(CONFIG_LPARCFG)		+= lparcfg.o
obj-$(CONFIG_IBMVIO)		+= vio.o
obj-$(CONFIG_IBMEBUS)           += ibmebus.o
obj-$(CONFIG_GENERIC_TBSYNC)	+= smp-tbsync.o
obj-$(CONFIG_CRASH_DUMP)	+= crash_dump.o
obj-$(CONFIG_E500)		+= idle_e500.o
obj-$(CONFIG_6xx)		+= idle_6xx.o l2cr_6xx.o cpu_setup_6xx.o
obj-$(CONFIG_TAU)		+= tau_6xx.o
obj-$(CONFIG_HIBERNATION)	+= swsusp.o suspend.o
ifeq ($(CONFIG_FSL_BOOKE),y)
obj-$(CONFIG_HIBERNATION)	+= swsusp_booke.o
else
obj-$(CONFIG_HIBERNATION)	+= swsusp_$(CONFIG_WORD_SIZE).o
endif
obj64-$(CONFIG_HIBERNATION)	+= swsusp_asm64.o
obj-$(CONFIG_MODULES)		+= module.o module_$(CONFIG_WORD_SIZE).o
obj-$(CONFIG_44x)		+= cpu_setup_44x.o
obj-$(CONFIG_FSL_BOOKE)		+= cpu_setup_fsl_booke.o dbell.o
obj-$(USE_IMMEDIATE)		+= immediate.o

extra-y				:= head_$(CONFIG_WORD_SIZE).o
extra-$(CONFIG_PPC_BOOK3E_32)	:= head_new_booke.o
extra-$(CONFIG_40x)		:= head_40x.o
extra-$(CONFIG_44x)		:= head_44x.o
extra-$(CONFIG_FSL_BOOKE)	:= head_fsl_booke.o
extra-$(CONFIG_8xx)		:= head_8xx.o
extra-y				+= vmlinux.lds

obj-y				+= time.o prom.o traps.o setup-common.o \
				   udbg.o misc.o io.o dma.o \
				   misc_$(CONFIG_WORD_SIZE).o
obj-$(CONFIG_PPC32)		+= entry_32.o setup_32.o
obj-$(CONFIG_PPC64)		+= dma-iommu.o iommu.o
obj-$(CONFIG_KGDB)		+= kgdb.o
obj-$(CONFIG_PPC_OF_BOOT_TRAMPOLINE)	+= prom_init.o
obj-$(CONFIG_MODULES)		+= ppc_ksyms.o
obj-$(CONFIG_BOOTX_TEXT)	+= btext.o
obj-$(CONFIG_SMP)		+= smp.o
obj-$(CONFIG_KPROBES)		+= kprobes.o
obj-$(CONFIG_PPC_UDBG_16550)	+= legacy_serial.o udbg_16550.o
obj-$(CONFIG_STACKTRACE)	+= stacktrace.o
obj-$(CONFIG_SWIOTLB)		+= dma-swiotlb.o

pci64-$(CONFIG_PPC64)		+= pci_dn.o isa-bridge.o
obj-$(CONFIG_PCI)		+= pci_$(CONFIG_WORD_SIZE).o $(pci64-y) \
				   pci-common.o pci_of_scan.o
obj-$(CONFIG_PCI_MSI)		+= msi.o
obj-$(CONFIG_KEXEC)		+= machine_kexec.o crash.o \
				   machine_kexec_$(CONFIG_WORD_SIZE).o
obj-$(CONFIG_AUDIT)		+= audit.o
obj64-$(CONFIG_AUDIT)		+= compat_audit.o

obj-$(CONFIG_DYNAMIC_FTRACE)	+= ftrace.o
obj-$(CONFIG_FUNCTION_GRAPH_TRACER)	+= ftrace.o
obj-$(CONFIG_PERF_EVENTS)	+= perf_callchain.o

obj-$(CONFIG_PPC_PERF_CTRS)	+= perf_event.o
obj64-$(CONFIG_PPC_PERF_CTRS)	+= power4-pmu.o ppc970-pmu.o power5-pmu.o \
				   power5+-pmu.o power6-pmu.o power7-pmu.o
obj32-$(CONFIG_PPC_PERF_CTRS)	+= mpc7450-pmu.o

obj-$(CONFIG_FSL_EMB_PERF_EVENT) += perf_event_fsl_emb.o
obj-$(CONFIG_FSL_EMB_PERF_EVENT_E500) += e500-pmu.o

obj-$(CONFIG_8XX_MINIMAL_FPEMU) += softemu8xx.o

ifneq ($(CONFIG_PPC_INDIRECT_IO),y)
obj-y				+= iomap.o
endif

obj-$(CONFIG_PPC64)		+= $(obj64-y)
obj-$(CONFIG_PPC32)		+= $(obj32-y)

ifneq ($(CONFIG_XMON)$(CONFIG_KEXEC),)
obj-y				+= ppc_save_regs.o
endif

# Disable GCOV in odd or sensitive code
GCOV_PROFILE_prom_init.o := n
GCOV_PROFILE_ftrace.o := n
GCOV_PROFILE_machine_kexec_64.o := n
GCOV_PROFILE_machine_kexec_32.o := n
GCOV_PROFILE_kprobes.o := n

extra-$(CONFIG_PPC_FPU)		+= fpu.o
extra-$(CONFIG_ALTIVEC)		+= vector.o
extra-$(CONFIG_PPC64)		+= entry_64.o

extra-y				+= systbl_chk.i
$(obj)/systbl.o:		systbl_chk

quiet_cmd_systbl_chk = CALL    $<
      cmd_systbl_chk = $(CONFIG_SHELL) $< $(obj)/systbl_chk.i

PHONY += systbl_chk
systbl_chk: $(src)/systbl_chk.sh $(obj)/systbl_chk.i
	$(call cmd,systbl_chk)

ifeq ($(CONFIG_PPC_OF_BOOT_TRAMPOLINE),y)
$(obj)/built-in.o:		prom_init_check

quiet_cmd_prom_init_check = CALL    $<
      cmd_prom_init_check = $(CONFIG_SHELL) $< "$(NM)" "$(obj)/prom_init.o"

PHONY += prom_init_check
prom_init_check: $(src)/prom_init_check.sh $(obj)/prom_init.o
	$(call cmd,prom_init_check)
endif

clean-files := vmlinux.lds
