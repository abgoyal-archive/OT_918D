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
 * Copyright (C) Research In Motion Limited 2009. All rights reserved.
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
 */

#include "config.h"
#include "VGUtils.h"

#include "FloatRect.h"
#include "TransformationMatrix.h"

namespace WebCore {

VGMatrix::VGMatrix(const VGfloat data[9])
{
    m_data[0] = data[0];
    m_data[1] = data[1];
    m_data[2] = data[2];
    m_data[3] = data[3];
    m_data[4] = data[4];
    m_data[5] = data[5];
    m_data[6] = data[6];
    m_data[7] = data[7];
    m_data[8] = data[8];
}

VGMatrix::VGMatrix(const TransformationMatrix& matrix)
{
    m_data[0] = matrix.m11();
    m_data[1] = matrix.m12();
    m_data[2] = matrix.m14();
    m_data[3] = matrix.m21();
    m_data[4] = matrix.m22();
    m_data[5] = matrix.m24();
    m_data[6] = matrix.m41();
    m_data[7] = matrix.m42();
    m_data[8] = matrix.m44();
}

VGMatrix::operator TransformationMatrix() const
{
    TransformationMatrix matrix;
    matrix.setM11(m_data[0]);
    matrix.setM12(m_data[1]);
    matrix.setM14(m_data[2]);
    matrix.setM21(m_data[3]);
    matrix.setM22(m_data[4]);
    matrix.setM24(m_data[5]);
    matrix.setM41(m_data[6]);
    matrix.setM42(m_data[7]);
    matrix.setM44(m_data[8]);
    return matrix;
}

TransformationMatrix::operator VGMatrix() const
{
    return VGMatrix(*this);
}

VGRect::VGRect(const VGfloat data[4])
{
    m_data[0] = data[0];
    m_data[1] = data[1];
    m_data[2] = data[2];
    m_data[3] = data[3];
}

VGRect::VGRect(const FloatRect& rect)
{
    m_data[0] = rect.x();
    m_data[1] = rect.y();
    m_data[2] = rect.width();
    m_data[3] = rect.height();
}

VGRect::operator FloatRect() const
{
    return FloatRect(m_data[0], m_data[1], m_data[2], m_data[3]);
}

FloatRect::operator VGRect() const
{
    return VGRect(*this);
}

}
