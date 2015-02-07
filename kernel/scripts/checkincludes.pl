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
# checkincludes: find/remove files included more than once
#
# Copyright abandoned, 2000, Niels Kristian Bech Jensen <nkbj@image.dk>.
# Copyright 2009 Luis R. Rodriguez <mcgrof@gmail.com>
#
# This script checks for duplicate includes. It also has support
# to remove them in place. Note that this will not take into
# consideration macros so you should run this only if you know
# you do have real dups and do not have them under #ifdef's. You
# could also just review the results.

use strict;

sub usage {
	print "Usage: checkincludes.pl [-r]\n";
	print "By default we just warn of duplicates\n";
	print "To remove duplicated includes in place use -r\n";
	exit 1;
}

my $remove = 0;

if ($#ARGV < 0) {
	usage();
}

if ($#ARGV >= 1) {
	if ($ARGV[0] =~ /^-/) {
		if ($ARGV[0] eq "-r") {
			$remove = 1;
			shift;
		} else {
			usage();
		}
	}
}

foreach my $file (@ARGV) {
	open(my $f, '<', $file)
	    or die "Cannot open $file: $!.\n";

	my %includedfiles = ();
	my @file_lines = ();

	while (<$f>) {
		if (m/^\s*#\s*include\s*[<"](\S*)[>"]/o) {
			++$includedfiles{$1};
		}
		push(@file_lines, $_);
	}

	close($f);

	if (!$remove) {
		foreach my $filename (keys %includedfiles) {
			if ($includedfiles{$filename} > 1) {
				print "$file: $filename is included more than once.\n";
			}
		}
		next;
	}

	open($f, '>', $file)
	    or die("Cannot write to $file: $!");

	my $dups = 0;
	foreach (@file_lines) {
		if (m/^\s*#\s*include\s*[<"](\S*)[>"]/o) {
			foreach my $filename (keys %includedfiles) {
				if ($1 eq $filename) {
					if ($includedfiles{$filename} > 1) {
						$includedfiles{$filename}--;
						$dups++;
					} else {
						print {$f} $_;
					}
				}
			}
		} else {
			print {$f} $_;
		}
	}
	if ($dups > 0) {
		print "$file: removed $dups duplicate includes\n";
	}
	close($f);
}
