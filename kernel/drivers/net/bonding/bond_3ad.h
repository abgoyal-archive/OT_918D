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
 * Copyright(c) 1999 - 2004 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 */

#ifndef __BOND_3AD_H__
#define __BOND_3AD_H__

#include <asm/byteorder.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>

// General definitions
#define PKT_TYPE_LACPDU         cpu_to_be16(ETH_P_SLOW)
#define AD_TIMER_INTERVAL       100 /*msec*/

#define MULTICAST_LACPDU_ADDR    {0x01, 0x80, 0xC2, 0x00, 0x00, 0x02}

#define AD_LACP_SLOW 0
#define AD_LACP_FAST 1

typedef struct mac_addr {
	u8 mac_addr_value[ETH_ALEN];
} mac_addr_t;

enum {
	BOND_AD_STABLE = 0,
	BOND_AD_BANDWIDTH = 1,
	BOND_AD_COUNT = 2,
};

// rx machine states(43.4.11 in the 802.3ad standard)
typedef enum {
	AD_RX_DUMMY,
	AD_RX_INITIALIZE,     // rx Machine
	AD_RX_PORT_DISABLED,  // rx Machine
	AD_RX_LACP_DISABLED,  // rx Machine
	AD_RX_EXPIRED,	      // rx Machine
	AD_RX_DEFAULTED,      // rx Machine
	AD_RX_CURRENT	      // rx Machine
} rx_states_t;

// periodic machine states(43.4.12 in the 802.3ad standard)
typedef enum {
	AD_PERIODIC_DUMMY,
	AD_NO_PERIODIC,	       // periodic machine
	AD_FAST_PERIODIC,      // periodic machine
	AD_SLOW_PERIODIC,      // periodic machine
	AD_PERIODIC_TX	   // periodic machine
} periodic_states_t;

// mux machine states(43.4.13 in the 802.3ad standard)
typedef enum {
	AD_MUX_DUMMY,
	AD_MUX_DETACHED,       // mux machine
	AD_MUX_WAITING,	       // mux machine
	AD_MUX_ATTACHED,       // mux machine
	AD_MUX_COLLECTING_DISTRIBUTING // mux machine
} mux_states_t;

// tx machine states(43.4.15 in the 802.3ad standard)
typedef enum {
	AD_TX_DUMMY,
	AD_TRANSMIT	   // tx Machine
} tx_states_t;

// rx indication types
typedef enum {
	AD_TYPE_LACPDU = 1,    // type lacpdu
	AD_TYPE_MARKER	   // type marker
} pdu_type_t;

// rx marker indication types
typedef enum {
	AD_MARKER_INFORMATION_SUBTYPE = 1, // marker imformation subtype
	AD_MARKER_RESPONSE_SUBTYPE     // marker response subtype
} bond_marker_subtype_t;

// timers types(43.4.9 in the 802.3ad standard)
typedef enum {
	AD_CURRENT_WHILE_TIMER,
	AD_ACTOR_CHURN_TIMER,
	AD_PERIODIC_TIMER,
	AD_PARTNER_CHURN_TIMER,
	AD_WAIT_WHILE_TIMER
} ad_timers_t;

#pragma pack(1)

// Link Aggregation Control Protocol(LACP) data unit structure(43.4.2.2 in the 802.3ad standard)
typedef struct lacpdu {
	u8 subtype;		     // = LACP(= 0x01)
	u8 version_number;
	u8 tlv_type_actor_info;	      // = actor information(type/length/value)
	u8 actor_information_length; // = 20
	__be16 actor_system_priority;
	struct mac_addr actor_system;
	__be16 actor_key;
	__be16 actor_port_priority;
	__be16 actor_port;
	u8 actor_state;
	u8 reserved_3_1[3];	     // = 0
	u8 tlv_type_partner_info;     // = partner information
	u8 partner_information_length;	 // = 20
	__be16 partner_system_priority;
	struct mac_addr partner_system;
	__be16 partner_key;
	__be16 partner_port_priority;
	__be16 partner_port;
	u8 partner_state;
	u8 reserved_3_2[3];	     // = 0
	u8 tlv_type_collector_info;	  // = collector information
	u8 collector_information_length; // = 16
	__be16 collector_max_delay;
	u8 reserved_12[12];
	u8 tlv_type_terminator;	     // = terminator
	u8 terminator_length;	     // = 0
	u8 reserved_50[50];	     // = 0
} lacpdu_t;

typedef struct lacpdu_header {
	struct ethhdr hdr;
	struct lacpdu lacpdu;
} lacpdu_header_t;

// Marker Protocol Data Unit(PDU) structure(43.5.3.2 in the 802.3ad standard)
typedef struct bond_marker {
	u8 subtype;		 //  = 0x02  (marker PDU)
	u8 version_number;	 //  = 0x01
	u8 tlv_type;		 //  = 0x01  (marker information)
	//  = 0x02  (marker response information)
	u8 marker_length;	 //  = 0x16
	u16 requester_port;	 //   The number assigned to the port by the requester
	struct mac_addr requester_system;      //   The requester's system id
	u32 requester_transaction_id;	//   The transaction id allocated by the requester,
	u16 pad;		 //  = 0
	u8 tlv_type_terminator;	     //  = 0x00
	u8 terminator_length;	     //  = 0x00
	u8 reserved_90[90];	     //  = 0
} bond_marker_t;

typedef struct bond_marker_header {
	struct ethhdr hdr;
	struct bond_marker marker;
} bond_marker_header_t;

#pragma pack()

struct slave;
struct bonding;
struct ad_info;
struct port;

#ifdef __ia64__
#pragma pack(8)
#endif

// aggregator structure(43.4.5 in the 802.3ad standard)
typedef struct aggregator {
	struct mac_addr aggregator_mac_address;
	u16 aggregator_identifier;
	bool is_individual;
	u16 actor_admin_aggregator_key;
	u16 actor_oper_aggregator_key;
	struct mac_addr partner_system;
	u16 partner_system_priority;
	u16 partner_oper_aggregator_key;
	u16 receive_state;		// BOOLEAN
	u16 transmit_state;		// BOOLEAN
	struct port *lag_ports;
	// ****** PRIVATE PARAMETERS ******
	struct slave *slave;	    // pointer to the bond slave that this aggregator belongs to
	u16 is_active;	    // BOOLEAN. Indicates if this aggregator is active
	u16 num_of_ports;
} aggregator_t;

struct port_params {
	struct mac_addr system;
	u16 system_priority;
	u16 key;
	u16 port_number;
	u16 port_priority;
	u16 port_state;
};

// port structure(43.4.6 in the 802.3ad standard)
typedef struct port {
	u16 actor_port_number;
	u16 actor_port_priority;
	struct mac_addr actor_system;	       // This parameter is added here although it is not specified in the standard, just for simplification
	u16 actor_system_priority;	 // This parameter is added here although it is not specified in the standard, just for simplification
	u16 actor_port_aggregator_identifier;
	bool ntt;
	u16 actor_admin_port_key;
	u16 actor_oper_port_key;
	u8 actor_admin_port_state;
	u8 actor_oper_port_state;

	struct port_params partner_admin;
	struct port_params partner_oper;

	bool is_enabled;

	// ****** PRIVATE PARAMETERS ******
	u16 sm_vars;	      // all state machines variables for this port
	rx_states_t sm_rx_state;	// state machine rx state
	u16 sm_rx_timer_counter;    // state machine rx timer counter
	periodic_states_t sm_periodic_state;// state machine periodic state
	u16 sm_periodic_timer_counter;	// state machine periodic timer counter
	mux_states_t sm_mux_state;	// state machine mux state
	u16 sm_mux_timer_counter;   // state machine mux timer counter
	tx_states_t sm_tx_state;	// state machine tx state
	u16 sm_tx_timer_counter;    // state machine tx timer counter(allways on - enter to transmit state 3 time per second)
	struct slave *slave;	    // pointer to the bond slave that this port belongs to
	struct aggregator *aggregator;	   // pointer to an aggregator that this port related to
	struct port *next_port_in_aggregator; // Next port on the linked list of the parent aggregator
	u32 transaction_id;	    // continuous number for identification of Marker PDU's;
	struct lacpdu lacpdu;	       // the lacpdu that will be sent for this port
} port_t;

// system structure
struct ad_system {
	u16 sys_priority;
	struct mac_addr sys_mac_addr;
};

#ifdef __ia64__
#pragma pack()
#endif

// ================= AD Exported structures to the main bonding code ==================
#define BOND_AD_INFO(bond)   ((bond)->ad_info)
#define SLAVE_AD_INFO(slave) ((slave)->ad_info)

struct ad_bond_info {
	struct ad_system system;	    /* 802.3ad system structure */
	u32 agg_select_timer;	    // Timer to select aggregator after all adapter's hand shakes
	u32 agg_select_mode;	    // Mode of selection of active aggregator(bandwidth/count)
	int lacp_fast;		/* whether fast periodic tx should be
				 * requested
				 */
	struct timer_list ad_timer;
	struct packet_type ad_pkt_type;
};

struct ad_slave_info {
	struct aggregator aggregator;	    // 802.3ad aggregator structure
	struct port port;		    // 802.3ad port structure
	spinlock_t rx_machine_lock; // To avoid race condition between callback and receive interrupt
	u16 id;
};

// ================= AD Exported functions to the main bonding code ==================
void bond_3ad_initialize(struct bonding *bond, u16 tick_resolution, int lacp_fast);
int  bond_3ad_bind_slave(struct slave *slave);
void bond_3ad_unbind_slave(struct slave *slave);
void bond_3ad_state_machine_handler(struct work_struct *);
void bond_3ad_initiate_agg_selection(struct bonding *bond, int timeout);
void bond_3ad_adapter_speed_changed(struct slave *slave);
void bond_3ad_adapter_duplex_changed(struct slave *slave);
void bond_3ad_handle_link_change(struct slave *slave, char link);
int  bond_3ad_get_active_agg_info(struct bonding *bond, struct ad_info *ad_info);
int bond_3ad_xmit_xor(struct sk_buff *skb, struct net_device *dev);
int bond_3ad_lacpdu_recv(struct sk_buff *skb, struct net_device *dev, struct packet_type* ptype, struct net_device *orig_dev);
int bond_3ad_set_carrier(struct bonding *bond);
#endif //__BOND_3AD_H__

