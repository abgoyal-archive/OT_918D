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
 * This is a direct copy of the ev96100.h file, with a global
 * search and replace.  The numbers are the same.
 *
 * The reason I'm duplicating this is so that the 64120/96100
 * defines won't be confusing in the source code.
 */
#ifndef __ASM_MIPS_GT64120_H
#define __ASM_MIPS_GT64120_H

/*
 * This is the CPU physical memory map of PPMC Board:
 *
 *    0x00000000-0x03FFFFFF      - 64MB SDRAM (SCS[0]#)
 *    0x1C000000-0x1C000000      - LED (CS0)
 *    0x1C800000-0x1C800007      - UART 16550 port (CS1)
 *    0x1F000000-0x1F000000      - MailBox (CS3)
 *    0x1FC00000-0x20000000      - 4MB Flash (BOOT CS)
 */

#define WRPPMC_SDRAM_SCS0_BASE	0x00000000
#define WRPPMC_SDRAM_SCS0_SIZE	0x04000000

#define WRPPMC_UART16550_BASE	0x1C800000
#define WRPPMC_UART16550_CLOCK	3686400 /* 3.68MHZ */

#define WRPPMC_LED_BASE		0x1C000000
#define WRPPMC_MBOX_BASE	0x1F000000

#define WRPPMC_BOOTROM_BASE	0x1FC00000
#define WRPPMC_BOOTROM_SIZE	0x00400000 /* 4M Flash */

#define WRPPMC_MIPS_TIMER_IRQ	7 /* MIPS compare/count timer interrupt */
#define WRPPMC_UART16550_IRQ	6
#define WRPPMC_PCI_INTA_IRQ	3

/*
 * PCI Bus I/O and Memory resources allocation
 *
 * NOTE: We only have PCI_0 hose interface
 */
#define GT_PCI_MEM_BASE	0x13000000UL
#define GT_PCI_MEM_SIZE	0x02000000UL
#define GT_PCI_IO_BASE	0x11000000UL
#define GT_PCI_IO_SIZE	0x02000000UL

/*
 * PCI interrupts will come in on either the INTA or INTD interrupt lines,
 * which are mapped to the #2 and #5 interrupt pins of the MIPS.  On our
 * boards, they all either come in on IntD or they all come in on IntA, they
 * aren't mixed. There can be numerous PCI interrupts, so we keep a list of the
 * "requested" interrupt numbers and go through the list whenever we get an
 * IntA/D.
 *
 * Interrupts < 8 are directly wired to the processor; PCI INTA is 8 and
 * INTD is 11.
 */
#define GT_TIMER	4
#define GT_INTA		2
#define GT_INTD		5

#ifndef __ASSEMBLY__

/*
 * GT64120 internal register space base address
 */
extern unsigned long gt64120_base;

#define GT64120_BASE	(gt64120_base)

/* define WRPPMC_EARLY_DEBUG to enable early output something to UART */
#undef WRPPMC_EARLY_DEBUG

#ifdef WRPPMC_EARLY_DEBUG
extern void wrppmc_led_on(int mask);
extern void wrppmc_led_off(int mask);
extern void wrppmc_early_printk(const char *fmt, ...);
#else
#define wrppmc_early_printk(fmt, ...) do {} while (0)
#endif /* WRPPMC_EARLY_DEBUG */

#endif /* __ASSEMBLY__ */
#endif /* __ASM_MIPS_GT64120_H */
