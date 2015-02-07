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

#ifndef __VP_PRIV_H__
#define __VP_PRIV_H__

#include <fcs/bfa_fcs_lport.h>
#include <fcs/bfa_fcs_vport.h>

/*
 * Functions exported by vps
 */
void            bfa_fcs_vport_init(struct bfa_fcs_vport_s *vport);

/*
 * Functions exported by vps
 */
void            bfa_fcs_vps_online(struct bfa_fcs_port_s *port);
void            bfa_fcs_vps_offline(struct bfa_fcs_port_s *port);
void            bfa_fcs_vps_lip(struct bfa_fcs_port_s *port);

/*
 * Functions exported by port_fab
 */
void            bfa_fcs_port_fab_init(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_fab_online(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_fab_offline(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_fab_rx_frame(struct bfa_fcs_port_s *port,
					  u8 *rx_frame, u32 len);

/*
 * Functions exported by VP-NS.
 */
void            bfa_fcs_port_ns_init(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_ns_offline(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_ns_online(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_ns_query(struct bfa_fcs_port_s *port);

/*
 * Functions exported by VP-SCN
 */
void            bfa_fcs_port_scn_init(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_scn_offline(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_scn_online(struct bfa_fcs_port_s *vport);
void            bfa_fcs_port_scn_process_rscn(struct bfa_fcs_port_s *port,
					      struct fchs_s *rx_frame, u32 len);

/*
 * Functions exported by VP-N2N
 */

void            bfa_fcs_port_n2n_init(struct bfa_fcs_port_s *port);
void            bfa_fcs_port_n2n_online(struct bfa_fcs_port_s *port);
void            bfa_fcs_port_n2n_offline(struct bfa_fcs_port_s *port);
void            bfa_fcs_port_n2n_rx_frame(struct bfa_fcs_port_s *port,
					  u8 *rx_frame, u32 len);

/*
 * Functions exported by VP-LOOP
 */
void            bfa_fcs_port_loop_init(struct bfa_fcs_port_s *port);
void            bfa_fcs_port_loop_online(struct bfa_fcs_port_s *port);
void            bfa_fcs_port_loop_offline(struct bfa_fcs_port_s *port);
void            bfa_fcs_port_loop_lip(struct bfa_fcs_port_s *port);
void            bfa_fcs_port_loop_rx_frame(struct bfa_fcs_port_s *port,
					   u8 *rx_frame, u32 len);

#endif /* __VP_PRIV_H__ */
