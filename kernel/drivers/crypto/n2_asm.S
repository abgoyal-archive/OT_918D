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

/* n2_asm.S: Hypervisor calls for NCS support.
 *
 * Copyright (C) 2009 David S. Miller <davem@davemloft.net>
 */

#include <linux/linkage.h>
#include <asm/hypervisor.h>
#include "n2_core.h"

	/* o0: queue type
	 * o1: RA of queue
	 * o2: num entries in queue
	 * o3: address of queue handle return
	 */
ENTRY(sun4v_ncs_qconf)
	mov	HV_FAST_NCS_QCONF, %o5
	ta	HV_FAST_TRAP
	stx	%o1, [%o3]
	retl
	 nop
ENDPROC(sun4v_ncs_qconf)

	/* %o0: queue handle
	 * %o1: address of queue type return
	 * %o2: address of queue base address return
	 * %o3: address of queue num entries return
	 */
ENTRY(sun4v_ncs_qinfo)
	mov	%o1, %g1
	mov	%o2, %g2
	mov	%o3, %g3
	mov	HV_FAST_NCS_QINFO, %o5
	ta	HV_FAST_TRAP
	stx	%o1, [%g1]
	stx	%o2, [%g2]
	stx	%o3, [%g3]
	retl
	 nop
ENDPROC(sun4v_ncs_qinfo)

	/* %o0: queue handle
	 * %o1: address of head offset return
	 */
ENTRY(sun4v_ncs_gethead)
	mov	%o1, %o2
	mov	HV_FAST_NCS_GETHEAD, %o5
	ta	HV_FAST_TRAP
	stx	%o1, [%o2]
	retl
	 nop
ENDPROC(sun4v_ncs_gethead)

	/* %o0: queue handle
	 * %o1: address of tail offset return
	 */
ENTRY(sun4v_ncs_gettail)
	mov	%o1, %o2
	mov	HV_FAST_NCS_GETTAIL, %o5
	ta	HV_FAST_TRAP
	stx	%o1, [%o2]
	retl
	 nop
ENDPROC(sun4v_ncs_gettail)

	/* %o0: queue handle
	 * %o1: new tail offset
	 */
ENTRY(sun4v_ncs_settail)
	mov	HV_FAST_NCS_SETTAIL, %o5
	ta	HV_FAST_TRAP
	retl
	 nop
ENDPROC(sun4v_ncs_settail)

	/* %o0: queue handle
	 * %o1: address of devino return
	 */
ENTRY(sun4v_ncs_qhandle_to_devino)
	mov	%o1, %o2
	mov	HV_FAST_NCS_QHANDLE_TO_DEVINO, %o5
	ta	HV_FAST_TRAP
	stx	%o1, [%o2]
	retl
	 nop
ENDPROC(sun4v_ncs_qhandle_to_devino)

	/* %o0: queue handle
	 * %o1: new head offset
	 */
ENTRY(sun4v_ncs_sethead_marker)
	mov	HV_FAST_NCS_SETHEAD_MARKER, %o5
	ta	HV_FAST_TRAP
	retl
	 nop
ENDPROC(sun4v_ncs_sethead_marker)
