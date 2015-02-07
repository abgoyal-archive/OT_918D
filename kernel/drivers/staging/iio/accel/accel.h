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


#include "../sysfs.h"

/* Accelerometer types of attribute */
#define IIO_DEV_ATTR_ACCEL_OFFSET(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(accel_offset, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_X_OFFSET(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(accel_x_offset, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_Y_OFFSET(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(accel_y_offset, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_Z_OFFSET(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(accel_z_offset, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_X_GAIN(_mode, _show, _store, _addr)		\
	IIO_DEVICE_ATTR(accel_x_gain, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_Y_GAIN(_mode, _show, _store, _addr)		\
	IIO_DEVICE_ATTR(accel_y_gain, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_Z_GAIN(_mode, _show, _store, _addr)		\
	IIO_DEVICE_ATTR(accel_z_gain, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL(_show, _addr)			\
	IIO_DEVICE_ATTR(accel_raw, S_IRUGO, _show, NULL, _addr)

#define IIO_DEV_ATTR_ACCEL_X(_show, _addr)			\
	IIO_DEVICE_ATTR(accel_x_raw, S_IRUGO, _show, NULL, _addr)

#define IIO_DEV_ATTR_ACCEL_Y(_show, _addr)			\
	IIO_DEVICE_ATTR(accel_y_raw, S_IRUGO, _show, NULL, _addr)

#define IIO_DEV_ATTR_ACCEL_Z(_show, _addr)			\
	IIO_DEVICE_ATTR(accel_z_raw, S_IRUGO, _show, NULL, _addr)

/* Thresholds are somewhat chip dependent - may need quite a few defs here */
/* For unified thresholds (shared across all directions */

/**
 * IIO_DEV_ATTR_ACCEL_THRESH: unified threshold
 * @_mode: read/write
 * @_show: read detector threshold value
 * @_store: write detector threshold value
 * @_addr: driver specific data, typically a register address
 *
 * This one is for cases where as single threshold covers all directions
 **/
#define IIO_DEV_ATTR_ACCEL_THRESH(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(thresh, _mode, _show, _store, _addr)

/**
 * IIO_DEV_ATTR_ACCEL_THRESH_X: independant direction threshold, x axis
 * @_mode: readable / writable
 * @_show: read x axis detector threshold value
 * @_store: write x axis detector threshold value
 * @_addr: device driver dependant, typically a register address
 **/
#define IIO_DEV_ATTR_ACCEL_THRESH_X(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(thresh_accel_x, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_THRESH_Y(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(thresh_accel_y, _mode, _show, _store, _addr)

#define IIO_DEV_ATTR_ACCEL_THRESH_Z(_mode, _show, _store, _addr)	\
	IIO_DEVICE_ATTR(thresh_accel_z, _mode, _show, _store, _addr)

/**
 * IIO_EVENT_ATTR_ACCEL_X_HIGH: threshold event, x acceleration
 * @_show: read x acceleration high threshold
 * @_store: write x acceleration high threshold
 * @_mask: device dependant, typically a bit mask
 * @_handler: the iio_handler associated with this attribute
 **/
#define IIO_EVENT_ATTR_ACCEL_X_HIGH(_show, _store, _mask, _handler)	\
	IIO_EVENT_ATTR(accel_x_high, _show, _store, _mask, _handler)

/**
 * IIO_EVENT_ATTR_ACCEL_X_HIGH_SH: threshold event, x accel high, shared handler
 * @_evlist: event list used to share the handler
 * @_show: attribute read
 * @_store: attribute write
 * @_mask: driver specific data, typically a bit mask
 **/
#define IIO_EVENT_ATTR_ACCEL_X_HIGH_SH(_evlist, _show, _store, _mask)	\
	IIO_EVENT_ATTR_SH(accel_x_high, _evlist, _show, _store, _mask)

/**
 * IIO_EVENT_CODE_ACCEL_X_HIGH - event code for x axis high accel threshold
 **/
#define IIO_EVENT_CODE_ACCEL_X_HIGH IIO_EVENT_CODE_ACCEL_BASE

#define IIO_EVENT_ATTR_ACCEL_Y_HIGH(_show, _store, _mask, _handler)	\
	IIO_EVENT_ATTR(accel_y_high, _show, _store, _mask, _handler)

#define IIO_EVENT_ATTR_ACCEL_Y_HIGH_SH(_evlist, _show, _store, _mask)	\
	IIO_EVENT_ATTR_SH(accel_y_high, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Y_HIGH (IIO_EVENT_CODE_ACCEL_BASE + 1)

#define IIO_EVENT_ATTR_ACCEL_Z_HIGH(_show, _store, _mask, _handler)	\
	IIO_EVENT_ATTR(accel_z_high, _show, _store, _mask, _handler)

#define IIO_EVENT_ATTR_ACCEL_Z_HIGH_SH(_evlist, _show, _store, _mask)	\
	IIO_EVENT_ATTR_SH(accel_z_high, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Z_HIGH (IIO_EVENT_CODE_ACCEL_BASE + 2)

#define IIO_EVENT_ATTR_ACCEL_X_LOW(_show, _store, _mask, _handler)	\
	IIO_EVENT_ATTR(accel_x_low, _show, _store, _mask, _handler)

#define IIO_EVENT_ATTR_ACCEL_X_LOW_SH(_evlist, _show, _store, _mask)	\
	IIO_EVENT_ATTR_SH(accel_x_low, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_X_LOW (IIO_EVENT_CODE_ACCEL_BASE + 3)

#define IIO_EVENT_ATTR_ACCEL_Y_LOW(_show, _store, _mask, _handler) \
	IIO_EVENT_ATTR(accel_y_low, _show, _store, _mask, _handler)

#define IIO_EVENT_ATTR_ACCEL_Y_LOW_SH(_evlist, _show, _store, _mask)\
	IIO_EVENT_ATTR_SH(accel_y_low, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Y_LOW (IIO_EVENT_CODE_ACCEL_BASE + 4)

#define IIO_EVENT_ATTR_ACCEL_Z_LOW(_show, _store, _mask, _handler)	\
	IIO_EVENT_ATTR(accel_z_low, _show, _store, _mask, _handler)

#define IIO_EVENT_ATTR_ACCEL_Z_LOW_SH(_evlist, _show, _store, _mask)	\
	IIO_EVENT_ATTR_SH(accel_z_low, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Z_LOW (IIO_EVENT_CODE_ACCEL_BASE + 5)

#define IIO_EVENT_ATTR_FREE_FALL_DETECT(_show, _store, _mask, _handler)	\
	IIO_EVENT_ATTR(free_fall, _show, _store, _mask, _handler)

#define IIO_EVENT_ATTR_FREE_FALL_DETECT_SH(_evlist, _show, _store, _mask) \
	IIO_EVENT_ATTR_SH(free_fall, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_FREE_FALL (IIO_EVENT_CODE_ACCEL_BASE + 6)


#define IIO_EVENT_ATTR_ACCEL_X_ROC_HIGH_SH(_evlist, _show, _store, _mask) \
	IIO_EVENT_ATTR_SH(accel_x_roc_high, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_X_ROC_HIGH (IIO_EVENT_CODE_ACCEL_BASE + 10)

#define IIO_EVENT_ATTR_ACCEL_X_ROC_LOW_SH(_evlist, _show, _store, _mask) \
	IIO_EVENT_ATTR_SH(accel_x_roc_low, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_X_ROC_LOW (IIO_EVENT_CODE_ACCEL_BASE + 11)

#define IIO_EVENT_ATTR_ACCEL_Y_ROC_HIGH_SH(_evlist, _show, _store, _mask) \
	IIO_EVENT_ATTR_SH(accel_y_roc_high, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Y_ROC_HIGH (IIO_EVENT_CODE_ACCEL_BASE + 12)

#define IIO_EVENT_ATTR_ACCEL_Y_ROC_LOW_SH(_evlist, _show, _store, _mask) \
	IIO_EVENT_ATTR_SH(accel_y_roc_low, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Y_ROC_LOW (IIO_EVENT_CODE_ACCEL_BASE + 13)

#define IIO_EVENT_ATTR_ACCEL_Z_ROC_HIGH_SH(_evlist, _show, _store, _mask) \
	IIO_EVENT_ATTR_SH(accel_z_roc_high, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Z_ROC_HIGH (IIO_EVENT_CODE_ACCEL_BASE + 14)

#define IIO_EVENT_ATTR_ACCEL_Z_ROC_LOW_SH(_evlist, _show, _store, _mask) \
	IIO_EVENT_ATTR_SH(accel_z_roc_low, _evlist, _show, _store, _mask)

#define IIO_EVENT_CODE_ACCEL_Z_ROC_LOW (IIO_EVENT_CODE_ACCEL_BASE + 15)
