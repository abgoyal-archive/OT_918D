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
 * YAFFS: Yet another Flash File System . A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2010 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1 as
 * published by the Free Software Foundation.
 *
 * Note: Only YAFFS headers are LGPL, YAFFS C code is covered by GPL.
 *
 * Note: Tis code is currently unused. Being checked in in case it becomes useful.
 */


#include "yaffs_allocator.h"
#include "yaffs_guts.h"
#include "yaffs_trace.h"
#include "yportenv.h"
#include "yaffs_linux.h"
/*
 * Start out with the same allocator as yaffs direct.
 * Todo: Change to Linux slab allocator.
 */



#define NAMELEN  20
struct yaffs_AllocatorStruct {
	char tnode_name[NAMELEN+1];
	char object_name[NAMELEN+1];
	struct kmem_cache *tnode_cache;
	struct kmem_cache *object_cache;
};

typedef struct yaffs_AllocatorStruct yaffs_Allocator;

int mount_id;

void yaffs_DeinitialiseRawTnodesAndObjects(yaffs_Device *dev)
{
	yaffs_Allocator *allocator = (yaffs_Allocator *)dev->allocator;

	T(YAFFS_TRACE_ALLOCATE,(TSTR("Deinitialising yaffs allocator\n")));

	if(allocator){
		if(allocator->tnode_cache){
			kmem_cache_destroy(allocator->tnode_cache);
			allocator->tnode_cache = NULL;
		} else {
			T(YAFFS_TRACE_ALWAYS,
				(TSTR("NULL tnode cache\n")));
			YBUG();
		}

		if(allocator->object_cache){
			kmem_cache_destroy(allocator->object_cache);
			allocator->object_cache = NULL;
		} else {
			T(YAFFS_TRACE_ALWAYS,
				(TSTR("NULL object cache\n")));
			YBUG();
		}

		YFREE(allocator);

	} else {
		T(YAFFS_TRACE_ALWAYS,
			(TSTR("Deinitialising NULL allocator\n")));
		YBUG();
	}
	dev->allocator = NULL;
}


static void fake_ctor0(void *data){data = data;}
static void fake_ctor1(void *data){data = data;}
static void fake_ctor2(void *data){data = data;}
static void fake_ctor3(void *data){data = data;}
static void fake_ctor4(void *data){data = data;}
static void fake_ctor5(void *data){data = data;}
static void fake_ctor6(void *data){data = data;}
static void fake_ctor7(void *data){data = data;}
static void fake_ctor8(void *data){data = data;}
static void fake_ctor9(void *data){data = data;}

static void (*fake_ctor_list[10]) (void *) = {
	fake_ctor0,
	fake_ctor1,
	fake_ctor2,
	fake_ctor3,
	fake_ctor4,
	fake_ctor5,
	fake_ctor6,
	fake_ctor7,
	fake_ctor8,
	fake_ctor9,
};

void yaffs_InitialiseRawTnodesAndObjects(yaffs_Device *dev)
{
	yaffs_Allocator *allocator;
	unsigned mount_id = yaffs_DeviceToLC(dev)->mount_id;

	T(YAFFS_TRACE_ALLOCATE,(TSTR("Initialising yaffs allocator\n")));

	if(dev->allocator)
		YBUG();
	else if(mount_id >= 10){
		T(YAFFS_TRACE_ALWAYS,(TSTR("Bad mount_id %u\n"),mount_id));
	} else {
		 allocator = YMALLOC(sizeof(yaffs_Allocator));
		 memset(allocator,0,sizeof(yaffs_Allocator));
		 dev->allocator = allocator;

		if(!dev->allocator){
			T(YAFFS_TRACE_ALWAYS,
				(TSTR("yaffs allocator creation failed\n")));
			YBUG();
			return;

		}

		sprintf(allocator->tnode_name,"yaffs_t_%u",mount_id);
		sprintf(allocator->object_name,"yaffs_o_%u",mount_id);

		allocator->tnode_cache =
			kmem_cache_create(allocator->tnode_name,
				dev->tnodeSize,
				0, 0,
				fake_ctor_list[mount_id]);
		if(allocator->tnode_cache)
			T(YAFFS_TRACE_ALLOCATE,
				(TSTR("tnode cache \"%s\" %p\n"),
				allocator->tnode_name,allocator->tnode_cache));
		else {
			T(YAFFS_TRACE_ALWAYS,
				(TSTR("yaffs cache creation failed\n")));
			YBUG();
		}


		allocator->object_cache = 
			kmem_cache_create(allocator->object_name,
				sizeof(yaffs_Object),
				0, 0,
				fake_ctor_list[mount_id]);

		if(allocator->object_cache)
			T(YAFFS_TRACE_ALLOCATE,
				(TSTR("object cache \"%s\" %p\n"),
				allocator->object_name,allocator->object_cache));

		else {
			T(YAFFS_TRACE_ALWAYS,
				(TSTR("yaffs cache creation failed\n")));
			YBUG();
		}
	} 
}


yaffs_Tnode *yaffs_AllocateRawTnode(yaffs_Device *dev)
{
	yaffs_Allocator *allocator = dev->allocator;
	if(!allocator || !allocator->tnode_cache){
		YBUG();
		return NULL;
	}
	return kmem_cache_alloc(allocator->tnode_cache, GFP_NOFS);
}

void yaffs_FreeRawTnode(yaffs_Device *dev, yaffs_Tnode *tn)
{
	yaffs_Allocator *allocator = dev->allocator;
	kmem_cache_free(allocator->tnode_cache,tn);
}

yaffs_Object *yaffs_AllocateRawObject(yaffs_Device *dev)
{
	yaffs_Allocator *allocator = dev->allocator;
	if(!allocator){
		YBUG();
		return NULL;
	}
	if(!allocator->object_cache){
		YBUG();
		return NULL;
	}
	return kmem_cache_alloc(allocator->object_cache, GFP_NOFS);
}

void yaffs_FreeRawObject(yaffs_Device *dev, yaffs_Object *obj)
{
	yaffs_Allocator *allocator = dev->allocator;
	kmem_cache_free(allocator->object_cache,obj);
}
