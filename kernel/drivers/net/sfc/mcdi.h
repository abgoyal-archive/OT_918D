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

/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2008-2009 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#ifndef EFX_MCDI_H
#define EFX_MCDI_H

/**
 * enum efx_mcdi_state
 * @MCDI_STATE_QUIESCENT: No pending MCDI requests. If the caller holds the
 *	mcdi_lock then they are able to move to MCDI_STATE_RUNNING
 * @MCDI_STATE_RUNNING: There is an MCDI request pending. Only the thread that
 *	moved into this state is allowed to move out of it.
 * @MCDI_STATE_COMPLETED: An MCDI request has completed, but the owning thread
 *	has not yet consumed the result. For all other threads, equivalent to
 *	MCDI_STATE_RUNNING.
 */
enum efx_mcdi_state {
	MCDI_STATE_QUIESCENT,
	MCDI_STATE_RUNNING,
	MCDI_STATE_COMPLETED,
};

enum efx_mcdi_mode {
	MCDI_MODE_POLL,
	MCDI_MODE_EVENTS,
};

/**
 * struct efx_mcdi_iface
 * @state: Interface state. Waited for by mcdi_wq.
 * @wq: Wait queue for threads waiting for state != STATE_RUNNING
 * @iface_lock: Protects @credits, @seqno, @resprc, @resplen
 * @mode: Poll for mcdi completion, or wait for an mcdi_event.
 *	Serialised by @lock
 * @seqno: The next sequence number to use for mcdi requests.
 *	Serialised by @lock
 * @credits: Number of spurious MCDI completion events allowed before we
 *	trigger a fatal error. Protected by @lock
 * @resprc: Returned MCDI completion
 * @resplen: Returned payload length
 */
struct efx_mcdi_iface {
	atomic_t state;
	wait_queue_head_t wq;
	spinlock_t iface_lock;
	enum efx_mcdi_mode mode;
	unsigned int credits;
	unsigned int seqno;
	unsigned int resprc;
	size_t resplen;
};

extern void efx_mcdi_init(struct efx_nic *efx);

extern int efx_mcdi_rpc(struct efx_nic *efx, unsigned cmd, const u8 *inbuf,
			size_t inlen, u8 *outbuf, size_t outlen,
			size_t *outlen_actual);

extern int efx_mcdi_poll_reboot(struct efx_nic *efx);
extern void efx_mcdi_mode_poll(struct efx_nic *efx);
extern void efx_mcdi_mode_event(struct efx_nic *efx);

extern void efx_mcdi_process_event(struct efx_channel *channel,
				   efx_qword_t *event);

#define MCDI_PTR2(_buf, _ofst)						\
	(((u8 *)_buf) + _ofst)
#define MCDI_SET_DWORD2(_buf, _ofst, _value)				\
	EFX_POPULATE_DWORD_1(*((efx_dword_t *)MCDI_PTR2(_buf, _ofst)),	\
			     EFX_DWORD_0, _value)
#define MCDI_DWORD2(_buf, _ofst)					\
	EFX_DWORD_FIELD(*((efx_dword_t *)MCDI_PTR2(_buf, _ofst)),	\
			EFX_DWORD_0)
#define MCDI_QWORD2(_buf, _ofst)					\
	EFX_QWORD_FIELD64(*((efx_qword_t *)MCDI_PTR2(_buf, _ofst)),	\
			  EFX_QWORD_0)

#define MCDI_PTR(_buf, _ofst)						\
	MCDI_PTR2(_buf, MC_CMD_ ## _ofst ## _OFST)
#define MCDI_SET_DWORD(_buf, _ofst, _value)				\
	MCDI_SET_DWORD2(_buf, MC_CMD_ ## _ofst ## _OFST, _value)
#define MCDI_DWORD(_buf, _ofst)						\
	MCDI_DWORD2(_buf, MC_CMD_ ## _ofst ## _OFST)
#define MCDI_QWORD(_buf, _ofst)						\
	MCDI_QWORD2(_buf, MC_CMD_ ## _ofst ## _OFST)

#define MCDI_EVENT_FIELD(_ev, _field)			\
	EFX_QWORD_FIELD(_ev, MCDI_EVENT_ ## _field)

extern int efx_mcdi_fwver(struct efx_nic *efx, u64 *version, u32 *build);
extern int efx_mcdi_drv_attach(struct efx_nic *efx, bool driver_operating,
			       bool *was_attached_out);
extern int efx_mcdi_get_board_cfg(struct efx_nic *efx, u8 *mac_address,
				  u16 *fw_subtype_list);
extern int efx_mcdi_log_ctrl(struct efx_nic *efx, bool evq, bool uart,
			     u32 dest_evq);
extern int efx_mcdi_nvram_types(struct efx_nic *efx, u32 *nvram_types_out);
extern int efx_mcdi_nvram_info(struct efx_nic *efx, unsigned int type,
			       size_t *size_out, size_t *erase_size_out,
			       bool *protected_out);
extern int efx_mcdi_nvram_update_start(struct efx_nic *efx,
				       unsigned int type);
extern int efx_mcdi_nvram_read(struct efx_nic *efx, unsigned int type,
			       loff_t offset, u8 *buffer, size_t length);
extern int efx_mcdi_nvram_write(struct efx_nic *efx, unsigned int type,
				loff_t offset, const u8 *buffer,
				size_t length);
#define EFX_MCDI_NVRAM_LEN_MAX 128
extern int efx_mcdi_nvram_erase(struct efx_nic *efx, unsigned int type,
				loff_t offset, size_t length);
extern int efx_mcdi_nvram_update_finish(struct efx_nic *efx,
					unsigned int type);
extern int efx_mcdi_nvram_test_all(struct efx_nic *efx);
extern int efx_mcdi_handle_assertion(struct efx_nic *efx);
extern void efx_mcdi_set_id_led(struct efx_nic *efx, enum efx_led_mode mode);
extern int efx_mcdi_reset_port(struct efx_nic *efx);
extern int efx_mcdi_reset_mc(struct efx_nic *efx);
extern int efx_mcdi_wol_filter_set(struct efx_nic *efx, u32 type,
				   const u8 *mac, int *id_out);
extern int efx_mcdi_wol_filter_set_magic(struct efx_nic *efx,
					 const u8 *mac, int *id_out);
extern int efx_mcdi_wol_filter_get_magic(struct efx_nic *efx, int *id_out);
extern int efx_mcdi_wol_filter_remove(struct efx_nic *efx, int id);
extern int efx_mcdi_wol_filter_reset(struct efx_nic *efx);

#endif /* EFX_MCDI_H */
