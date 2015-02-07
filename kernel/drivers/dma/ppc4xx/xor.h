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
 * 440SPe's XOR engines support header file
 *
 * 2006-2009 (C) DENX Software Engineering.
 *
 * Author: Yuri Tikhonov <yur@emcraft.com>
 *
 * This file is licensed under the term of  the GNU General Public License
 * version 2. The program licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef _PPC440SPE_XOR_H
#define _PPC440SPE_XOR_H

#include <linux/types.h>

/* Number of XOR engines available on the contoller */
#define XOR_ENGINES_NUM		1

/* Number of operands supported in the h/w */
#define XOR_MAX_OPS		16

/*
 * XOR Command Block Control Register bits
 */
#define XOR_CBCR_LNK_BIT        (1<<31) /* link present */
#define XOR_CBCR_TGT_BIT        (1<<30) /* target present */
#define XOR_CBCR_CBCE_BIT       (1<<29) /* command block compete enable */
#define XOR_CBCR_RNZE_BIT       (1<<28) /* result not zero enable */
#define XOR_CBCR_XNOR_BIT       (1<<15) /* XOR/XNOR */
#define XOR_CDCR_OAC_MSK        (0x7F)  /* operand address count */

/*
 * XORCore Status Register bits
 */
#define XOR_SR_XCP_BIT		(1<<31)	/* core processing */
#define XOR_SR_ICB_BIT		(1<<17)	/* invalid CB */
#define XOR_SR_IC_BIT		(1<<16)	/* invalid command */
#define XOR_SR_IPE_BIT		(1<<15)	/* internal parity error */
#define XOR_SR_RNZ_BIT		(1<<2)	/* result not Zero */
#define XOR_SR_CBC_BIT		(1<<1)	/* CB complete */
#define XOR_SR_CBLC_BIT		(1<<0)	/* CB list complete */

/*
 * XORCore Control Set and Reset Register bits
 */
#define XOR_CRSR_XASR_BIT	(1<<31)	/* soft reset */
#define XOR_CRSR_XAE_BIT	(1<<30)	/* enable */
#define XOR_CRSR_RCBE_BIT	(1<<29)	/* refetch CB enable */
#define XOR_CRSR_PAUS_BIT	(1<<28)	/* pause */
#define XOR_CRSR_64BA_BIT	(1<<27) /* 64/32 CB format */
#define XOR_CRSR_CLP_BIT	(1<<25)	/* continue list processing */

/*
 * XORCore Interrupt Enable Register
 */
#define XOR_IE_ICBIE_BIT	(1<<17)	/* Invalid Command Block IRQ Enable */
#define XOR_IE_ICIE_BIT		(1<<16)	/* Invalid Command IRQ Enable */
#define XOR_IE_RPTIE_BIT	(1<<14)	/* Read PLB Timeout Error IRQ Enable */
#define XOR_IE_CBCIE_BIT	(1<<1)	/* CB complete interrupt enable */
#define XOR_IE_CBLCI_BIT	(1<<0)	/* CB list complete interrupt enable */

/*
 * XOR Accelerator engine Command Block Type
 */
struct xor_cb {
	/*
	 * Basic 64-bit format XOR CB (Table 19-1, p.463, 440spe_um_1_22.pdf)
	 */
	u32	cbc;		/* control */
	u32	cbbc;		/* byte count */
	u32	cbs;		/* status */
	u8	pad0[4];	/* reserved */
	u32	cbtah;		/* target address high */
	u32	cbtal;		/* target address low */
	u32	cblah;		/* link address high */
	u32	cblal;		/* link address low */
	struct {
		u32 h;
		u32 l;
	} __attribute__ ((packed)) ops[16];
} __attribute__ ((packed));

/*
 * XOR hardware registers Table 19-3, UM 1.22
 */
struct xor_regs {
	u32	op_ar[16][2];	/* operand address[0]-high,[1]-low registers */
	u8	pad0[352];	/* reserved */
	u32	cbcr;		/* CB control register */
	u32	cbbcr;		/* CB byte count register */
	u32	cbsr;		/* CB status register */
	u8	pad1[4];	/* reserved */
	u32	cbtahr;		/* operand target address high register */
	u32	cbtalr;		/* operand target address low register */
	u32	cblahr;		/* CB link address high register */
	u32	cblalr;		/* CB link address low register */
	u32	crsr;		/* control set register */
	u32	crrr;		/* control reset register */
	u32	ccbahr;		/* current CB address high register */
	u32	ccbalr;		/* current CB address low register */
	u32	plbr;		/* PLB configuration register */
	u32	ier;		/* interrupt enable register */
	u32	pecr;		/* parity error count register */
	u32	sr;		/* status register */
	u32	revidr;		/* revision ID register */
};

#endif /* _PPC440SPE_XOR_H */
