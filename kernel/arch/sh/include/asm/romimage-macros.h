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

#ifndef __ROMIMAGE_MACRO_H
#define __ROMIMAGE_MACRO_H

/* The LIST command is used to include comments in the script */
.macro	LIST comment
.endm

/* The ED command is used to write a 32-bit word */
.macro  ED, addr, data
	mov.l 1f, r1
	mov.l 2f, r0
	mov.l r0, @r1
	bra 3f
	 nop
	.align 2
1 :	.long \addr
2 :	.long \data
3 :
.endm

/* The EW command is used to write a 16-bit word */
.macro  EW, addr, data
	mov.l 1f, r1
	mov.l 2f, r0
	mov.w r0, @r1
	bra 3f
	 nop
	.align 2
1 :	.long \addr
2 :	.long \data
3 :
.endm

/* The EB command is used to write an 8-bit word */
.macro  EB, addr, data
	mov.l 1f, r1
	mov.l 2f, r0
	mov.b r0, @r1
	bra 3f
	 nop
	.align 2
1 :	.long \addr
2 :	.long \data
3 :
.endm

/* The WAIT command is used to delay the execution */
.macro  WAIT, time
	mov.l  2f, r3
1 :
	nop
	tst     r3, r3
	bf/s    1b
	dt      r3
	bra	3f
	 nop
	.align 2
2 :	.long \time * 100
3 :
.endm

/* The DD command is used to read a 32-bit word */
.macro  DD, addr, addr2, nr
	mov.l 1f, r1
	mov.l @r1, r0
	bra 2f
	 nop
	.align 2
1 :	.long \addr
2 :
.endm

#endif /* __ROMIMAGE_MACRO_H */
