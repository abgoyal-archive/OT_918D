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
# Makefile for the drm device driver.  This driver provides support for the
# Direct Rendering Infrastructure (DRI) in XFree86 4.1.0 and higher.

ccflags-y := -Iinclude/drm

hostprogs-y := mkregtable

quiet_cmd_mkregtable = MKREGTABLE $@
      cmd_mkregtable = $(obj)/mkregtable $< > $@

$(obj)/rn50_reg_safe.h: $(src)/reg_srcs/rn50 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/r100_reg_safe.h: $(src)/reg_srcs/r100 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/r200_reg_safe.h: $(src)/reg_srcs/r200 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/rv515_reg_safe.h: $(src)/reg_srcs/rv515 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/r300_reg_safe.h: $(src)/reg_srcs/r300 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/r420_reg_safe.h: $(src)/reg_srcs/r420 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/rs600_reg_safe.h: $(src)/reg_srcs/rs600 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/r600_reg_safe.h: $(src)/reg_srcs/r600 $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/evergreen_reg_safe.h: $(src)/reg_srcs/evergreen $(obj)/mkregtable
	$(call if_changed,mkregtable)

$(obj)/r100.o: $(obj)/r100_reg_safe.h $(obj)/rn50_reg_safe.h

$(obj)/r200.o: $(obj)/r200_reg_safe.h

$(obj)/rv515.o: $(obj)/rv515_reg_safe.h

$(obj)/r300.o: $(obj)/r300_reg_safe.h

$(obj)/r420.o: $(obj)/r420_reg_safe.h

$(obj)/rs600.o: $(obj)/rs600_reg_safe.h

$(obj)/r600_cs.o: $(obj)/r600_reg_safe.h

$(obj)/evergreen_cs.o: $(obj)/evergreen_reg_safe.h

radeon-y := radeon_drv.o radeon_cp.o radeon_state.o radeon_mem.o \
	radeon_irq.o r300_cmdbuf.o r600_cp.o
# add KMS driver
radeon-y += radeon_device.o radeon_asic.o radeon_kms.o \
	radeon_atombios.o radeon_agp.o atombios_crtc.o radeon_combios.o \
	atom.o radeon_fence.o radeon_ttm.o radeon_object.o radeon_gart.o \
	radeon_legacy_crtc.o radeon_legacy_encoders.o radeon_connectors.o \
	radeon_encoders.o radeon_display.o radeon_cursor.o radeon_i2c.o \
	radeon_clocks.o radeon_fb.o radeon_gem.o radeon_ring.o radeon_irq_kms.o \
	radeon_cs.o radeon_bios.o radeon_benchmark.o r100.o r300.o r420.o \
	rs400.o rs600.o rs690.o rv515.o r520.o r600.o rv770.o radeon_test.o \
	r200.o radeon_legacy_tv.o r600_cs.o r600_blit.o r600_blit_shaders.o \
	r600_blit_kms.o radeon_pm.o atombios_dp.o r600_audio.o r600_hdmi.o \
	evergreen.o evergreen_cs.o

radeon-$(CONFIG_COMPAT) += radeon_ioc32.o
radeon-$(CONFIG_VGA_SWITCHEROO) += radeon_atpx_handler.o

obj-$(CONFIG_DRM_RADEON)+= radeon.o
