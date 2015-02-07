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
 * Copyright (c) 2005-2009 Brocade Communications Systems, Inc.
 * All rights reserved
 * www.brocade.com
 *
 * Linux driver for Brocade Fibre Channel Host Bus Adapter.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (GPL) Version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

/**
 *  bfa_fcb_port.h BFA FCS virtual port driver interfaces
 */

#ifndef __BFA_FCB_PORT_H__
#define __BFA_FCB_PORT_H__

#include <fcb/bfa_fcb_vport.h>
/**
 *  fcs_port_fcb FCS port driver interfaces
 */

/*
 * Forward declarations
 */
struct bfad_port_s;

/*
 * Callback functions from BFA FCS to driver
 */

/**
 * 	Call from FCS to driver module when a port is instantiated. The port
 * 	can be a base port or a virtual port with in the base fabric or
 * 	a virtual fabric.
 *
 *  On this callback, driver is supposed to create scsi_host, scsi_tgt or
 *  network interfaces bases on ports personality/roles.
 *
 *      base port of base fabric:	vf_drv == NULL && vp_drv == NULL
 *      vport of base fabric:		vf_drv == NULL && vp_drv != NULL
 *      base port of VF:		vf_drv != NULL && vp_drv == NULL
 *      vport of VF:			vf_drv != NULL && vp_drv != NULL
 *
 * @param[in] bfad   - driver instance
 * @param[in] port   - FCS port instance
 * @param[in] roles  - port roles: IM, TM, IP
 * @param[in] vf_drv - VF driver instance, NULL if base fabric (no VF)
 * @param[in] vp_drv - vport driver instance, NULL if base port
 *
 * @return None
 */
struct bfad_port_s *bfa_fcb_port_new(struct bfad_s *bfad,
			struct bfa_fcs_port_s *port,
			enum bfa_port_role roles, struct bfad_vf_s *vf_drv,
			struct bfad_vport_s *vp_drv);

/**
 * 	Call from FCS to driver module when a port is deleted. The port
 * 	can be a base port or a virtual port with in the base fabric or
 * 	a virtual fabric.
 *
 * @param[in] bfad   - driver instance
 * @param[in] roles  - port roles: IM, TM, IP
 * @param[in] vf_drv - VF driver instance, NULL if base fabric (no VF)
 * @param[in] vp_drv - vport driver instance, NULL if base port
 *
 * @return None
 */
void bfa_fcb_port_delete(struct bfad_s *bfad, enum bfa_port_role roles,
			struct bfad_vf_s *vf_drv, struct bfad_vport_s *vp_drv);

/**
 * 	Notification when port transitions to ONLINE state.
 *
 * Online notification is a logical link up for the local port. This
 * notification is sent after a successfull FLOGI, or a successful
 * link initialization in proviate-loop or N2N topologies.
 *
 * @param[in] bfad   - driver instance
 * @param[in] roles  - port roles: IM, TM, IP
 * @param[in] vf_drv - VF driver instance, NULL if base fabric (no VF)
 * @param[in] vp_drv - vport driver instance, NULL if base port
 *
 * @return None
 */
void bfa_fcb_port_online(struct bfad_s *bfad, enum bfa_port_role roles,
			struct bfad_vf_s *vf_drv, struct bfad_vport_s *vp_drv);

/**
 * 	Notification when port transitions to OFFLINE state.
 *
 * Offline notification is a logical link down for the local port.
 *
 * @param[in] bfad   - driver instance
 * @param[in] roles  - port roles: IM, TM, IP
 * @param[in] vf_drv - VF driver instance, NULL if base fabric (no VF)
 * @param[in] vp_drv - vport driver instance, NULL if base port
 *
 * @return None
 */
void bfa_fcb_port_offline(struct bfad_s *bfad, enum bfa_port_role roles,
			struct bfad_vf_s *vf_drv, struct bfad_vport_s *vp_drv);


#endif /* __BFA_FCB_PORT_H__ */
