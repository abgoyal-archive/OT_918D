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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <linux/utsname.h>
#include <asm/uaccess.h>
#include <linux/tick.h>
#include "prof_main.h"
#include "prof_mem.h"
struct mt_proc_struct *mt_proc[PROC_NUM];
int proc_count = 0;
int mtsched_enabled = 0;
unsigned long long prof_start_ts, prof_end_ts;
static DEFINE_MUTEX(mt_cputime_lock);
static DEFINE_MUTEX(mt_memprof_lock);

unsigned long long cpu0_idletime_start, cpu0_iowait_start;
unsigned long long cpu0_idletime_end, cpu0_iowait_end;

void mt_cputime_switch(int on);
unsigned long long mtprof_get_cpu_idle(int cpu)
{
    unsigned long long *unused = 0;
    return get_cpu_idle_time_us(cpu, unused);
}
unsigned long long mtprof_get_cpu_iowait(int cpu)
{
    unsigned long long *unused = 0;
    return get_cpu_iowait_time_us(cpu, unused);
}
void mt_task_times(struct task_struct *p, cputime_t *ut, cputime_t *st){
    task_times(p,ut,st);
}
/********************
     MT cputime prof
*********************/
#ifdef CONFIG_MTPROF_CPUTIME
void setup_mtproc_info(struct task_struct *p, unsigned long long ts)
{
    struct mt_proc_struct *mtproc;

    if(0 == mtsched_enabled){
	return;
    }
    if(proc_count>= PROC_NUM){
	return;
    }
    mtproc = kmalloc(sizeof(struct mt_proc_struct), GFP_KERNEL);
    if(!mtproc){
	printk("mtproc unable to kmalloc\n");
	return;
    }
    proc_count++; 

    mtproc->pid = p->pid;
    mtproc->tgid = p->tgid;
    mtproc->index = proc_count;
    mtproc->cputime = 0 ;
    mtproc->cputime_init = p->se.sum_exec_runtime;
    mtproc->prof_start = ts;
    mtproc->prof_end = 0;
    mt_task_times(p,&mtproc->utime_init, &mtproc->stime_init);
    strcpy(mtproc->comm, p->comm);
    mt_proc[proc_count - 1] = mtproc;
}
void save_mtproc_info(struct task_struct *p, unsigned long long ts)
{
    struct mt_proc_struct *mtproc;

    mutex_lock(&mt_cputime_lock);
    if(0 == mtsched_enabled){
	mutex_unlock(&mt_cputime_lock);
	return;
    }

    if(proc_count>= PROC_NUM){
	mutex_unlock(&mt_cputime_lock);
	return;
    }
    mutex_unlock(&mt_cputime_lock);

    mtproc = kmalloc(sizeof(struct mt_proc_struct), GFP_KERNEL);
    if(!mtproc){
	printk("mtproc unable to kmalloc\n");
	return;
    }
    mutex_lock(&mt_cputime_lock);
    proc_count++; 

    mtproc->pid = p->pid;
    mtproc->tgid = p->tgid;
    mtproc->index = proc_count;
    mtproc->cputime = 0 ;
    mtproc->cputime_init = p->se.sum_exec_runtime;
    mtproc->prof_start = ts;
    mtproc->prof_end = 0;
    mt_task_times(p,&mtproc->utime_init, &mtproc->stime_init);
    strcpy(mtproc->comm, p->comm);
    mt_proc[proc_count - 1] = mtproc;
    mutex_unlock(&mt_cputime_lock);
}
void end_mtproc_info(struct task_struct *p)
{
    int i;

    mutex_lock(&mt_cputime_lock);
    //check profiling enable flag
    if(0 == mtsched_enabled){
	mutex_unlock(&mt_cputime_lock);
	return;
    }

    //may waste time...
    for(i=0;i<proc_count; i++){
	if(p->pid == mt_proc[i]->pid)
	    break;
    }
    if(i == proc_count){
	printk("pid:%d can't be found in mtsched proc_info\n",p->pid);
	mutex_unlock(&mt_cputime_lock);
	return;
    }
    mt_proc[i]->prof_end = sched_clock();
    /* update cputime */
    mt_proc[i]->cputime = p->se.sum_exec_runtime;
    mt_task_times(p,&mt_proc[i]->utime, &mt_proc[i]->stime);
    mt_proc[i]->utime = cputime_sub(mt_proc[i]->utime, mt_proc[i]->utime_init);
    mt_proc[i]->stime = cputime_sub(mt_proc[i]->stime, mt_proc[i]->stime_init);
    
    mutex_unlock(&mt_cputime_lock);
    return;
}
void set_mtprof_comm(char* comm, int pid)
{
    int i;

    mutex_lock(&mt_cputime_lock);
    if(0 == mtsched_enabled){
	mutex_unlock(&mt_cputime_lock);
	return;
    }
    for(i=0;i<proc_count; i++){
	if(mt_proc[i]==NULL)
	{
	    printk("[mtprof] close mtprof cputime, [%d:%s]\n",pid, comm);
	    mt_cputime_switch(0);
	    mutex_unlock(&mt_cputime_lock);
	    return;
	}
	if(pid == mt_proc[i]->pid)
	    break;
    }
    if(i>= proc_count){
	printk("[mtprof] no matching pid\n");
	mutex_unlock(&mt_cputime_lock);
	return;
    }
    memset(mt_proc[i]->comm, 0, TASK_COMM_LEN);
    wmb();
    strlcpy(mt_proc[i]->comm, comm, TASK_COMM_LEN);
    mutex_unlock(&mt_cputime_lock);
    
}
void start_record_task(void)
{
    unsigned long long ts;

    struct task_struct *g, *p;
    unsigned long flags;
    int cpu = 0;
    mtsched_enabled = 1;
    
    prof_start_ts = sched_clock();
    cpu0_idletime_start = mtprof_get_cpu_idle(cpu);// cpu'0', notified SMP
    cpu0_iowait_start = mtprof_get_cpu_iowait(cpu); 
    ts = sched_clock();
    for_each_online_cpu(cpu){
	read_lock_irqsave(&tasklist_lock, flags);
	do_each_thread(g, p) {
	    setup_mtproc_info(p, ts);
	} while_each_thread(g, p);

	read_unlock_irqrestore(&tasklist_lock, flags);
    }
}
void stop_record_task(void)
{
    int i;
    struct task_struct *tsk;

    mtsched_enabled = 0;
    prof_end_ts = sched_clock();
    
    cpu0_idletime_end = mtprof_get_cpu_idle(0);// prepare for SMP
    cpu0_iowait_end = mtprof_get_cpu_iowait(0);

    for(i=0;i<proc_count; i++){
	tsk = find_task_by_vpid(mt_proc[i]->pid);
	if(tsk!=NULL){
	    mt_proc[i]->cputime = tsk->se.sum_exec_runtime;
	    mt_task_times(tsk, &mt_proc[i]->utime, &mt_proc[i]->stime);
	    mt_proc[i]->utime = cputime_sub(mt_proc[i]->utime, mt_proc[i]->utime_init);
	    mt_proc[i]->stime = cputime_sub(mt_proc[i]->stime, mt_proc[i]->stime_init);
	}
    }
}
void reset_record_task(void){
    int i;
    for(i=0;i<proc_count; i++){
	kfree(mt_proc[i]);
    }
    proc_count = 0;
    prof_end_ts = 0;
    
    cpu0_idletime_start = 0;cpu0_iowait_start = 0;
    cpu0_idletime_end = 0;cpu0_iowait_end = 0;
}

void mt_cputime_switch(int on)
{
    printk("Original mtsched enabled = %d\n", mtsched_enabled);
    mutex_lock(&mt_cputime_lock);
    if (mtsched_enabled ^ on) {
	if (on) {
	    reset_record_task();
	    start_record_task();
	} else {
	    stop_record_task();
	}
    }
    mutex_unlock(&mt_cputime_lock);
    printk("Current mtsched enabled = %d\n", mtsched_enabled);
}

#else  //CONFIG_MTPROF_CPUTIME
void setup_mtproc_info(struct task_struct *p, unsigned long long ts){
    return;
}
void save_mtproc_info(struct task_struct *p, unsigned long long ts){
    return;
}
void end_mtproc_info(struct task_struct *p){
    return;
}
void set_mtprof_comm(char* comm, int pid){
    return;
}
void start_record_task(void){
    return;
}
void stop_record_task(void){
    return;
}
void reset_record_task(void){
    return;
}
void mt_cputime_switch(int on){
    return;
}
#endif//end of CONFIG_MTPROF_CPUTIME

/********************
     MT mem prof
*********************/
void reset_allocpages_prof(void)
{
    num_good_pages = 0;
    num_bad_pages = 0;
    good_pages_time = 0;
    bad_pages_time = 0;
}

void start_allocpages_prof(void)
{
    mt_memprof_enabled = 1;
}

void stop_allocpages_prof(void)
{
    mt_memprof_enabled = 0;
}

void mt_memprof_switch(int on)
{
    printk("Original memprof enabled = %d\n", mt_memprof_enabled);
    mutex_lock(&mt_memprof_lock);
    if (mt_memprof_enabled ^ on) {
	if (on) {
	    reset_allocpages_prof();
	    start_allocpages_prof();
	} else {
	    stop_allocpages_prof();
	}
    }
    mutex_unlock(&mt_memprof_lock);
    printk("Current memprof enabled = %d\n", mt_memprof_enabled);

}
