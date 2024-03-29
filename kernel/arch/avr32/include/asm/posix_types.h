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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_AVR32_POSIX_TYPES_H
#define __ASM_AVR32_POSIX_TYPES_H

/*
 * This file is generally used by user-level software, so you need to
 * be a little careful about namespace pollution etc.  Also, we cannot
 * assume GCC is being used.
 */

typedef unsigned long   __kernel_ino_t;
typedef unsigned short  __kernel_mode_t;
typedef unsigned short  __kernel_nlink_t;
typedef long            __kernel_off_t;
typedef int             __kernel_pid_t;
typedef unsigned short  __kernel_ipc_pid_t;
typedef unsigned int	__kernel_uid_t;
typedef unsigned int	__kernel_gid_t;
typedef unsigned long	__kernel_size_t;
typedef long		__kernel_ssize_t;
typedef int             __kernel_ptrdiff_t;
typedef long            __kernel_time_t;
typedef long            __kernel_suseconds_t;
typedef long            __kernel_clock_t;
typedef int             __kernel_timer_t;
typedef int             __kernel_clockid_t;
typedef int             __kernel_daddr_t;
typedef char *          __kernel_caddr_t;
typedef unsigned short  __kernel_uid16_t;
typedef unsigned short  __kernel_gid16_t;
typedef unsigned int    __kernel_uid32_t;
typedef unsigned int    __kernel_gid32_t;

typedef unsigned short  __kernel_old_uid_t;
typedef unsigned short  __kernel_old_gid_t;
typedef unsigned short  __kernel_old_dev_t;

#ifdef __GNUC__
typedef long long       __kernel_loff_t;
#endif

typedef struct {
    int     val[2];
} __kernel_fsid_t;

#if defined(__KERNEL__)

#undef  __FD_SET
static __inline__ void __FD_SET(unsigned long __fd, __kernel_fd_set *__fdsetp)
{
    unsigned long __tmp = __fd / __NFDBITS;
    unsigned long __rem = __fd % __NFDBITS;
    __fdsetp->fds_bits[__tmp] |= (1UL<<__rem);
}

#undef  __FD_CLR
static __inline__ void __FD_CLR(unsigned long __fd, __kernel_fd_set *__fdsetp)
{
    unsigned long __tmp = __fd / __NFDBITS;
    unsigned long __rem = __fd % __NFDBITS;
    __fdsetp->fds_bits[__tmp] &= ~(1UL<<__rem);
}


#undef  __FD_ISSET
static __inline__ int __FD_ISSET(unsigned long __fd, const __kernel_fd_set *__p)
{
    unsigned long __tmp = __fd / __NFDBITS;
    unsigned long __rem = __fd % __NFDBITS;
    return (__p->fds_bits[__tmp] & (1UL<<__rem)) != 0;
}

/*
 * This will unroll the loop for the normal constant case (8 ints,
 * for a 256-bit fd_set)
 */
#undef  __FD_ZERO
static __inline__ void __FD_ZERO(__kernel_fd_set *__p)
{
    unsigned long *__tmp = __p->fds_bits;
    int __i;

    if (__builtin_constant_p(__FDSET_LONGS)) {
        switch (__FDSET_LONGS) {
            case 16:
                __tmp[ 0] = 0; __tmp[ 1] = 0;
                __tmp[ 2] = 0; __tmp[ 3] = 0;
                __tmp[ 4] = 0; __tmp[ 5] = 0;
                __tmp[ 6] = 0; __tmp[ 7] = 0;
                __tmp[ 8] = 0; __tmp[ 9] = 0;
                __tmp[10] = 0; __tmp[11] = 0;
                __tmp[12] = 0; __tmp[13] = 0;
                __tmp[14] = 0; __tmp[15] = 0;
                return;

            case 8:
                __tmp[ 0] = 0; __tmp[ 1] = 0;
                __tmp[ 2] = 0; __tmp[ 3] = 0;
                __tmp[ 4] = 0; __tmp[ 5] = 0;
                __tmp[ 6] = 0; __tmp[ 7] = 0;
                return;

            case 4:
                __tmp[ 0] = 0; __tmp[ 1] = 0;
                __tmp[ 2] = 0; __tmp[ 3] = 0;
                return;
        }
    }
    __i = __FDSET_LONGS;
    while (__i) {
        __i--;
        *__tmp = 0;
        __tmp++;
    }
}

#endif /* defined(__KERNEL__) */

#endif /* __ASM_AVR32_POSIX_TYPES_H */
