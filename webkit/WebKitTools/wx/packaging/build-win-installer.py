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



# Copyright (C) 2008 Kevin Ollivier  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Create a Windows installer package for wxPython wxWebKit binaries

import sys, os, string
import commands
import glob
from subprocess import *

script_dir = os.path.abspath(os.path.dirname(__file__))
sys.path.append(os.path.abspath(os.path.join(script_dir, "..", "build")))

from build_utils import *

wxwk_root = os.path.abspath(os.path.join(script_dir, "..", "..", ".."))
wxwebkit_dir = os.path.abspath(os.path.join(wxwk_root, "WebKitBuild", get_config(wxwk_root) + git_branch_name()))

# Find InnoSetup executable
def getInnoSetupPath():
    name = "ISCC.exe"
    retval = ""
    dirs = os.environ["PATH"].split(":")
    # Add the default file path
    dirs.append("C:\\Program Files\\Inno Setup 5")
    dirs.append("C:\\Program Files (x86)\\Inno Setup 5")
                    
    if os.environ.has_key("INNO5"):
        retval = os.environ["INNO5"]
    
    if retval == "":
        for dir in dirs:
            filepath = os.path.join(dir, name)
            if os.path.isfile(filepath):
                retval = filepath
            
    return retval

if __name__ == "__main__":
    innoSetup = getInnoSetupPath()
    os.chdir(sys.path[0])

    svnrevision = svn_revision()

    if not os.path.exists(innoSetup):
        print "ERROR: Cannot find InnoSetup."
        #sys.exit(1)
        
    if not os.path.exists(wxwebkit_dir):
        print "ERROR: Build dir %s doesn't exist." % wxwebkit_dir
        sys.exit(1)

    fileList = """
CopyMode: alwaysoverwrite; Source: *.pyd;        DestDir: "{app}"
CopyMode: alwaysoverwrite; Source: *.py;        DestDir: "{app}"
"""
    
    dlls = glob.glob(os.path.join(wxwebkit_dir, "*.dll"))
    for dll in dlls:
        if dll.find("wxbase") == -1 and dll.find("wxmsw") == -1:
            fileList += """CopyMode: alwaysoverwrite; Source: %s;        DestDir: "{app}" \n""" % dll

    installerTemplate = open("wxWebKitInstaller.iss.in", "r").read()

    installerTemplate = installerTemplate.replace("<<VERSION>>", svnrevision)
    installerTemplate = installerTemplate.replace("<<ROOTDIR>>", wxwebkit_dir )
    installerTemplate = installerTemplate.replace("<<PYTHONVER>>", sys.version[0:3] )
    installerTemplate = installerTemplate.replace("<<FILES>>", fileList )

    outputFile = open("wxWebKitInstaller.iss", "w")
    outputFile.write(installerTemplate)
    outputFile.close()

    success = os.system('"%s" wxWebKitInstaller.iss' % innoSetup)
    sys.exit(success)
