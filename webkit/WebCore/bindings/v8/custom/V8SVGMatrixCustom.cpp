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
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <config.h>

#if ENABLE(SVG)
#include "AffineTransform.h"

#include "SVGException.h"
#include "V8Binding.h"
#include "V8Proxy.h"
#include "V8SVGMatrix.h"
#include "V8SVGPODTypeWrapper.h"

namespace WebCore {

v8::Handle<v8::Value> V8SVGMatrix::multiplyCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.SVGMatrix.multiply()");
    if (args.Length() < 1)
        return throwError("Not enough arguments");

    if (!V8SVGMatrix::HasInstance(args[0]))
        return throwError("secondMatrix argument was not a SVGMatrix");

    AffineTransform m1 = *V8SVGPODTypeWrapper<AffineTransform>::toNative(args.Holder());
    AffineTransform m2 = *V8SVGPODTypeWrapper<AffineTransform>::toNative(v8::Handle<v8::Object>::Cast(args[0]));

    RefPtr<V8SVGStaticPODTypeWrapper<AffineTransform> > wrapper = V8SVGStaticPODTypeWrapper<AffineTransform>::create(m1.multLeft(m2));
    return toV8(wrapper.get());
}

v8::Handle<v8::Value> V8SVGMatrix::inverseCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.SVGMatrix.inverse()");
    AffineTransform matrix = *V8SVGPODTypeWrapper<AffineTransform>::toNative(args.Holder());
    ExceptionCode ec = 0;
    AffineTransform result = matrix.inverse();

    if (!matrix.isInvertible())
        ec = SVGException::SVG_MATRIX_NOT_INVERTABLE;

    if (ec != 0) {
        V8Proxy::setDOMException(ec);
        return v8::Handle<v8::Value>();
    }

    RefPtr<V8SVGStaticPODTypeWrapper<AffineTransform> > wrapper = V8SVGStaticPODTypeWrapper<AffineTransform>::create(result);
    return toV8(wrapper.get());
}

v8::Handle<v8::Value> V8SVGMatrix::rotateFromVectorCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.SVGMatrix.rotateFromVector()");
    AffineTransform matrix = *V8SVGPODTypeWrapper<AffineTransform>::toNative(args.Holder());
    ExceptionCode ec = 0;
    float x = toFloat(args[0]);
    float y = toFloat(args[1]);
    AffineTransform result = matrix;
    result.rotateFromVector(x, y);
    if (x == 0.0 || y == 0.0)
        ec = SVGException::SVG_INVALID_VALUE_ERR;

    if (ec != 0) {
        V8Proxy::setDOMException(ec);
        return v8::Handle<v8::Value>();
    }

    RefPtr<V8SVGStaticPODTypeWrapper<AffineTransform> > wrapper = V8SVGStaticPODTypeWrapper<AffineTransform>::create(result);
    return toV8(wrapper.get());
}

} // namespace WebCore

#endif
