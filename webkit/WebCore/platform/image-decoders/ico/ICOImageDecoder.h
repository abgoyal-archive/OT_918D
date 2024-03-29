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
 * Copyright (c) 2008, 2009, Google Inc. All rights reserved.
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

#ifndef ICOImageDecoder_h
#define ICOImageDecoder_h

#include "BMPImageReader.h"

namespace WebCore {

    class PNGImageDecoder;

    // This class decodes the ICO and CUR image formats.
    class ICOImageDecoder : public ImageDecoder {
    public:
        ICOImageDecoder();

        // ImageDecoder
        virtual String filenameExtension() const { return "ico"; }
        virtual void setData(SharedBuffer*, bool allDataReceived);
        virtual bool isSizeAvailable();
        virtual IntSize size() const;
        virtual IntSize frameSizeAtIndex(size_t) const;
        virtual bool setSize(unsigned width, unsigned height);
        virtual size_t frameCount();
        virtual RGBA32Buffer* frameBufferAtIndex(size_t);

    private:
        enum ImageType {
            Unknown,
            BMP,
            PNG,
        };

        struct IconDirectoryEntry {
            IntSize m_size;
            uint16_t m_bitCount;
            uint32_t m_imageOffset;
        };

        // Returns true if |a| is a preferable icon entry to |b|.
        // Larger sizes, or greater bitdepths at the same size, are preferable.
        static bool compareEntries(const IconDirectoryEntry& a,
                                   const IconDirectoryEntry& b);

        inline uint16_t readUint16(int offset) const
        {
            return BMPImageReader::readUint16(m_data.get(),
                                              m_decodedOffset + offset);
        }

        inline uint32_t readUint32(int offset) const
        {
            return BMPImageReader::readUint32(m_data.get(),
                                              m_decodedOffset + offset);
        }

        // If the desired PNGImageDecoder exists, gives it the appropriate data.
        void setDataForPNGDecoderAtIndex(size_t);

        // Decodes the entry at |index|.  If |onlySize| is true, stops decoding
        // after calculating the image size.  If decoding fails but there is no
        // more data coming, sets the "decode failure" flag.
        //
        // NOTE: If the desired entry is a PNG, this doesn't actually trigger
        // decoding, it merely ensures the decoder is created and ready to
        // decode.  The caller will then call a function on the PNGImageDecoder
        // that actually triggers decoding.
        void decodeWithCheckForDataEnded(size_t index, bool onlySize);

        // Decodes the directory and directory entries at the beginning of the
        // data, and initializes members.  Returns true if all decoding
        // succeeded.  Once this returns true, all entries' sizes are known.
        bool decodeDirectory();

        // Decodes the specified entry.
        bool decodeAtIndex(size_t);

        // Processes the ICONDIR at the beginning of the data.  Returns true if
        // the directory could be decoded.
        bool processDirectory();

        // Processes the ICONDIRENTRY records after the directory.  Keeps the
        // "best" entry as the one we'll decode.  Returns true if the entries
        // could be decoded.
        bool processDirectoryEntries();

        // Reads and returns a directory entry from the current offset into
        // |data|.
        IconDirectoryEntry readDirectoryEntry();

        // Determines whether the desired entry is a BMP or PNG.  Returns true
        // if the type could be determined.
        ImageType imageTypeAtIndex(size_t);

        // True if we've seen all the data.
        bool m_allDataReceived;

        // An index into |m_data| representing how much we've already decoded.
        // Note that this only tracks data _this_ class decodes; once the
        // BMPImageReader takes over this will not be updated further.
        size_t m_decodedOffset;

        // The headers for the ICO.
        typedef Vector<IconDirectoryEntry> IconDirectoryEntries;
        IconDirectoryEntries m_dirEntries;

        // The image decoders for the various frames.
        typedef Vector<OwnPtr<BMPImageReader> > BMPReaders;
        BMPReaders m_bmpReaders;
        typedef Vector<OwnPtr<PNGImageDecoder> > PNGDecoders;
        PNGDecoders m_pngDecoders;

        // Valid only while a BMPImageReader is decoding, this holds the size
        // for the particular entry being decoded.
        IntSize m_frameSize;
    };

} // namespace WebCore

#endif
