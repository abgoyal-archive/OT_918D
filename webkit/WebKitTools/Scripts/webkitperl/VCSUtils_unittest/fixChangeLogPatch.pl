#!/usr/bin/perl
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


#
# Copyright (C) 2009, 2010 Chris Jerdonek (chris.jerdonek@gmail.com)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Unit tests of VCSUtils::fixChangeLogPatch().

use Test::Simple tests => 7;
use VCSUtils;

# The source ChangeLog for these tests is the following:
# 
# 2009-12-22  Alice  <alice@email.address>
# 
#         Reviewed by Ray.
# 
#         Changed some code on 2009-12-22.
# 
#         * File:
#         * File2:
# 
# 2009-12-21  Alice  <alice@email.address>
# 
#         Reviewed by Ray.
# 
#         Changed some code on 2009-12-21.
# 
#         * File:
#         * File2:

my $title;
my $in;
my $out;

# New test
$title = "fixChangeLogPatch: [no change] First line is new line.";

$in = <<'END';
--- ChangeLog
+++ ChangeLog
@@ -1,3 +1,11 @@
+2009-12-22  Bob  <bob@email.address>
+
+        Reviewed by Ray.
+
+        Changed some more code on 2009-12-22.
+
+        * File:
+
 2009-12-22  Alice  <alice@email.address>
 
         Reviewed by Ray.
END

ok(fixChangeLogPatch($in) eq $in, $title);

# New test
$title = "fixChangeLogPatch: [no change] No date string.";

$in = <<'END';
--- ChangeLog
+++ ChangeLog
@@ -6,6 +6,7 @@
 
         * File:
         * File2:
+        * File3:
 
 2009-12-21  Alice  <alice@email.address>
 
END

ok(fixChangeLogPatch($in) eq $in, $title);

# New test
$title = "fixChangeLogPatch: [no change] New entry inserted in middle.";

$in = <<'END';
--- ChangeLog
+++ ChangeLog
@@ -11,6 +11,14 @@
 
         Reviewed by Ray.
 
+        Changed some more code on 2009-12-21.
+
+        * File:
+
+2009-12-21  Alice  <alice@email.address>
+
+        Reviewed by Ray.
+
         Changed some code on 2009-12-21.
 
         * File:
END

ok(fixChangeLogPatch($in) eq $in, $title);

# New test
$title = "fixChangeLogPatch: Leading context includes first line.";

$in = <<'END';
--- ChangeLog
+++ ChangeLog
@@ -1,5 +1,13 @@
 2009-12-22  Alice  <alice@email.address>
 
+        Reviewed by Sue.
+
+        Changed some more code on 2009-12-22.
+
+        * File:
+
+2009-12-22  Alice  <alice@email.address>
+
         Reviewed by Ray.
 
         Changed some code on 2009-12-22.
END

$out = <<'END';
--- ChangeLog
+++ ChangeLog
@@ -1,3 +1,11 @@
+2009-12-22  Alice  <alice@email.address>
+
+        Reviewed by Sue.
+
+        Changed some more code on 2009-12-22.
+
+        * File:
+
 2009-12-22  Alice  <alice@email.address>
 
         Reviewed by Ray.
END

ok(fixChangeLogPatch($in) eq $out, $title);

# New test
$title = "fixChangeLogPatch: Leading context does not include first line.";

$in = <<'END';
@@ -2,6 +2,14 @@
 
         Reviewed by Ray.
 
+        Changed some more code on 2009-12-22.
+
+        * File:
+
+2009-12-22  Alice  <alice@email.address>
+
+        Reviewed by Ray.
+
         Changed some code on 2009-12-22.
 
         * File:
END

$out = <<'END';
@@ -1,3 +1,11 @@
+2009-12-22  Alice  <alice@email.address>
+
+        Reviewed by Ray.
+
+        Changed some more code on 2009-12-22.
+
+        * File:
+
 2009-12-22  Alice  <alice@email.address>
 
         Reviewed by Ray.
END

ok(fixChangeLogPatch($in) eq $out, $title);

# New test
$title = "fixChangeLogPatch: Non-consecutive line additions.";

# This can occur, for example, if the new ChangeLog entry includes
# trailing white space in the first blank line but not the second.
# A diff command can then match the second blank line of the new
# ChangeLog entry with the first blank line of the old.
# The svn diff command with the default --diff-cmd has done this.
$in = <<'END';
@@ -1,5 +1,11 @@
 2009-12-22  Alice  <alice@email.address>
+ <pretend-whitespace>
+        Reviewed by Ray.
 
+        Changed some more code on 2009-12-22.
+
+2009-12-22  Alice  <alice@email.address>
+
         Reviewed by Ray.
 
         Changed some code on 2009-12-22.
END

$out = <<'END';
@@ -1,3 +1,9 @@
+2009-12-22  Alice  <alice@email.address>
+ <pretend-whitespace>
+        Reviewed by Ray.
+
+        Changed some more code on 2009-12-22.
+
 2009-12-22  Alice  <alice@email.address>
 
         Reviewed by Ray.
END

ok(fixChangeLogPatch($in) eq $out, $title);

# New test
$title = "fixChangeLogPatch: Additional edits after new entry.";

$in = <<'END';
@@ -2,10 +2,17 @@
 
         Reviewed by Ray.
 
+        Changed some more code on 2009-12-22.
+
+        * File:
+
+2009-12-22  Alice  <alice@email.address>
+
+        Reviewed by Ray.
+
         Changed some code on 2009-12-22.
 
         * File:
-        * File2:
 
 2009-12-21  Alice  <alice@email.address>
 
END

$out = <<'END';
@@ -1,11 +1,18 @@
+2009-12-22  Alice  <alice@email.address>
+
+        Reviewed by Ray.
+
+        Changed some more code on 2009-12-22.
+
+        * File:
+
 2009-12-22  Alice  <alice@email.address>
 
         Reviewed by Ray.
 
         Changed some code on 2009-12-22.
 
         * File:
-        * File2:
 
 2009-12-21  Alice  <alice@email.address>
 
END

ok(fixChangeLogPatch($in) eq $out, $title);
