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


# (c) 2009, Tom Zanussi <tzanussi@gmail.com>
# Licensed under the terms of the GNU GPL License version 2

# Display avg/min/max wakeup latency

# The common_* event handler fields are the most useful fields common to
# all events.  They don't necessarily correspond to the 'common_*' fields
# in the status files.  Those fields not available as handler params can
# be retrieved via script functions of the form get_common_*().

use 5.010000;
use strict;
use warnings;

use lib "$ENV{'PERF_EXEC_PATH'}/scripts/perl/Perf-Trace-Util/lib";
use lib "./Perf-Trace-Util/lib";
use Perf::Trace::Core;
use Perf::Trace::Util;

my %last_wakeup;

my $max_wakeup_latency;
my $min_wakeup_latency;
my $total_wakeup_latency = 0;
my $total_wakeups = 0;

sub sched::sched_switch
{
    my ($event_name, $context, $common_cpu, $common_secs, $common_nsecs,
	$common_pid, $common_comm,
	$prev_comm, $prev_pid, $prev_prio, $prev_state, $next_comm, $next_pid,
	$next_prio) = @_;

    my $wakeup_ts = $last_wakeup{$common_cpu}{ts};
    if ($wakeup_ts) {
	my $switch_ts = nsecs($common_secs, $common_nsecs);
	my $wakeup_latency = $switch_ts - $wakeup_ts;
	if ($wakeup_latency > $max_wakeup_latency) {
	    $max_wakeup_latency = $wakeup_latency;
	}
	if ($wakeup_latency < $min_wakeup_latency) {
	    $min_wakeup_latency = $wakeup_latency;
	}
	$total_wakeup_latency += $wakeup_latency;
	$total_wakeups++;
    }
    $last_wakeup{$common_cpu}{ts} = 0;
}

sub sched::sched_wakeup
{
    my ($event_name, $context, $common_cpu, $common_secs, $common_nsecs,
	$common_pid, $common_comm,
	$comm, $pid, $prio, $success, $target_cpu) = @_;

    $last_wakeup{$target_cpu}{ts} = nsecs($common_secs, $common_nsecs);
}

sub trace_begin
{
    $min_wakeup_latency = 1000000000;
    $max_wakeup_latency = 0;
}

sub trace_end
{
    printf("wakeup_latency stats:\n\n");
    print "total_wakeups: $total_wakeups\n";
    if ($total_wakeups) {
	printf("avg_wakeup_latency (ns): %u\n",
	       avg($total_wakeup_latency, $total_wakeups));
    } else {
	printf("avg_wakeup_latency (ns): N/A\n");
    }
    printf("min_wakeup_latency (ns): %u\n", $min_wakeup_latency);
    printf("max_wakeup_latency (ns): %u\n", $max_wakeup_latency);

    print_unhandled();
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
