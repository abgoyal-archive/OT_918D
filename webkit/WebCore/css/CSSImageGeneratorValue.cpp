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
 * Copyright (C) 2008 Apple Inc.  All rights reserved.
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
#include "CSSImageGeneratorValue.h"

#include "Image.h"
#include "IntSize.h"
#include "IntSizeHash.h"
#include "PlatformString.h"
#include "RenderObject.h"
#include "StyleGeneratedImage.h"

namespace WebCore {

CSSImageGeneratorValue::CSSImageGeneratorValue()
: m_accessedImage(false)
{
}

CSSImageGeneratorValue::~CSSImageGeneratorValue()
{
}

void CSSImageGeneratorValue::addClient(RenderObject* renderer, const IntSize& size)
{
    ref();
    if (!size.isEmpty())
        m_sizes.add(size);
    
    RenderObjectSizeCountMap::iterator it = m_clients.find(renderer);
    if (it == m_clients.end())
        m_clients.add(renderer, SizeCountPair(size, 1));
    else {
        SizeCountPair& sizeCount = it->second;
        ++sizeCount.second;
    }
}

void CSSImageGeneratorValue::removeClient(RenderObject* renderer)
{
    RenderObjectSizeCountMap::iterator it = m_clients.find(renderer);
    ASSERT(it != m_clients.end());

    SizeCountPair& sizeCount = it->second;
    IntSize size = sizeCount.first;
    if (!size.isEmpty()) {
        m_sizes.remove(size);
        if (!m_sizes.contains(size))
            m_images.remove(size);
    }
    
    if (!--sizeCount.second)
        m_clients.remove(renderer);

    deref();
}

Image* CSSImageGeneratorValue::getImage(RenderObject* renderer, const IntSize& size)
{
    RenderObjectSizeCountMap::iterator it = m_clients.find(renderer);
    ASSERT(it != m_clients.end());

    SizeCountPair& sizeCount = it->second;
    IntSize oldSize = sizeCount.first;
    if (oldSize != size) {
        removeClient(renderer);
        addClient(renderer, size);
    }
    
    // Don't generate an image for empty sizes.
    if (size.isEmpty())
        return 0;
    
    // Look up the image in our cache.
    return m_images.get(size).get();
}

void CSSImageGeneratorValue::putImage(const IntSize& size, PassRefPtr<Image> image)
{
    m_images.add(size, image);
}

StyleGeneratedImage* CSSImageGeneratorValue::generatedImage()
{
    if (!m_accessedImage) {
        m_accessedImage = true;
        m_image = StyleGeneratedImage::create(this, isFixedSize());
    }
    return m_image.get();
}

} // namespace WebCore
