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

/*
 * DRAM/SDRAM initialization - alter with care
 * This file is intended to be included from other assembler files
 *
 * Note: This file may not modify r9 because r9 is used to carry
 *       information from the decompresser to the kernel
 *
 * Copyright (C) 2000, 2001 Axis Communications AB
 *
 * Authors:  Mikael Starvik (starvik@axis.com)
 *
 */

/* Just to be certain the config file is included, we include it here
 * explicitly instead of depending on it being included in the file that
 * uses this code.
 */


	;; WARNING! The registers r8 and r9 are used as parameters carrying
	;; information from the decompressor (if the kernel was compressed). 
	;; They should not be used in the code below.

#ifndef CONFIG_SVINTO_SIM	
	move.d   CONFIG_ETRAX_DEF_R_WAITSTATES, $r0
	move.d   $r0, [R_WAITSTATES]

	move.d   CONFIG_ETRAX_DEF_R_BUS_CONFIG, $r0
	move.d   $r0, [R_BUS_CONFIG]
	
#ifndef CONFIG_ETRAX_SDRAM
	move.d   CONFIG_ETRAX_DEF_R_DRAM_CONFIG, $r0
	move.d   $r0, [R_DRAM_CONFIG]

	move.d   CONFIG_ETRAX_DEF_R_DRAM_TIMING, $r0
	move.d   $r0, [R_DRAM_TIMING]
#else
	;; Samsung SDRAMs seem to require to be initialized twice to work properly.
	moveq    2, $r6	
_sdram_init:
	
	; Refer to ETRAX 100LX Designers Reference for a description of SDRAM initialization
	
	; Bank configuration
	move.d   CONFIG_ETRAX_DEF_R_SDRAM_CONFIG, $r0
	move.d   $r0, [R_SDRAM_CONFIG]

	; Calculate value of mrs_data 
	; CAS latency = 2 && bus_width = 32 => 0x40
	; CAS latency = 3 && bus_width = 32 => 0x60
	; CAS latency = 2 && bus_width = 16 => 0x20
	; CAS latency = 3 && bus_width = 16 => 0x30

	; Check if value is already supplied in kernel config
	move.d   CONFIG_ETRAX_DEF_R_SDRAM_TIMING, $r2
	and.d    0x00ff0000, $r2
	bne	 _set_timing
	lsrq     16, $r2
	
	move.d   0x40, $r2       ; Assume 32 bits and CAS latency = 2
	move.d   CONFIG_ETRAX_DEF_R_SDRAM_TIMING, $r1
	move.d   $r1, $r3
 	and.d    0x03, $r1       ; Get CAS latency
	and.d    0x1000, $r3     ; 50 or 100 MHz?
	beq      _speed_50
	nop
_speed_100:		
	cmp.d    0x00, $r1	; CAS latency = 2?
	beq      _bw_check
	nop
	or.d     0x20, $r2	; CAS latency = 3 
	ba       _bw_check
	nop
_speed_50:			
	cmp.d    0x01, $r1	; CAS latency = 2?
	beq      _bw_check
	nop
	or.d     0x20, $r2       ; CAS latency = 3
_bw_check:
	move.d   CONFIG_ETRAX_DEF_R_SDRAM_CONFIG, $r1
	and.d    0x800000, $r1	; DRAM width is bit 23
	bne      _set_timing
	nop
	lsrq     1, $r2		;  16 bits. Shift down value.

	; Set timing parameters. Starts master clock
_set_timing:
	move.d   CONFIG_ETRAX_DEF_R_SDRAM_TIMING, $r1
	and.d    0x8000f9ff, $r1 ; Make sure mrs data and command is 0 
	or.d     0x80000000, $r1	; Make sure sdram enable bit is set
	move.d   $r1, $r5
	or.d     0x0000c000, $r1 ; ref = disable
	lslq     16, $r2		; mrs data starts at bit 16
	or.d     $r2, $r1 
	move.d   $r1, [R_SDRAM_TIMING]	
		
	; Wait 200us
	move.d   10000, $r2
1:	bne      1b
	subq     1, $r2
	
	; Issue initialization command sequence
	move.d   _sdram_commands_start, $r2
	and.d    0x000fffff, $r2 ; Make sure commands are read from flash
	move.d   _sdram_commands_end,  $r3
	and.d    0x000fffff, $r3
1:	clear.d  $r4
	move.b   [$r2+], $r4
	lslq     9, $r4	; Command starts at bit 9
	or.d     $r1, $r4
	move.d   $r4, [R_SDRAM_TIMING]
	nop		; Wait five nop cycles between each command
	nop
	nop
	nop
	nop
	cmp.d    $r2, $r3
	bne      1b
	nop
	move.d   $r5, [R_SDRAM_TIMING]
	subq     1, $r6
	bne      _sdram_init
	nop
	ba       _sdram_commands_end
	nop

_sdram_commands_start:
	.byte   3	; Precharge
	.byte   0       ; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   2	; refresh
	.byte   0	; nop
	.byte   1	; mrs
	.byte   0	; nop 
_sdram_commands_end:		
#endif
#endif
