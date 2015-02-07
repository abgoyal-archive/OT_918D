/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/***********************license start***************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
 ***********************license end**************************************/

/**
 *
 * This file provides support for the processor local scratch memory.
 * Scratch memory is byte addressable - all addresses are byte addresses.
 *
 */

#ifndef __CVMX_SCRATCH_H__
#define __CVMX_SCRATCH_H__

/*
 * Note: This define must be a long, not a long long in order to
 * compile without warnings for both 32bit and 64bit.
 */
#define CVMX_SCRATCH_BASE       (-32768l)	/* 0xffffffffffff8000 */

/**
 * Reads an 8 bit value from the processor local scratchpad memory.
 *
 * @address: byte address to read from
 *
 * Returns value read
 */
static inline uint8_t cvmx_scratch_read8(uint64_t address)
{
	return *CASTPTR(volatile uint8_t, CVMX_SCRATCH_BASE + address);
}

/**
 * Reads a 16 bit value from the processor local scratchpad memory.
 *
 * @address: byte address to read from
 *
 * Returns value read
 */
static inline uint16_t cvmx_scratch_read16(uint64_t address)
{
	return *CASTPTR(volatile uint16_t, CVMX_SCRATCH_BASE + address);
}

/**
 * Reads a 32 bit value from the processor local scratchpad memory.
 *
 * @address: byte address to read from
 *
 * Returns value read
 */
static inline uint32_t cvmx_scratch_read32(uint64_t address)
{
	return *CASTPTR(volatile uint32_t, CVMX_SCRATCH_BASE + address);
}

/**
 * Reads a 64 bit value from the processor local scratchpad memory.
 *
 * @address: byte address to read from
 *
 * Returns value read
 */
static inline uint64_t cvmx_scratch_read64(uint64_t address)
{
	return *CASTPTR(volatile uint64_t, CVMX_SCRATCH_BASE + address);
}

/**
 * Writes an 8 bit value to the processor local scratchpad memory.
 *
 * @address: byte address to write to
 * @value:   value to write
 */
static inline void cvmx_scratch_write8(uint64_t address, uint64_t value)
{
	*CASTPTR(volatile uint8_t, CVMX_SCRATCH_BASE + address) =
	    (uint8_t) value;
}

/**
 * Writes a 32 bit value to the processor local scratchpad memory.
 *
 * @address: byte address to write to
 * @value:   value to write
 */
static inline void cvmx_scratch_write16(uint64_t address, uint64_t value)
{
	*CASTPTR(volatile uint16_t, CVMX_SCRATCH_BASE + address) =
	    (uint16_t) value;
}

/**
 * Writes a 16 bit value to the processor local scratchpad memory.
 *
 * @address: byte address to write to
 * @value:   value to write
 */
static inline void cvmx_scratch_write32(uint64_t address, uint64_t value)
{
	*CASTPTR(volatile uint32_t, CVMX_SCRATCH_BASE + address) =
	    (uint32_t) value;
}

/**
 * Writes a 64 bit value to the processor local scratchpad memory.
 *
 * @address: byte address to write to
 * @value:   value to write
 */
static inline void cvmx_scratch_write64(uint64_t address, uint64_t value)
{
	*CASTPTR(volatile uint64_t, CVMX_SCRATCH_BASE + address) = value;
}

#endif /* __CVMX_SCRATCH_H__ */
