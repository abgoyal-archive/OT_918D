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

/*****************************************************************************
* Copyright 2003 - 2008 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <csp/stdint.h>
#include <mach/csp/chipcHw_def.h>
#include <mach/csp/chipcHw_inline.h>
#include <csp/intcHw.h>
#include <csp/cache.h>

/* ---- Private Constants and Types --------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
void chipcHw_reset_run_from_aram(void);

typedef void (*RUNFUNC) (void);

/****************************************************************************/
/**
*  @brief   warmReset
*
*  @note warmReset configures the clocks which are not reset back to the state
*   required to execute on reset.  To do so we need to copy the code into internal
*   memory to change the ARM clock while we are not executing from DDR.
*/
/****************************************************************************/
void chipcHw_reset(uint32_t mask)
{
	int i = 0;
	RUNFUNC runFunc = (RUNFUNC) (unsigned long)MM_ADDR_IO_ARAM;

	/* Disable all interrupts */
	intcHw_irq_disable(INTCHW_INTC0, 0xffffffff);
	intcHw_irq_disable(INTCHW_INTC1, 0xffffffff);
	intcHw_irq_disable(INTCHW_SINTC, 0xffffffff);

	{
		REG_LOCAL_IRQ_SAVE;
		if (mask & chipcHw_REG_SOFT_RESET_CHIP_SOFT) {
			chipcHw_softReset(chipcHw_REG_SOFT_RESET_CHIP_SOFT);
		}
		/* Bypass the PLL clocks before reboot */
		pChipcHw->UARTClock |= chipcHw_REG_PLL_CLOCK_BYPASS_SELECT;
		pChipcHw->SPIClock |= chipcHw_REG_PLL_CLOCK_BYPASS_SELECT;

		/* Copy the chipcHw_warmReset_run_from_aram function into ARAM */
		do {
			((uint32_t *) MM_IO_BASE_ARAM)[i] =
			    ((uint32_t *) &chipcHw_reset_run_from_aram)[i];
			i++;
		} while (((uint32_t *) MM_IO_BASE_ARAM)[i - 1] != 0xe1a0f00f);	/* 0xe1a0f00f == asm ("mov r15, r15"); */

		CSP_CACHE_FLUSH_ALL;

		/* run the function from ARAM */
		runFunc();

		/* Code will never get here, but include it to balance REG_LOCAL_IRQ_SAVE above */
		REG_LOCAL_IRQ_RESTORE;
	}
}

/* This function must run from internal memory */
void chipcHw_reset_run_from_aram(void)
{
/* Make sure, pipeline is filled with instructions coming from ARAM */
__asm (" nop                                                            \n\t"
		" nop                                                            \n\t"
#if defined(__KERNEL__) && !defined(STANDALONE)
		" MRC      p15,#0x0,r0,c1,c0,#0                                  \n\t"
		" BIC      r0,r0,#0xd                                            \n\t"
		" MCR      p15,#0x0,r0,c1,c0,#0                                  \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
#endif
		" nop                                                            \n\t"
		" nop                                                            \n\t"
/* Bypass the ARM clock and switch to XTAL clock */
		" MOV      r2,#0x80000000                                        \n\t"
		" LDR      r3,[r2,#8]                                            \n\t"
		" ORR      r3,r3,#0x20000                                        \n\t"
		" STR      r3,[r2,#8]                                            \n\t"

		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
		" nop                                                            \n\t"
/* Issue reset */
		" MOV      r3,#0x2                                               \n\t"
		" STR      r3,[r2,#0x80]                                         \n\t"
/* End here */
		" MOV      pc,pc                                                 \n\t");
/* 0xe1a0f00f ==  asm ("mov r15, r15"); */
}
