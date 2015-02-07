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
 *   Data definitions for channel report processing
 *    Copyright IBM Corp. 2000,2009
 *    Author(s): Ingo Adlung <adlung@de.ibm.com>,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Cornelia Huck <cornelia.huck@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
 */

#ifndef _ASM_S390_CRW_H
#define _ASM_S390_CRW_H

#include <linux/types.h>

/*
 * Channel Report Word
 */
struct crw {
	__u32 res1 :  1;   /* reserved zero */
	__u32 slct :  1;   /* solicited */
	__u32 oflw :  1;   /* overflow */
	__u32 chn  :  1;   /* chained */
	__u32 rsc  :  4;   /* reporting source code */
	__u32 anc  :  1;   /* ancillary report */
	__u32 res2 :  1;   /* reserved zero */
	__u32 erc  :  6;   /* error-recovery code */
	__u32 rsid : 16;   /* reporting-source ID */
} __attribute__ ((packed));

typedef void (*crw_handler_t)(struct crw *, struct crw *, int);

extern int crw_register_handler(int rsc, crw_handler_t handler);
extern void crw_unregister_handler(int rsc);
extern void crw_handle_channel_report(void);
void crw_wait_for_channel_report(void);

#define NR_RSCS 16

#define CRW_RSC_MONITOR  0x2  /* monitoring facility */
#define CRW_RSC_SCH	 0x3  /* subchannel */
#define CRW_RSC_CPATH	 0x4  /* channel path */
#define CRW_RSC_CONFIG	 0x9  /* configuration-alert facility */
#define CRW_RSC_CSS	 0xB  /* channel subsystem */

#define CRW_ERC_EVENT	 0x00 /* event information pending */
#define CRW_ERC_AVAIL	 0x01 /* available */
#define CRW_ERC_INIT	 0x02 /* initialized */
#define CRW_ERC_TERROR	 0x03 /* temporary error */
#define CRW_ERC_IPARM	 0x04 /* installed parm initialized */
#define CRW_ERC_TERM	 0x05 /* terminal */
#define CRW_ERC_PERRN	 0x06 /* perm. error, fac. not init */
#define CRW_ERC_PERRI	 0x07 /* perm. error, facility init */
#define CRW_ERC_PMOD	 0x08 /* installed parameters modified */

static inline int stcrw(struct crw *pcrw)
{
	int ccode;

	asm volatile(
		"	stcrw	0(%2)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (ccode), "=m" (*pcrw)
		: "a" (pcrw)
		: "cc" );
	return ccode;
}

#endif /* _ASM_S390_CRW_H */
