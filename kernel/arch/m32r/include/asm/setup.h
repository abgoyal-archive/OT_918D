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

#ifndef _ASM_M32R_SETUP_H
#define _ASM_M32R_SETUP_H

/*
 * This is set up by the setup-routine at boot-time
 */

#define COMMAND_LINE_SIZE       512

#ifdef __KERNEL__

#define PARAM			((unsigned char *)empty_zero_page)

#define MOUNT_ROOT_RDONLY	(*(unsigned long *) (PARAM+0x000))
#define RAMDISK_FLAGS		(*(unsigned long *) (PARAM+0x004))
#define ORIG_ROOT_DEV		(*(unsigned long *) (PARAM+0x008))
#define LOADER_TYPE		(*(unsigned long *) (PARAM+0x00c))
#define INITRD_START		(*(unsigned long *) (PARAM+0x010))
#define INITRD_SIZE		(*(unsigned long *) (PARAM+0x014))

#define M32R_CPUCLK		(*(unsigned long *) (PARAM+0x018))
#define M32R_BUSCLK		(*(unsigned long *) (PARAM+0x01c))
#define M32R_TIMER_DIVIDE	(*(unsigned long *) (PARAM+0x020))

#define COMMAND_LINE		((char *) (PARAM+0x100))

#define SCREEN_INFO		(*(struct screen_info *) (PARAM+0x200))

#define RAMDISK_IMAGE_START_MASK	(0x07FF)
#define RAMDISK_PROMPT_FLAG		(0x8000)
#define RAMDISK_LOAD_FLAG		(0x4000)

extern unsigned long memory_start;
extern unsigned long memory_end;

#endif  /*  __KERNEL__  */

#endif /* _ASM_M32R_SETUP_H */
