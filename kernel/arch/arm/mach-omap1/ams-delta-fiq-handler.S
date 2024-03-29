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
 *  linux/arch/arm/mach-omap1/ams-delta-fiq-handler.S
 *
 *  Based on  linux/arch/arm/lib/floppydma.S
 *  Renamed and modified to work with 2.6 kernel by Matt Callow
 *  Copyright (C) 1995, 1996 Russell King
 *  Copyright (C) 2004 Pete Trapps
 *  Copyright (C) 2006 Matt Callow
 *  Copyright (C) 2010 Janusz Krzysztofik
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

#include <linux/linkage.h>

#include <plat/io.h>
#include <plat/board-ams-delta.h>

#include <mach/ams-delta-fiq.h>

/*
 * GPIO related definitions, copied from arch/arm/plat-omap/gpio.c.
 * Unfortunately, those were not placed in a separate header file.
 */
#define OMAP1510_GPIO_BASE		0xFFFCE000
#define OMAP1510_GPIO_DATA_INPUT	0x00
#define OMAP1510_GPIO_DATA_OUTPUT	0x04
#define OMAP1510_GPIO_DIR_CONTROL	0x08
#define OMAP1510_GPIO_INT_CONTROL	0x0c
#define OMAP1510_GPIO_INT_MASK		0x10
#define OMAP1510_GPIO_INT_STATUS	0x14
#define OMAP1510_GPIO_PIN_CONTROL	0x18

/* GPIO register bitmasks */
#define KEYBRD_DATA_MASK		(0x1 << AMS_DELTA_GPIO_PIN_KEYBRD_DATA)
#define KEYBRD_CLK_MASK			(0x1 << AMS_DELTA_GPIO_PIN_KEYBRD_CLK)
#define MODEM_IRQ_MASK			(0x1 << AMS_DELTA_GPIO_PIN_MODEM_IRQ)
#define HOOK_SWITCH_MASK		(0x1 << AMS_DELTA_GPIO_PIN_HOOK_SWITCH)
#define OTHERS_MASK			(MODEM_IRQ_MASK | HOOK_SWITCH_MASK)

/* IRQ handler register bitmasks */
#define DEFERRED_FIQ_MASK		(0x1 << (INT_DEFERRED_FIQ % IH2_BASE))
#define GPIO_BANK1_MASK  		(0x1 << INT_GPIO_BANK1)

/* Driver buffer byte offsets */
#define BUF_MASK			(FIQ_MASK * 4)
#define BUF_STATE			(FIQ_STATE * 4)
#define BUF_KEYS_CNT			(FIQ_KEYS_CNT * 4)
#define BUF_TAIL_OFFSET			(FIQ_TAIL_OFFSET * 4)
#define BUF_HEAD_OFFSET			(FIQ_HEAD_OFFSET * 4)
#define BUF_BUF_LEN			(FIQ_BUF_LEN * 4)
#define BUF_KEY				(FIQ_KEY * 4)
#define BUF_MISSED_KEYS			(FIQ_MISSED_KEYS * 4)
#define BUF_BUFFER_START		(FIQ_BUFFER_START * 4)
#define BUF_GPIO_INT_MASK		(FIQ_GPIO_INT_MASK * 4)
#define BUF_KEYS_HICNT			(FIQ_KEYS_HICNT * 4)
#define BUF_IRQ_PEND			(FIQ_IRQ_PEND * 4)
#define BUF_SIR_CODE_L1			(FIQ_SIR_CODE_L1 * 4)
#define BUF_SIR_CODE_L2			(IRQ_SIR_CODE_L2 * 4)
#define BUF_CNT_INT_00			(FIQ_CNT_INT_00 * 4)
#define BUF_CNT_INT_KEY			(FIQ_CNT_INT_KEY * 4)
#define BUF_CNT_INT_MDM			(FIQ_CNT_INT_MDM * 4)
#define BUF_CNT_INT_03			(FIQ_CNT_INT_03 * 4)
#define BUF_CNT_INT_HSW			(FIQ_CNT_INT_HSW * 4)
#define BUF_CNT_INT_05			(FIQ_CNT_INT_05 * 4)
#define BUF_CNT_INT_06			(FIQ_CNT_INT_06 * 4)
#define BUF_CNT_INT_07			(FIQ_CNT_INT_07 * 4)
#define BUF_CNT_INT_08			(FIQ_CNT_INT_08 * 4)
#define BUF_CNT_INT_09			(FIQ_CNT_INT_09 * 4)
#define BUF_CNT_INT_10			(FIQ_CNT_INT_10 * 4)
#define BUF_CNT_INT_11			(FIQ_CNT_INT_11 * 4)
#define BUF_CNT_INT_12			(FIQ_CNT_INT_12 * 4)
#define BUF_CNT_INT_13			(FIQ_CNT_INT_13 * 4)
#define BUF_CNT_INT_14			(FIQ_CNT_INT_14 * 4)
#define BUF_CNT_INT_15			(FIQ_CNT_INT_15 * 4)
#define BUF_CIRC_BUFF			(FIQ_CIRC_BUFF * 4)


/*
 * Register useage
 * r8  - temporary
 * r9  - the driver buffer
 * r10 - temporary
 * r11 - interrupts mask
 * r12 - base pointers
 * r13 - interrupts status
 */

	.text

	.global qwerty_fiqin_end

ENTRY(qwerty_fiqin_start)
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@ FIQ intrrupt handler
	ldr r12, omap_ih1_base			@ set pointer to level1 handler

	ldr r11, [r12, #IRQ_MIR_REG_OFFSET]	@ fetch interrupts mask

	ldr r13, [r12, #IRQ_ITR_REG_OFFSET]	@ fetch interrupts status
	bics r13, r13, r11			@ clear masked - any left?
	beq exit				@ none - spurious FIQ? exit

	ldr r10, [r12, #IRQ_SIR_FIQ_REG_OFFSET]	@ get requested interrupt number

	mov r8, #2				@ reset FIQ agreement
	str r8, [r12, #IRQ_CONTROL_REG_OFFSET]

	cmp r10, #INT_GPIO_BANK1		@ is it GPIO bank interrupt?
	beq gpio				@ yes - process it

	mov r8, #1
	orr r8, r11, r8, lsl r10		@ mask spurious interrupt
	str r8, [r12, #IRQ_MIR_REG_OFFSET]
exit:
	subs	pc, lr, #4			@ return from FIQ
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


	@@@@@@@@@@@@@@@@@@@@@@@@@@@
gpio:	@ GPIO bank interrupt handler
	ldr r12, omap1510_gpio_base		@ set base pointer to GPIO bank

	ldr r11, [r12, #OMAP1510_GPIO_INT_MASK]	@ fetch GPIO interrupts mask
restart:
	ldr r13, [r12, #OMAP1510_GPIO_INT_STATUS]	@ fetch status bits
	bics r13, r13, r11			@ clear masked - any left?
	beq exit				@ no - spurious interrupt? exit

	orr r11, r11, r13			@ mask all requested interrupts
	str r11, [r12, #OMAP1510_GPIO_INT_MASK]

	ands r10, r13, #KEYBRD_CLK_MASK		@ extract keyboard status - set?
	beq hksw				@ no - try next source


	@@@@@@@@@@@@@@@@@@@@@@
	@ Keyboard clock FIQ mode interrupt handler
	@ r10 now contains KEYBRD_CLK_MASK, use it
	str r10, [r12, #OMAP1510_GPIO_INT_STATUS]	@ ack the interrupt
	bic r11, r11, r10				@ unmask it
	str r11, [r12, #OMAP1510_GPIO_INT_MASK]

	@ Process keyboard data
	ldr r8, [r12, #OMAP1510_GPIO_DATA_INPUT]	@ fetch GPIO input

	ldr r10, [r9, #BUF_STATE]		@ fetch kbd interface state
	cmp r10, #0				@ are we expecting start bit?
	bne data				@ no - go to data processing

	ands r8, r8, #KEYBRD_DATA_MASK		@ check start bit - detected?
	beq hksw				@ no - try next source

	@ r8 contains KEYBRD_DATA_MASK, use it
	str r8, [r9, #BUF_STATE]		@ enter data processing state
	@ r10 already contains 0, reuse it
	str r10, [r9, #BUF_KEY]			@ clear keycode
	mov r10, #2				@ reset input bit mask
	str r10, [r9, #BUF_MASK]

	@ Mask other GPIO line interrupts till key done
	str r11, [r9, #BUF_GPIO_INT_MASK]	@ save mask for later restore
	mvn r11, #KEYBRD_CLK_MASK		@ prepare all except kbd mask
	str r11, [r12, #OMAP1510_GPIO_INT_MASK]	@ store into the mask register

	b restart				@ restart

data:	ldr r10, [r9, #BUF_MASK]		@ fetch current input bit mask

	@ r8 still contains GPIO input bits
	ands r8, r8, #KEYBRD_DATA_MASK		@ is keyboard data line low?
	ldreq r8, [r9, #BUF_KEY]		@ yes - fetch collected so far,
	orreq r8, r8, r10			@ set 1 at current mask position
	streq r8, [r9, #BUF_KEY]		@ and save back

	mov r10, r10, lsl #1			@ shift mask left
	bics r10, r10, #0x800			@ have we got all the bits?
	strne r10, [r9, #BUF_MASK]		@ not yet - store the mask
	bne restart				@ and restart

	@ r10 already contains 0, reuse it
	str r10, [r9, #BUF_STATE]		@ reset state to start

	@ Key done - restore interrupt mask
	ldr r10, [r9, #BUF_GPIO_INT_MASK]	@ fetch saved mask
	and r11, r11, r10			@ unmask all saved as unmasked
	str r11, [r12, #OMAP1510_GPIO_INT_MASK]	@ restore into the mask register

	@ Try appending the keycode to the circular buffer
	ldr r10, [r9, #BUF_KEYS_CNT]		@ get saved keystrokes count
	ldr r8, [r9, #BUF_BUF_LEN]		@ get buffer size
	cmp r10, r8				@ is buffer full?
	beq hksw				@ yes - key lost, next source

	add r10, r10, #1			@ incremet keystrokes counter
	str r10, [r9, #BUF_KEYS_CNT]

	ldr r10, [r9, #BUF_TAIL_OFFSET]		@ get buffer tail offset
	@ r8 already contains buffer size
	cmp r10, r8				@ end of buffer?
	moveq r10, #0				@ yes - rewind to buffer start

	ldr r12, [r9, #BUF_BUFFER_START]	@ get buffer start address
	add r12, r12, r10, LSL #2		@ calculate buffer tail address
	ldr r8, [r9, #BUF_KEY]			@ get last keycode
	str r8, [r12]				@ append it to the buffer tail

	add r10, r10, #1			@ increment buffer tail offset
	str r10, [r9, #BUF_TAIL_OFFSET]

	ldr r10, [r9, #BUF_CNT_INT_KEY]		@ increment interrupts counter
	add r10, r10, #1
	str r10, [r9, #BUF_CNT_INT_KEY]
	@@@@@@@@@@@@@@@@@@@@@@@@


hksw:	@Is hook switch interrupt requested?
	tst r13, #HOOK_SWITCH_MASK 		@ is hook switch status bit set?
	beq mdm					@ no - try next source


	@@@@@@@@@@@@@@@@@@@@@@@@
	@ Hook switch interrupt FIQ mode simple handler

	@ Don't toggle active edge, the switch always bounces

	@ Increment hook switch interrupt counter
	ldr r10, [r9, #BUF_CNT_INT_HSW]
	add r10, r10, #1
	str r10, [r9, #BUF_CNT_INT_HSW]
	@@@@@@@@@@@@@@@@@@@@@@@@


mdm:	@Is it a modem interrupt?
	tst r13, #MODEM_IRQ_MASK 		@ is modem status bit set?
	beq irq					@ no - check for next interrupt


	@@@@@@@@@@@@@@@@@@@@@@@@
	@ Modem FIQ mode interrupt handler stub

	@ Increment modem interrupt counter
	ldr r10, [r9, #BUF_CNT_INT_MDM]
	add r10, r10, #1
	str r10, [r9, #BUF_CNT_INT_MDM]
	@@@@@@@@@@@@@@@@@@@@@@@@


irq:	@ Place deferred_fiq interrupt request
	ldr r12, deferred_fiq_ih_base		@ set pointer to IRQ handler
	mov r10, #DEFERRED_FIQ_MASK		@ set deferred_fiq bit
	str r10, [r12, #IRQ_ISR_REG_OFFSET] 	@ place it in the ISR register

	ldr r12, omap1510_gpio_base		@ set pointer back to GPIO bank
	b restart				@ check for next GPIO interrupt
	@@@@@@@@@@@@@@@@@@@@@@@@@@@


/*
 * Virtual addresses for IO
 */
omap_ih1_base:
	.word OMAP1_IO_ADDRESS(OMAP_IH1_BASE)
deferred_fiq_ih_base:
	.word OMAP1_IO_ADDRESS(DEFERRED_FIQ_IH_BASE)
omap1510_gpio_base:
	.word OMAP1_IO_ADDRESS(OMAP1510_GPIO_BASE)
qwerty_fiqin_end:

/*
 * Check the size of the FIQ,
 * it cannot go beyond 0xffff0200, and is copied to 0xffff001c
 */
.if (qwerty_fiqin_end - qwerty_fiqin_start) > (0x200 - 0x1c)
	.err
.endif
