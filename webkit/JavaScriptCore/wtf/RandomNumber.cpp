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
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
 *           (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "RandomNumber.h"

#include "RandomNumberSeed.h"

#include <limits>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#if OS(WINCE)
extern "C" {
#include "wince/mt19937ar.c"
}
#endif

#if PLATFORM(BREWMP)
#include <AEEAppGen.h>
#include <AEESource.h>
#include <AEEStdLib.h>
#endif

namespace WTF {

double weakRandomNumber()
{
#if COMPILER(MSVC) && defined(_CRT_RAND_S)
    // rand_s is incredibly slow on windows so we fall back on rand for Math.random
    return (rand() + (rand() / (RAND_MAX + 1.0))) / (RAND_MAX + 1.0);
#elif PLATFORM(BREWMP)
    uint32_t bits;
    GETRAND(reinterpret_cast<byte*>(&bits), sizeof(uint32_t));
    return static_cast<double>(bits) / (static_cast<double>(std::numeric_limits<uint32_t>::max()) + 1.0);
#else
    return randomNumber();
#endif
}

double randomNumber()
{
#if !ENABLE(JSC_MULTIPLE_THREADS)
    static bool s_initialized = false;
    if (!s_initialized) {
        initializeRandomNumberGenerator();
        s_initialized = true;
    }
#endif
    
#if COMPILER(MSVC) && defined(_CRT_RAND_S)
    uint32_t bits;
    rand_s(&bits);
    return static_cast<double>(bits) / (static_cast<double>(std::numeric_limits<uint32_t>::max()) + 1.0);
#elif OS(DARWIN)
    uint32_t bits = arc4random();
    return static_cast<double>(bits) / (static_cast<double>(std::numeric_limits<uint32_t>::max()) + 1.0);
#elif OS(UNIX)
    uint32_t part1 = random() & (RAND_MAX - 1);
    uint32_t part2 = random() & (RAND_MAX - 1);
    // random only provides 31 bits
    uint64_t fullRandom = part1;
    fullRandom <<= 31;
    fullRandom |= part2;

    // Mask off the low 53bits
    fullRandom &= (1LL << 53) - 1;
    return static_cast<double>(fullRandom)/static_cast<double>(1LL << 53);
#elif OS(WINCE)
    return genrand_res53();
#elif OS(WINDOWS)
    uint32_t part1 = rand() & (RAND_MAX - 1);
    uint32_t part2 = rand() & (RAND_MAX - 1);
    uint32_t part3 = rand() & (RAND_MAX - 1);
    uint32_t part4 = rand() & (RAND_MAX - 1);
    // rand only provides 15 bits on Win32
    uint64_t fullRandom = part1;
    fullRandom <<= 15;
    fullRandom |= part2;
    fullRandom <<= 15;
    fullRandom |= part3;
    fullRandom <<= 15;
    fullRandom |= part4;

    // Mask off the low 53bits
    fullRandom &= (1LL << 53) - 1;
    return static_cast<double>(fullRandom)/static_cast<double>(1LL << 53);
#elif PLATFORM(BREWMP)
    uint32_t bits;
    ISource* randomSource;

    IShell* shell = reinterpret_cast<AEEApplet*>(GETAPPINSTANCE())->m_pIShell;
    ISHELL_CreateInstance(shell, AEECLSID_RANDOM, reinterpret_cast<void**>(&randomSource));
    ISOURCE_Read(randomSource, reinterpret_cast<char*>(&bits), 4);
    ISOURCE_Release(randomSource);

    return static_cast<double>(bits) / (static_cast<double>(std::numeric_limits<uint32_t>::max()) + 1.0);
#else
    uint32_t part1 = rand() & (RAND_MAX - 1);
    uint32_t part2 = rand() & (RAND_MAX - 1);
    // rand only provides 31 bits, and the low order bits of that aren't very random
    // so we take the high 26 bits of part 1, and the high 27 bits of part2.
    part1 >>= 5; // drop the low 5 bits
    part2 >>= 4; // drop the low 4 bits
    uint64_t fullRandom = part1;
    fullRandom <<= 27;
    fullRandom |= part2;

    // Mask off the low 53bits
    fullRandom &= (1LL << 53) - 1;
    return static_cast<double>(fullRandom)/static_cast<double>(1LL << 53);
#endif
}

}
