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

#if !defined(_I915_TRACE_H_) || defined(TRACE_HEADER_MULTI_READ)
#define _I915_TRACE_H_

#include <linux/stringify.h>
#include <linux/types.h>
#include <linux/tracepoint.h>

#include <drm/drmP.h>

#undef TRACE_SYSTEM
#define TRACE_SYSTEM i915
#define TRACE_SYSTEM_STRING __stringify(TRACE_SYSTEM)
#define TRACE_INCLUDE_FILE i915_trace

/* object tracking */

TRACE_EVENT(i915_gem_object_create,

	    TP_PROTO(struct drm_gem_object *obj),

	    TP_ARGS(obj),

	    TP_STRUCT__entry(
			     __field(struct drm_gem_object *, obj)
			     __field(u32, size)
			     ),

	    TP_fast_assign(
			   __entry->obj = obj;
			   __entry->size = obj->size;
			   ),

	    TP_printk("obj=%p, size=%u", __entry->obj, __entry->size)
);

TRACE_EVENT(i915_gem_object_bind,

	    TP_PROTO(struct drm_gem_object *obj, u32 gtt_offset),

	    TP_ARGS(obj, gtt_offset),

	    TP_STRUCT__entry(
			     __field(struct drm_gem_object *, obj)
			     __field(u32, gtt_offset)
			     ),

	    TP_fast_assign(
			   __entry->obj = obj;
			   __entry->gtt_offset = gtt_offset;
			   ),

	    TP_printk("obj=%p, gtt_offset=%08x",
		      __entry->obj, __entry->gtt_offset)
);

TRACE_EVENT(i915_gem_object_change_domain,

	    TP_PROTO(struct drm_gem_object *obj, uint32_t old_read_domains, uint32_t old_write_domain),

	    TP_ARGS(obj, old_read_domains, old_write_domain),

	    TP_STRUCT__entry(
			     __field(struct drm_gem_object *, obj)
			     __field(u32, read_domains)
			     __field(u32, write_domain)
			     ),

	    TP_fast_assign(
			   __entry->obj = obj;
			   __entry->read_domains = obj->read_domains | (old_read_domains << 16);
			   __entry->write_domain = obj->write_domain | (old_write_domain << 16);
			   ),

	    TP_printk("obj=%p, read=%04x, write=%04x",
		      __entry->obj,
		      __entry->read_domains, __entry->write_domain)
);

TRACE_EVENT(i915_gem_object_get_fence,

	    TP_PROTO(struct drm_gem_object *obj, int fence, int tiling_mode),

	    TP_ARGS(obj, fence, tiling_mode),

	    TP_STRUCT__entry(
			     __field(struct drm_gem_object *, obj)
			     __field(int, fence)
			     __field(int, tiling_mode)
			     ),

	    TP_fast_assign(
			   __entry->obj = obj;
			   __entry->fence = fence;
			   __entry->tiling_mode = tiling_mode;
			   ),

	    TP_printk("obj=%p, fence=%d, tiling=%d",
		      __entry->obj, __entry->fence, __entry->tiling_mode)
);

DECLARE_EVENT_CLASS(i915_gem_object,

	    TP_PROTO(struct drm_gem_object *obj),

	    TP_ARGS(obj),

	    TP_STRUCT__entry(
			     __field(struct drm_gem_object *, obj)
			     ),

	    TP_fast_assign(
			   __entry->obj = obj;
			   ),

	    TP_printk("obj=%p", __entry->obj)
);

DEFINE_EVENT(i915_gem_object, i915_gem_object_clflush,

	    TP_PROTO(struct drm_gem_object *obj),

	    TP_ARGS(obj)
);

DEFINE_EVENT(i915_gem_object, i915_gem_object_unbind,

	    TP_PROTO(struct drm_gem_object *obj),

	    TP_ARGS(obj)
);

DEFINE_EVENT(i915_gem_object, i915_gem_object_destroy,

	    TP_PROTO(struct drm_gem_object *obj),

	    TP_ARGS(obj)
);

/* batch tracing */

TRACE_EVENT(i915_gem_request_submit,

	    TP_PROTO(struct drm_device *dev, u32 seqno),

	    TP_ARGS(dev, seqno),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, seqno)
			     ),

	    TP_fast_assign(
			   __entry->dev = dev->primary->index;
			   __entry->seqno = seqno;
			   i915_trace_irq_get(dev, seqno);
			   ),

	    TP_printk("dev=%u, seqno=%u", __entry->dev, __entry->seqno)
);

TRACE_EVENT(i915_gem_request_flush,

	    TP_PROTO(struct drm_device *dev, u32 seqno,
		     u32 flush_domains, u32 invalidate_domains),

	    TP_ARGS(dev, seqno, flush_domains, invalidate_domains),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, seqno)
			     __field(u32, flush_domains)
			     __field(u32, invalidate_domains)
			     ),

	    TP_fast_assign(
			   __entry->dev = dev->primary->index;
			   __entry->seqno = seqno;
			   __entry->flush_domains = flush_domains;
			   __entry->invalidate_domains = invalidate_domains;
			   ),

	    TP_printk("dev=%u, seqno=%u, flush=%04x, invalidate=%04x",
		      __entry->dev, __entry->seqno,
		      __entry->flush_domains, __entry->invalidate_domains)
);

DECLARE_EVENT_CLASS(i915_gem_request,

	    TP_PROTO(struct drm_device *dev, u32 seqno),

	    TP_ARGS(dev, seqno),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, seqno)
			     ),

	    TP_fast_assign(
			   __entry->dev = dev->primary->index;
			   __entry->seqno = seqno;
			   ),

	    TP_printk("dev=%u, seqno=%u", __entry->dev, __entry->seqno)
);

DEFINE_EVENT(i915_gem_request, i915_gem_request_complete,

	    TP_PROTO(struct drm_device *dev, u32 seqno),

	    TP_ARGS(dev, seqno)
);

DEFINE_EVENT(i915_gem_request, i915_gem_request_retire,

	    TP_PROTO(struct drm_device *dev, u32 seqno),

	    TP_ARGS(dev, seqno)
);

DEFINE_EVENT(i915_gem_request, i915_gem_request_wait_begin,

	    TP_PROTO(struct drm_device *dev, u32 seqno),

	    TP_ARGS(dev, seqno)
);

DEFINE_EVENT(i915_gem_request, i915_gem_request_wait_end,

	    TP_PROTO(struct drm_device *dev, u32 seqno),

	    TP_ARGS(dev, seqno)
);

DECLARE_EVENT_CLASS(i915_ring,

	    TP_PROTO(struct drm_device *dev),

	    TP_ARGS(dev),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     ),

	    TP_fast_assign(
			   __entry->dev = dev->primary->index;
			   ),

	    TP_printk("dev=%u", __entry->dev)
);

DEFINE_EVENT(i915_ring, i915_ring_wait_begin,

	    TP_PROTO(struct drm_device *dev),

	    TP_ARGS(dev)
);

DEFINE_EVENT(i915_ring, i915_ring_wait_end,

	    TP_PROTO(struct drm_device *dev),

	    TP_ARGS(dev)
);

#endif /* _I915_TRACE_H_ */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#include <trace/define_trace.h>
