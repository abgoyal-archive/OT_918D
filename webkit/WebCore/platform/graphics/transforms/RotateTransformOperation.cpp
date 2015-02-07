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
 * Copyright (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "RotateTransformOperation.h"

#include <algorithm>
#include <wtf/MathExtras.h>

using namespace std;

namespace WebCore {

PassRefPtr<TransformOperation> RotateTransformOperation::blend(const TransformOperation* from, double progress, bool blendToIdentity)
{
    if (from && !from->isSameType(*this))
        return this;
    
    if (blendToIdentity)
        return RotateTransformOperation::create(m_x, m_y, m_z, m_angle - m_angle * progress, m_type);
    
    const RotateTransformOperation* fromOp = static_cast<const RotateTransformOperation*>(from);
    
    // Optimize for single axis rotation
    if (!fromOp || (fromOp->m_x == 0 && fromOp->m_y == 0 && fromOp->m_z == 1) || 
                   (fromOp->m_x == 0 && fromOp->m_y == 1 && fromOp->m_z == 0) || 
                   (fromOp->m_x == 1 && fromOp->m_y == 0 && fromOp->m_z == 0)) {
        double fromAngle = fromOp ? fromOp->m_angle : 0;
        return RotateTransformOperation::create(fromOp ? fromOp->m_x : m_x, 
                                                fromOp ? fromOp->m_y : m_y, 
                                                fromOp ? fromOp->m_z : m_z, 
                                                fromAngle + (m_angle - fromAngle) * progress, m_type);
    }

    const RotateTransformOperation* toOp = this;

    // Create the 2 rotation matrices
    TransformationMatrix fromT;
    TransformationMatrix toT;
    fromT.rotate3d((float)(fromOp ? fromOp->m_x : 0),
                   (float)(fromOp ? fromOp->m_y : 0),
                   (float)(fromOp ? fromOp->m_z : 1),
                   (float)(fromOp ? fromOp->m_angle : 0));

    toT.rotate3d((float)(toOp ? toOp->m_x : 0),
                 (float)(toOp ? toOp->m_y : 0),
                 (float)(toOp ? toOp->m_z : 1),
                 (float)(toOp ? toOp->m_angle : 0));
    
    // Blend them
    toT.blend(fromT, progress);
    
    // Extract the result as a quaternion
    TransformationMatrix::DecomposedType decomp;
    toT.decompose(decomp);
    
    // Convert that to Axis/Angle form
    double x = -decomp.quaternionX;
    double y = -decomp.quaternionY;
    double z = -decomp.quaternionZ;
    double length = sqrt(x * x + y * y + z * z);
    double angle = 0;
    
    if (length > 0.00001) {
        x /= length;
        y /= length;
        z /= length;
        angle = rad2deg(acos(decomp.quaternionW) * 2);
    } else {
        x = 0;
        y = 0;
        z = 1;
    }
    return RotateTransformOperation::create(x, y, z, angle, ROTATE_3D);
}

} // namespace WebCore
