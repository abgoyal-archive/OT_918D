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
    Copyright (C) 2004, 2005, 2008 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(SVG)
#include "SVGTests.h"

#include "DOMImplementation.h"
#include "Language.h"
#include "MappedAttribute.h"
#include "SVGElement.h"
#include "SVGNames.h"
#include "SVGStringList.h"

namespace WebCore {

SVGTests::SVGTests()
{
}

SVGTests::~SVGTests()
{
}

SVGStringList* SVGTests::requiredFeatures() const
{
    if (!m_features)
        m_features = SVGStringList::create(SVGNames::requiredFeaturesAttr);

    return m_features.get();
}

SVGStringList* SVGTests::requiredExtensions() const
{
    if (!m_extensions)
        m_extensions = SVGStringList::create(SVGNames::requiredExtensionsAttr);

    return m_extensions.get();
}

SVGStringList* SVGTests::systemLanguage() const
{
    if (!m_systemLanguage)
        m_systemLanguage = SVGStringList::create(SVGNames::systemLanguageAttr);

    return m_systemLanguage.get();
}

bool SVGTests::hasExtension(const String&) const
{
    return false;
}

bool SVGTests::isValid() const
{
    ExceptionCode ec = 0;

    if (m_features) {
        for (unsigned long i = 0; i < m_features->numberOfItems(); i++) {
            String value = m_features->getItem(i, ec);
            if (value.isEmpty() || !DOMImplementation::hasFeature(value, String()))
                return false;
        }
    }

    if (m_systemLanguage) {
        for (unsigned long i = 0; i < m_systemLanguage->numberOfItems(); i++)
            if (m_systemLanguage->getItem(i, ec) != defaultLanguage().substring(0, 2))
                return false;
    }

    if (m_extensions && m_extensions->numberOfItems() > 0)
        return false;

    return true;
}

bool SVGTests::parseMappedAttribute(MappedAttribute* attr)
{
    if (attr->name() == SVGNames::requiredFeaturesAttr) {
        requiredFeatures()->reset(attr->value());
        return true;
    } else if (attr->name() == SVGNames::requiredExtensionsAttr) {
        requiredExtensions()->reset(attr->value());
        return true;
    } else if (attr->name() == SVGNames::systemLanguageAttr) {
        systemLanguage()->reset(attr->value());
        return true;
    }
    
    return false;
}

bool SVGTests::isKnownAttribute(const QualifiedName& attrName)
{
    return (attrName == SVGNames::requiredFeaturesAttr ||
            attrName == SVGNames::requiredExtensionsAttr ||
            attrName == SVGNames::systemLanguageAttr);
}

}

#endif // ENABLE(SVG)
