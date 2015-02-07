#!/usr/bin/env python
# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.



import os
import shutil
import sys

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "build")))

import build_utils

script_dir = os.path.abspath(os.path.dirname(__file__))
wxwebkit_dir = os.path.abspath(os.path.join(script_dir, "..", "..", "..", "WebKitBuild", "Debug" + build_utils.git_branch_name()))
wxwk_root = os.path.abspath(os.path.join(script_dir, "..", "..", ".."))

try:
    os.chdir(wxwk_root)
    deb_dir = os.path.join(wxwk_root, 'wxwebkit')
    if os.path.exists(deb_dir):
        shutil.rmtree(deb_dir)
    os.makedirs(deb_dir)
    print "Archiving git tree..."
    os.system('git archive --format=tar HEAD | gzip > %s/webkitwx_0.1.orig.tar.gz' % deb_dir)
    src_root = os.path.join(deb_dir, 'webkitwx-0.1')
    print "Extracting tree..."    
    os.makedirs(src_root)
    os.chdir(src_root)
    os.system('tar xzvf ../webkitwx_0.1.orig.tar.gz')

    shutil.copytree(os.path.join(script_dir, 'debian'), os.path.join(src_root, 'debian'))

    print "Building package..."
    os.system('fakeroot debian/rules clean')
    os.system('fakeroot debian/rules build')
    os.system('debuild -i -rfakeroot -us -uc')
finally:
    shutil.rmtree(os.path.join(src_root, 'debian'))
