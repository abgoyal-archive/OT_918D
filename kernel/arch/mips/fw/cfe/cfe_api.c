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
 * Copyright (C) 2000, 2001, 2002 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
 *
 * Broadcom Common Firmware Environment (CFE)
 *
 * This module contains device function stubs (small routines to
 * call the standard "iocb" interface entry point to CFE).
 * There should be one routine here per iocb function call.
 *
 * Authors:  Mitch Lichtenberg, Chris Demetriou
 */

#include <asm/fw/cfe/cfe_api.h>
#include "cfe_api_int.h"

/* Cast from a native pointer to a cfe_xptr_t and back.	 */
#define XPTR_FROM_NATIVE(n)	((cfe_xptr_t) (intptr_t) (n))
#define NATIVE_FROM_XPTR(x)	((void *) (intptr_t) (x))

int cfe_iocb_dispatch(struct cfe_xiocb *xiocb);

/*
 * Declare the dispatch function with args of "intptr_t".
 * This makes sure whatever model we're compiling in
 * puts the pointers in a single register.  For example,
 * combining -mlong64 and -mips1 or -mips2 would lead to
 * trouble, since the handle and IOCB pointer will be
 * passed in two registers each, and CFE expects one.
 */

static int (*cfe_dispfunc) (intptr_t handle, intptr_t xiocb);
static u64 cfe_handle;

int cfe_init(u64 handle, u64 ept)
{
	cfe_dispfunc = NATIVE_FROM_XPTR(ept);
	cfe_handle = handle;
	return 0;
}

int cfe_iocb_dispatch(struct cfe_xiocb * xiocb)
{
	if (!cfe_dispfunc)
		return -1;
	return (*cfe_dispfunc) ((intptr_t) cfe_handle, (intptr_t) xiocb);
}

int cfe_close(int handle)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_CLOSE;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = handle;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = 0;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;

}

int cfe_cpu_start(int cpu, void (*fn) (void), long sp, long gp, long a1)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_FW_CPUCTL;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_cpuctl);
	xiocb.plist.xiocb_cpuctl.cpu_number = cpu;
	xiocb.plist.xiocb_cpuctl.cpu_command = CFE_CPU_CMD_START;
	xiocb.plist.xiocb_cpuctl.gp_val = gp;
	xiocb.plist.xiocb_cpuctl.sp_val = sp;
	xiocb.plist.xiocb_cpuctl.a1_val = a1;
	xiocb.plist.xiocb_cpuctl.start_addr = (long) fn;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;
}

int cfe_cpu_stop(int cpu)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_FW_CPUCTL;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_cpuctl);
	xiocb.plist.xiocb_cpuctl.cpu_number = cpu;
	xiocb.plist.xiocb_cpuctl.cpu_command = CFE_CPU_CMD_STOP;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;
}

int cfe_enumenv(int idx, char *name, int namelen, char *val, int vallen)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_ENV_SET;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_envbuf);
	xiocb.plist.xiocb_envbuf.enum_idx = idx;
	xiocb.plist.xiocb_envbuf.name_ptr = XPTR_FROM_NATIVE(name);
	xiocb.plist.xiocb_envbuf.name_length = namelen;
	xiocb.plist.xiocb_envbuf.val_ptr = XPTR_FROM_NATIVE(val);
	xiocb.plist.xiocb_envbuf.val_length = vallen;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;
}

int
cfe_enummem(int idx, int flags, u64 *start, u64 *length, u64 *type)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_FW_MEMENUM;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = flags;
	xiocb.xiocb_psize = sizeof(struct xiocb_meminfo);
	xiocb.plist.xiocb_meminfo.mi_idx = idx;

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;

	*start = xiocb.plist.xiocb_meminfo.mi_addr;
	*length = xiocb.plist.xiocb_meminfo.mi_size;
	*type = xiocb.plist.xiocb_meminfo.mi_type;

	return 0;
}

int cfe_exit(int warm, int status)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_FW_RESTART;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = warm ? CFE_FLG_WARMSTART : 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_exitstat);
	xiocb.plist.xiocb_exitstat.status = status;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;
}

int cfe_flushcache(int flg)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_FW_FLUSHCACHE;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = flg;
	xiocb.xiocb_psize = 0;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;
}

int cfe_getdevinfo(char *name)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_GETINFO;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_buffer);
	xiocb.plist.xiocb_buffer.buf_offset = 0;
	xiocb.plist.xiocb_buffer.buf_ptr = XPTR_FROM_NATIVE(name);
	xiocb.plist.xiocb_buffer.buf_length = strlen(name);

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;
	return xiocb.plist.xiocb_buffer.buf_ioctlcmd;
}

int cfe_getenv(char *name, char *dest, int destlen)
{
	struct cfe_xiocb xiocb;

	*dest = 0;

	xiocb.xiocb_fcode = CFE_CMD_ENV_GET;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_envbuf);
	xiocb.plist.xiocb_envbuf.enum_idx = 0;
	xiocb.plist.xiocb_envbuf.name_ptr = XPTR_FROM_NATIVE(name);
	xiocb.plist.xiocb_envbuf.name_length = strlen(name);
	xiocb.plist.xiocb_envbuf.val_ptr = XPTR_FROM_NATIVE(dest);
	xiocb.plist.xiocb_envbuf.val_length = destlen;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;
}

int cfe_getfwinfo(cfe_fwinfo_t * info)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_FW_GETINFO;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_fwinfo);

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;

	info->fwi_version = xiocb.plist.xiocb_fwinfo.fwi_version;
	info->fwi_totalmem = xiocb.plist.xiocb_fwinfo.fwi_totalmem;
	info->fwi_flags = xiocb.plist.xiocb_fwinfo.fwi_flags;
	info->fwi_boardid = xiocb.plist.xiocb_fwinfo.fwi_boardid;
	info->fwi_bootarea_va = xiocb.plist.xiocb_fwinfo.fwi_bootarea_va;
	info->fwi_bootarea_pa = xiocb.plist.xiocb_fwinfo.fwi_bootarea_pa;
	info->fwi_bootarea_size =
	    xiocb.plist.xiocb_fwinfo.fwi_bootarea_size;
#if 0
	info->fwi_reserved1 = xiocb.plist.xiocb_fwinfo.fwi_reserved1;
	info->fwi_reserved2 = xiocb.plist.xiocb_fwinfo.fwi_reserved2;
	info->fwi_reserved3 = xiocb.plist.xiocb_fwinfo.fwi_reserved3;
#endif

	return 0;
}

int cfe_getstdhandle(int flg)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_GETHANDLE;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = flg;
	xiocb.xiocb_psize = 0;

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;
	return xiocb.xiocb_handle;
}

int64_t
cfe_getticks(void)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_FW_GETTIME;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_time);
	xiocb.plist.xiocb_time.ticks = 0;

	cfe_iocb_dispatch(&xiocb);

	return xiocb.plist.xiocb_time.ticks;

}

int cfe_inpstat(int handle)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_INPSTAT;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = handle;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_inpstat);
	xiocb.plist.xiocb_inpstat.inp_status = 0;

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;
	return xiocb.plist.xiocb_inpstat.inp_status;
}

int
cfe_ioctl(int handle, unsigned int ioctlnum, unsigned char *buffer,
	  int length, int *retlen, u64 offset)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_IOCTL;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = handle;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_buffer);
	xiocb.plist.xiocb_buffer.buf_offset = offset;
	xiocb.plist.xiocb_buffer.buf_ioctlcmd = ioctlnum;
	xiocb.plist.xiocb_buffer.buf_ptr = XPTR_FROM_NATIVE(buffer);
	xiocb.plist.xiocb_buffer.buf_length = length;

	cfe_iocb_dispatch(&xiocb);

	if (retlen)
		*retlen = xiocb.plist.xiocb_buffer.buf_retlen;
	return xiocb.xiocb_status;
}

int cfe_open(char *name)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_OPEN;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_buffer);
	xiocb.plist.xiocb_buffer.buf_offset = 0;
	xiocb.plist.xiocb_buffer.buf_ptr = XPTR_FROM_NATIVE(name);
	xiocb.plist.xiocb_buffer.buf_length = strlen(name);

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;
	return xiocb.xiocb_handle;
}

int cfe_read(int handle, unsigned char *buffer, int length)
{
	return cfe_readblk(handle, 0, buffer, length);
}

int cfe_readblk(int handle, s64 offset, unsigned char *buffer, int length)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_READ;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = handle;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_buffer);
	xiocb.plist.xiocb_buffer.buf_offset = offset;
	xiocb.plist.xiocb_buffer.buf_ptr = XPTR_FROM_NATIVE(buffer);
	xiocb.plist.xiocb_buffer.buf_length = length;

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;
	return xiocb.plist.xiocb_buffer.buf_retlen;
}

int cfe_setenv(char *name, char *val)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_ENV_SET;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = 0;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_envbuf);
	xiocb.plist.xiocb_envbuf.enum_idx = 0;
	xiocb.plist.xiocb_envbuf.name_ptr = XPTR_FROM_NATIVE(name);
	xiocb.plist.xiocb_envbuf.name_length = strlen(name);
	xiocb.plist.xiocb_envbuf.val_ptr = XPTR_FROM_NATIVE(val);
	xiocb.plist.xiocb_envbuf.val_length = strlen(val);

	cfe_iocb_dispatch(&xiocb);

	return xiocb.xiocb_status;
}

int cfe_write(int handle, unsigned char *buffer, int length)
{
	return cfe_writeblk(handle, 0, buffer, length);
}

int cfe_writeblk(int handle, s64 offset, unsigned char *buffer, int length)
{
	struct cfe_xiocb xiocb;

	xiocb.xiocb_fcode = CFE_CMD_DEV_WRITE;
	xiocb.xiocb_status = 0;
	xiocb.xiocb_handle = handle;
	xiocb.xiocb_flags = 0;
	xiocb.xiocb_psize = sizeof(struct xiocb_buffer);
	xiocb.plist.xiocb_buffer.buf_offset = offset;
	xiocb.plist.xiocb_buffer.buf_ptr = XPTR_FROM_NATIVE(buffer);
	xiocb.plist.xiocb_buffer.buf_length = length;

	cfe_iocb_dispatch(&xiocb);

	if (xiocb.xiocb_status < 0)
		return xiocb.xiocb_status;
	return xiocb.plist.xiocb_buffer.buf_retlen;
}
