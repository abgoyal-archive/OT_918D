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
 * Copyright 2008, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PICTURESET_H
#define PICTURESET_H

#define PICTURE_SET_DUMP 0
#define PICTURE_SET_DEBUG 0
#define PICTURE_SET_VALIDATE 0

#if PICTURE_SET_DEBUG
#define DBG_SET_LOG(message) LOGD("%s %s", __FUNCTION__, message)
#define DBG_SET_LOGD(format, ...) LOGD("%s " format, __FUNCTION__, __VA_ARGS__)
#define DEBUG_SET_UI_LOGD(...) LOGD(__VA_ARGS__)
#else
#define DBG_SET_LOG(message) ((void)0)
#define DBG_SET_LOGD(format, ...) ((void)0)
#define DEBUG_SET_UI_LOGD(...) ((void)0)
#endif

#include "jni.h"
#include "SkRegion.h"
#include <wtf/Vector.h>

class SkCanvas;
class SkPicture;
class SkIRect;

namespace android {

    class PictureSet {
    public:
        PictureSet();
        PictureSet(const PictureSet& src) { set(src); }
        virtual ~PictureSet();
        void add(const SkRegion& area, SkPicture* picture,
            uint32_t elapsed, bool split) 
        {
            add(area, picture, elapsed, split, emptyPicture(picture));
        }
        void add(const SkRegion& area, SkPicture* picture,
            uint32_t elapsed, bool split, bool empty);
        const SkIRect& bounds(size_t i) const {
            return mPictures[i].mArea.getBounds(); }
        bool build();
        // Update mWidth/mHeight, and adds any additional inval region
        void checkDimensions(int width, int height, SkRegion* inval);
        void clear();
        bool draw(SkCanvas* );
        static PictureSet* GetNativePictureSet(JNIEnv* env, jobject jpic);
        int height() const { return mHeight; }
        bool isEmpty() const; // returns true if empty or only trivial content
        bool reuseSubdivided(const SkRegion& );
        void set(const PictureSet& );
        void setDrawTimes(const PictureSet& );
        void setPicture(size_t i, SkPicture* p);
        size_t size() const { return mPictures.size(); }
        void split(PictureSet* result) const;
        bool upToDate(size_t i) const { return mPictures[i].mPicture != NULL; }
        int width() const { return mWidth; }
        void dump(const char* label) const;
        bool validate(const char* label) const;
    private:
        bool emptyPicture(SkPicture* ) const; // true if no text, images, paths
        struct Pictures {
            SkRegion mArea;
            SkPicture* mPicture;
            SkIRect mUnsplit;
            uint32_t mElapsed;
            bool mSplit : 8;
            bool mWroteElapsed : 8;
            bool mBase : 8; // true if nothing is drawn underneath this
            bool mEmpty : 8; // true if the picture only draws white
        };
        void add(const Pictures* temp);
        WTF::Vector<Pictures> mPictures;
        int mHeight;
        int mWidth;
    };
}

#endif
