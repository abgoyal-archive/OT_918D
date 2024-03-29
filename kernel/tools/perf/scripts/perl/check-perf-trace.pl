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


# perf trace event handlers, generated by perf trace -g perl
# (c) 2009, Tom Zanussi <tzanussi@gmail.com>
# Licensed under the terms of the GNU GPL License version 2

# This script tests basic functionality such as flag and symbol
# strings, common_xxx() calls back into perf, begin, end, unhandled
# events, etc.  Basically, if this script runs successfully and
# displays expected results, perl scripting support should be ok.

use lib "$ENV{'PERF_EXEC_PATH'}/scripts/perl/Perf-Trace-Util/lib";
use lib "./Perf-Trace-Util/lib";
use Perf::Trace::Core;
use Perf::Trace::Context;
use Perf::Trace::Util;

sub trace_begin
{
    print "trace_begin\n";
}

sub trace_end
{
    print "trace_end\n";

    print_unhandled();
}

sub irq::softirq_entry
{
	my ($event_name, $context, $common_cpu, $common_secs, $common_nsecs,
	    $common_pid, $common_comm,
	    $vec) = @_;

	print_header($event_name, $common_cpu, $common_secs, $common_nsecs,
		     $common_pid, $common_comm);

	print_uncommon($context);

	printf("vec=%s\n",
	       symbol_str("irq::softirq_entry", "vec", $vec));
}

sub kmem::kmalloc
{
	my ($event_name, $context, $common_cpu, $common_secs, $common_nsecs,
	    $common_pid, $common_comm,
	    $call_site, $ptr, $bytes_req, $bytes_alloc,
	    $gfp_flags) = @_;

	print_header($event_name, $common_cpu, $common_secs, $common_nsecs,
		     $common_pid, $common_comm);

	print_uncommon($context);

	printf("call_site=%p, ptr=%p, bytes_req=%u, bytes_alloc=%u, ".
	       "gfp_flags=%s\n",
	       $call_site, $ptr, $bytes_req, $bytes_alloc,

	       flag_str("kmem::kmalloc", "gfp_flags", $gfp_flags));
}

# print trace fields not included in handler args
sub print_uncommon
{
    my ($context) = @_;

    printf("common_preempt_count=%d, common_flags=%s, common_lock_depth=%d, ",
	   common_pc($context), trace_flag_str(common_flags($context)),
	   common_lock_depth($context));

}

my %unhandled;

sub print_unhandled
{
    if ((scalar keys %unhandled) == 0) {
	return;
    }

    print "\nunhandled events:\n\n";

    printf("%-40s  %10s\n", "event", "count");
    printf("%-40s  %10s\n", "----------------------------------------",
	   "-----------");

    foreach my $event_name (keys %unhandled) {
	printf("%-40s  %10d\n", $event_name, $unhandled{$event_name});
    }
}

sub trace_unhandled
{
    my ($event_name, $context, $common_cpu, $common_secs, $common_nsecs,
	$common_pid, $common_comm) = @_;

    $unhandled{$event_name}++;
}

sub print_header
{
	my ($event_name, $cpu, $secs, $nsecs, $pid, $comm) = @_;

	printf("%-20s %5u %05u.%09u %8u %-20s ",
	       $event_name, $cpu, $secs, $nsecs, $pid, $comm);
}
