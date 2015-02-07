#!/usr/bin/perl -w
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



# Copyright (C) 2008 Julien Chaffraix <jchaffraix@webkit.org>
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

use strict;

package InFilesParser;

my $isParsingCommonParameters;
my $hasStartedParsing;

# Helper functions

sub trimComment
{   
    my $string = shift;
    $string =~ s/#.+$//;
    chomp($string);
    return $string;
}

sub trimWS
{
    my $string = shift;
    $string =~ s/^\s+//;
    $string =~ s/\s+$//;
    chomp($string);
    return $string;
}

sub trimQuoteAndWS
{
    my $string = shift;
    $string =~ s/\"([^\"]+)\"/$1/;
    return trimWS($string);
}

# Default constructor

sub new
{
    my $object = shift;
    my $reference = { };

    # Initialize the parser.
    $isParsingCommonParameters = 1;
    $hasStartedParsing = 0;

    bless($reference, $object);
    return $reference;
}

# parse take 3 attributes:
# - the filestream to read from (the caller has to open / close it).
# - the commonParameterHandler called when parsing the first part of the file with the parameter and the value.
# - the perTagHandler called for each optional parameter with the element name, the parameter and its value.
#   If no parameter were provided, it is called once with an empty parameter and value.
sub parse($)
{
    my $object = shift;
    my $fileStream = shift; # IO::File only
    my $commonParameterHandler = shift;
    my $perTagHandler = shift;

    foreach (<$fileStream>) {
        # Empty line, change from common parameter part
        # to per tag part if we have started parsing.
        if (/^$/) {
            if ($hasStartedParsing) {
                $isParsingCommonParameters = 0;
            }
            next;
        }

        # There may be a few empty lines at the beginning of the file
        # so detect the first non empty line which starts the common
        # parameters part.
        $hasStartedParsing = 1;

        if (/^#/) {
            next;
        }

        $_ = trimComment($_);

        if ($isParsingCommonParameters) {
            my ($name, $value) = split '=', $_;

            $name = trimWS($name);
            if (defined($value)) {
                $value = trimQuoteAndWS($value);
            } else {
                # We default to 1 as it eases the syntax.
                $value = "1";
            }

            &$commonParameterHandler($name, $value);
        } else {
            # Parsing per-tag parameters.

            # Split the tag name ($1) from the optionnal parameter(s) ($2)
            /^(\S+)\s*(.*)$/;
            my $elementName = $1;

            if ($2) {
                my @options = split "," , $2;
                my ($option, $value);
                for (my $i = 0; $i < @options; ++$i) {
                    ($option, $value) = split "=", $options[$i];
                    $option = trimWS($option);
                    if (defined($value)) {
                        $value = trimQuoteAndWS($value);
                    } else {
                        # We default to 1 as it eases the syntax.
                        $value = "1";
                    }

                    &$perTagHandler($elementName, $option, $value);
                }
            } else {
                # No parameter was given so call it with empty strings.
                &$perTagHandler($elementName, "", "");
            }
        }
    }
}

1;
