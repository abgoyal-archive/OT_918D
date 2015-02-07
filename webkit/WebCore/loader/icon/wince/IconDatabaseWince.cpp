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
 *  Copyright (C) 2007-2009 Torch Mobile Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "IconDatabase.h"

#include "AutodrainedPool.h"
#include "CString.h"
#include "DocumentLoader.h"
#include "FileSystem.h"
#include "IconDatabaseClient.h"
#include "IconRecord.h"
#include "Image.h"

namespace WebCore {

// Function to obtain the global icon database.
IconDatabase* iconDatabase() { return 0; }

IconDatabase::IconDatabase() {}
IconDatabase::~IconDatabase() {}

void IconDatabase::setClient(IconDatabaseClient*) {}

bool IconDatabase::open(const String& path) { return false; }
void IconDatabase::close() {}

void IconDatabase::removeAllIcons() {}

Image* IconDatabase::iconForPageURL(const String&, const IntSize&) { return 0; }
void IconDatabase::readIconForPageURLFromDisk(const String&) {}
String IconDatabase::iconURLForPageURL(const String&) { return String(); }
Image* IconDatabase::defaultIcon(const IntSize&) { return 0;}

void IconDatabase::retainIconForPageURL(const String&) {}
void IconDatabase::releaseIconForPageURL(const String&) {}

void IconDatabase::setIconDataForIconURL(PassRefPtr<SharedBuffer> data, const String&) {}
void IconDatabase::setIconURLForPageURL(const String& iconURL, const String& pageURL) {}

IconLoadDecision IconDatabase::loadDecisionForIconURL(const String&, DocumentLoader*) { return IconLoadNo; }
bool IconDatabase::iconDataKnownForIconURL(const String&) { return false; }

void IconDatabase::setEnabled(bool enabled) {}
bool IconDatabase::isEnabled() const { return false; }

void IconDatabase::setPrivateBrowsingEnabled(bool flag) {}
bool IconDatabase::isPrivateBrowsingEnabled() const { return false; }

void IconDatabase::delayDatabaseCleanup() {}
void IconDatabase::allowDatabaseCleanup() {}
void IconDatabase::checkIntegrityBeforeOpening() {}

// Support for WebCoreStatistics in WebKit
size_t IconDatabase::pageURLMappingCount() { return 0; }
size_t IconDatabase::retainedPageURLCount() {return 0; }
size_t IconDatabase::iconRecordCount() { return 0; }
size_t IconDatabase::iconRecordCountWithData() { return 0; }

bool IconDatabase::isOpen() const { return false; }
String IconDatabase::databasePath() const { return String(); }
String IconDatabase::defaultDatabaseFilename() { return String(); }

} // namespace WebCore
