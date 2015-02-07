#!/bin/sh
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
# arch/sh/boot/install.sh
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995 by Linus Torvalds
#
# Adapted from code in arch/i386/boot/Makefile by H. Peter Anvin
# Adapted from code in arch/i386/boot/install.sh by Russell King
# Adapted from code in arch/arm/boot/install.sh by Stuart Menefy
# Adapted from code in arch/sh/boot/install.sh by Takeo Takahashi
#
# "make install" script for sh architecture
#
# Arguments:
#   $1 - kernel version
#   $2 - kernel image file
#   $3 - kernel map file
#   $4 - default install path (blank if root directory)
#

# User may have a custom install script

if [ -x /sbin/${INSTALLKERNEL} ]; then
  exec /sbin/${INSTALLKERNEL} "$@"
fi

if [ "$2" = "zImage" ]; then
# Compressed install
  echo "Installing compressed kernel"
  if [ -f $4/vmlinuz-$1 ]; then
    mv $4/vmlinuz-$1 $4/vmlinuz.old
  fi

  if [ -f $4/System.map-$1 ]; then
    mv $4/System.map-$1 $4/System.old
  fi

  cat $2 > $4/vmlinuz-$1
  cp $3 $4/System.map-$1
else
# Normal install
  echo "Installing normal kernel"
  if [ -f $4/vmlinux-$1 ]; then
    mv $4/vmlinux-$1 $4/vmlinux.old
  fi

  if [ -f $4/System.map ]; then
    mv $4/System.map $4/System.old
  fi

  cat $2 > $4/vmlinux-$1
  cp $3 $4/System.map
fi
