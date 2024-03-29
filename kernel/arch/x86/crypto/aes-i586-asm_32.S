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

// -------------------------------------------------------------------------
// Copyright (c) 2001, Dr Brian Gladman <                 >, Worcester, UK.
// All rights reserved.
//
// LICENSE TERMS
//
// The free distribution and use of this software in both source and binary 
// form is allowed (with or without changes) provided that:
//
//   1. distributions of this source code include the above copyright 
//      notice, this list of conditions and the following disclaimer//
//
//   2. distributions in binary form include the above copyright
//      notice, this list of conditions and the following disclaimer
//      in the documentation and/or other associated materials//
//
//   3. the copyright holder's name is not used to endorse products 
//      built using this software without specific written permission.
//
//
// ALTERNATIVELY, provided that this notice is retained in full, this product
// may be distributed under the terms of the GNU General Public License (GPL),
// in which case the provisions of the GPL apply INSTEAD OF those given above.
//
// Copyright (c) 2004 Linus Torvalds <torvalds@osdl.org>
// Copyright (c) 2004 Red Hat, Inc., James Morris <jmorris@redhat.com>

// DISCLAIMER
//
// This software is provided 'as is' with no explicit or implied warranties
// in respect of its properties including, but not limited to, correctness 
// and fitness for purpose.
// -------------------------------------------------------------------------
// Issue Date: 29/07/2002

.file "aes-i586-asm.S"
.text

#include <asm/asm-offsets.h>

#define tlen 1024   // length of each of 4 'xor' arrays (256 32-bit words)

/* offsets to parameters with one register pushed onto stack */
#define ctx 8
#define out_blk 12
#define in_blk 16

/* offsets in crypto_aes_ctx structure */
#define klen (480)
#define ekey (0)
#define dkey (240)

// register mapping for encrypt and decrypt subroutines

#define r0  eax
#define r1  ebx
#define r2  ecx
#define r3  edx
#define r4  esi
#define r5  edi

#define eaxl  al
#define eaxh  ah
#define ebxl  bl
#define ebxh  bh
#define ecxl  cl
#define ecxh  ch
#define edxl  dl
#define edxh  dh

#define _h(reg) reg##h
#define h(reg) _h(reg)

#define _l(reg) reg##l
#define l(reg) _l(reg)

// This macro takes a 32-bit word representing a column and uses
// each of its four bytes to index into four tables of 256 32-bit
// words to obtain values that are then xored into the appropriate
// output registers r0, r1, r4 or r5.  

// Parameters:
// table table base address
//   %1  out_state[0]
//   %2  out_state[1]
//   %3  out_state[2]
//   %4  out_state[3]
//   idx input register for the round (destroyed)
//   tmp scratch register for the round
// sched key schedule

#define do_col(table, a1,a2,a3,a4, idx, tmp)	\
	movzx   %l(idx),%tmp;			\
	xor     table(,%tmp,4),%a1;		\
	movzx   %h(idx),%tmp;			\
	shr     $16,%idx;			\
	xor     table+tlen(,%tmp,4),%a2;	\
	movzx   %l(idx),%tmp;			\
	movzx   %h(idx),%idx;			\
	xor     table+2*tlen(,%tmp,4),%a3;	\
	xor     table+3*tlen(,%idx,4),%a4;

// initialise output registers from the key schedule
// NB1: original value of a3 is in idx on exit
// NB2: original values of a1,a2,a4 aren't used
#define do_fcol(table, a1,a2,a3,a4, idx, tmp, sched) \
	mov     0 sched,%a1;			\
	movzx   %l(idx),%tmp;			\
	mov     12 sched,%a2;			\
	xor     table(,%tmp,4),%a1;		\
	mov     4 sched,%a4;			\
	movzx   %h(idx),%tmp;			\
	shr     $16,%idx;			\
	xor     table+tlen(,%tmp,4),%a2;	\
	movzx   %l(idx),%tmp;			\
	movzx   %h(idx),%idx;			\
	xor     table+3*tlen(,%idx,4),%a4;	\
	mov     %a3,%idx;			\
	mov     8 sched,%a3;			\
	xor     table+2*tlen(,%tmp,4),%a3;

// initialise output registers from the key schedule
// NB1: original value of a3 is in idx on exit
// NB2: original values of a1,a2,a4 aren't used
#define do_icol(table, a1,a2,a3,a4, idx, tmp, sched) \
	mov     0 sched,%a1;			\
	movzx   %l(idx),%tmp;			\
	mov     4 sched,%a2;			\
	xor     table(,%tmp,4),%a1;		\
	mov     12 sched,%a4;			\
	movzx   %h(idx),%tmp;			\
	shr     $16,%idx;			\
	xor     table+tlen(,%tmp,4),%a2;	\
	movzx   %l(idx),%tmp;			\
	movzx   %h(idx),%idx;			\
	xor     table+3*tlen(,%idx,4),%a4;	\
	mov     %a3,%idx;			\
	mov     8 sched,%a3;			\
	xor     table+2*tlen(,%tmp,4),%a3;


// original Gladman had conditional saves to MMX regs.
#define save(a1, a2)		\
	mov     %a2,4*a1(%esp)

#define restore(a1, a2)		\
	mov     4*a2(%esp),%a1

// These macros perform a forward encryption cycle. They are entered with
// the first previous round column values in r0,r1,r4,r5 and
// exit with the final values in the same registers, using stack
// for temporary storage.

// round column values
// on entry: r0,r1,r4,r5
// on exit:  r2,r1,r4,r5
#define fwd_rnd1(arg, table)						\
	save   (0,r1);							\
	save   (1,r5);							\
									\
	/* compute new column values */					\
	do_fcol(table, r2,r5,r4,r1, r0,r3, arg);	/* idx=r0 */	\
	do_col (table, r4,r1,r2,r5, r0,r3);		/* idx=r4 */	\
	restore(r0,0);							\
	do_col (table, r1,r2,r5,r4, r0,r3);		/* idx=r1 */	\
	restore(r0,1);							\
	do_col (table, r5,r4,r1,r2, r0,r3);		/* idx=r5 */

// round column values
// on entry: r2,r1,r4,r5
// on exit:  r0,r1,r4,r5
#define fwd_rnd2(arg, table)						\
	save   (0,r1);							\
	save   (1,r5);							\
									\
	/* compute new column values */					\
	do_fcol(table, r0,r5,r4,r1, r2,r3, arg);	/* idx=r2 */	\
	do_col (table, r4,r1,r0,r5, r2,r3);		/* idx=r4 */	\
	restore(r2,0);							\
	do_col (table, r1,r0,r5,r4, r2,r3);		/* idx=r1 */	\
	restore(r2,1);							\
	do_col (table, r5,r4,r1,r0, r2,r3);		/* idx=r5 */

// These macros performs an inverse encryption cycle. They are entered with
// the first previous round column values in r0,r1,r4,r5 and
// exit with the final values in the same registers, using stack
// for temporary storage

// round column values
// on entry: r0,r1,r4,r5
// on exit:  r2,r1,r4,r5
#define inv_rnd1(arg, table)						\
	save    (0,r1);							\
	save    (1,r5);							\
									\
	/* compute new column values */					\
	do_icol(table, r2,r1,r4,r5, r0,r3, arg);	/* idx=r0 */	\
	do_col (table, r4,r5,r2,r1, r0,r3);		/* idx=r4 */	\
	restore(r0,0);							\
	do_col (table, r1,r4,r5,r2, r0,r3);		/* idx=r1 */	\
	restore(r0,1);							\
	do_col (table, r5,r2,r1,r4, r0,r3);		/* idx=r5 */

// round column values
// on entry: r2,r1,r4,r5
// on exit:  r0,r1,r4,r5
#define inv_rnd2(arg, table)						\
	save    (0,r1);							\
	save    (1,r5);							\
									\
	/* compute new column values */					\
	do_icol(table, r0,r1,r4,r5, r2,r3, arg);	/* idx=r2 */	\
	do_col (table, r4,r5,r0,r1, r2,r3);		/* idx=r4 */	\
	restore(r2,0);							\
	do_col (table, r1,r4,r5,r0, r2,r3);		/* idx=r1 */	\
	restore(r2,1);							\
	do_col (table, r5,r0,r1,r4, r2,r3);		/* idx=r5 */

// AES (Rijndael) Encryption Subroutine
/* void aes_enc_blk(struct crypto_aes_ctx *ctx, u8 *out_blk, const u8 *in_blk) */

.global  aes_enc_blk

.extern  crypto_ft_tab
.extern  crypto_fl_tab

.align 4

aes_enc_blk:
	push    %ebp
	mov     ctx(%esp),%ebp

// CAUTION: the order and the values used in these assigns 
// rely on the register mappings

1:	push    %ebx
	mov     in_blk+4(%esp),%r2
	push    %esi
	mov     klen(%ebp),%r3   // key size
	push    %edi
#if ekey != 0
	lea     ekey(%ebp),%ebp  // key pointer
#endif

// input four columns and xor in first round key

	mov     (%r2),%r0
	mov     4(%r2),%r1
	mov     8(%r2),%r4
	mov     12(%r2),%r5
	xor     (%ebp),%r0
	xor     4(%ebp),%r1
	xor     8(%ebp),%r4
	xor     12(%ebp),%r5

	sub     $8,%esp		// space for register saves on stack
	add     $16,%ebp	// increment to next round key
	cmp     $24,%r3
	jb      4f		// 10 rounds for 128-bit key
	lea     32(%ebp),%ebp
	je      3f		// 12 rounds for 192-bit key
	lea     32(%ebp),%ebp

2:	fwd_rnd1( -64(%ebp), crypto_ft_tab)	// 14 rounds for 256-bit key
	fwd_rnd2( -48(%ebp), crypto_ft_tab)
3:	fwd_rnd1( -32(%ebp), crypto_ft_tab)	// 12 rounds for 192-bit key
	fwd_rnd2( -16(%ebp), crypto_ft_tab)
4:	fwd_rnd1(    (%ebp), crypto_ft_tab)	// 10 rounds for 128-bit key
	fwd_rnd2( +16(%ebp), crypto_ft_tab)
	fwd_rnd1( +32(%ebp), crypto_ft_tab)
	fwd_rnd2( +48(%ebp), crypto_ft_tab)
	fwd_rnd1( +64(%ebp), crypto_ft_tab)
	fwd_rnd2( +80(%ebp), crypto_ft_tab)
	fwd_rnd1( +96(%ebp), crypto_ft_tab)
	fwd_rnd2(+112(%ebp), crypto_ft_tab)
	fwd_rnd1(+128(%ebp), crypto_ft_tab)
	fwd_rnd2(+144(%ebp), crypto_fl_tab)	// last round uses a different table

// move final values to the output array.  CAUTION: the 
// order of these assigns rely on the register mappings

	add     $8,%esp
	mov     out_blk+12(%esp),%ebp
	mov     %r5,12(%ebp)
	pop     %edi
	mov     %r4,8(%ebp)
	pop     %esi
	mov     %r1,4(%ebp)
	pop     %ebx
	mov     %r0,(%ebp)
	pop     %ebp
	ret

// AES (Rijndael) Decryption Subroutine
/* void aes_dec_blk(struct crypto_aes_ctx *ctx, u8 *out_blk, const u8 *in_blk) */

.global  aes_dec_blk

.extern  crypto_it_tab
.extern  crypto_il_tab

.align 4

aes_dec_blk:
	push    %ebp
	mov     ctx(%esp),%ebp

// CAUTION: the order and the values used in these assigns 
// rely on the register mappings

1:	push    %ebx
	mov     in_blk+4(%esp),%r2
	push    %esi
	mov     klen(%ebp),%r3   // key size
	push    %edi
#if dkey != 0
	lea     dkey(%ebp),%ebp  // key pointer
#endif
	
// input four columns and xor in first round key

	mov     (%r2),%r0
	mov     4(%r2),%r1
	mov     8(%r2),%r4
	mov     12(%r2),%r5
	xor     (%ebp),%r0
	xor     4(%ebp),%r1
	xor     8(%ebp),%r4
	xor     12(%ebp),%r5

	sub     $8,%esp		// space for register saves on stack
	add     $16,%ebp	// increment to next round key
	cmp     $24,%r3
	jb      4f		// 10 rounds for 128-bit key
	lea     32(%ebp),%ebp
	je      3f		// 12 rounds for 192-bit key
	lea     32(%ebp),%ebp

2:	inv_rnd1( -64(%ebp), crypto_it_tab)	// 14 rounds for 256-bit key
	inv_rnd2( -48(%ebp), crypto_it_tab)
3:	inv_rnd1( -32(%ebp), crypto_it_tab)	// 12 rounds for 192-bit key
	inv_rnd2( -16(%ebp), crypto_it_tab)
4:	inv_rnd1(    (%ebp), crypto_it_tab)	// 10 rounds for 128-bit key
	inv_rnd2( +16(%ebp), crypto_it_tab)
	inv_rnd1( +32(%ebp), crypto_it_tab)
	inv_rnd2( +48(%ebp), crypto_it_tab)
	inv_rnd1( +64(%ebp), crypto_it_tab)
	inv_rnd2( +80(%ebp), crypto_it_tab)
	inv_rnd1( +96(%ebp), crypto_it_tab)
	inv_rnd2(+112(%ebp), crypto_it_tab)
	inv_rnd1(+128(%ebp), crypto_it_tab)
	inv_rnd2(+144(%ebp), crypto_il_tab)	// last round uses a different table

// move final values to the output array.  CAUTION: the 
// order of these assigns rely on the register mappings

	add     $8,%esp
	mov     out_blk+12(%esp),%ebp
	mov     %r5,12(%ebp)
	pop     %edi
	mov     %r4,8(%ebp)
	pop     %esi
	mov     %r1,4(%ebp)
	pop     %ebx
	mov     %r0,(%ebp)
	pop     %ebp
	ret
