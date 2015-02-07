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

/* prism2mgmt.h
*
* Declares the mgmt command handler functions
*
* Copyright (C) 1999 AbsoluteValue Systems, Inc.  All Rights Reserved.
* --------------------------------------------------------------------
*
* linux-wlan
*
*   The contents of this file are subject to the Mozilla Public
*   License Version 1.1 (the "License"); you may not use this file
*   except in compliance with the License. You may obtain a copy of
*   the License at http://www.mozilla.org/MPL/
*
*   Software distributed under the License is distributed on an "AS
*   IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*   implied. See the License for the specific language governing
*   rights and limitations under the License.
*
*   Alternatively, the contents of this file may be used under the
*   terms of the GNU Public License version 2 (the "GPL"), in which
*   case the provisions of the GPL are applicable instead of the
*   above.  If you wish to allow the use of your version of this file
*   only under the terms of the GPL and not to allow others to use
*   your version of this file under the MPL, indicate your decision
*   by deleting the provisions above and replace them with the notice
*   and other provisions required by the GPL.  If you do not delete
*   the provisions above, a recipient may use your version of this
*   file under either the MPL or the GPL.
*
* --------------------------------------------------------------------
*
* Inquiries regarding the linux-wlan Open Source project can be
* made directly to:
*
* AbsoluteValue Systems Inc.
* info@linux-wlan.com
* http://www.linux-wlan.com
*
* --------------------------------------------------------------------
*
* Portions of the development of this software were funded by
* Intersil Corporation as part of PRISM(R) chipset product development.
*
* --------------------------------------------------------------------
*
* This file contains the constants and data structures for interaction
* with the hfa384x Wireless LAN (WLAN) Media Access Contoller (MAC).
* The hfa384x is a portion of the Harris PRISM(tm) WLAN chipset.
*
* [Implementation and usage notes]
*
* [References]
*   CW10 Programmer's Manual v1.5
*   IEEE 802.11 D10.0
*
*    --------------------------------------------------------------------
*/

#ifndef _PRISM2MGMT_H
#define _PRISM2MGMT_H

extern int prism2_reset_holdtime;
extern int prism2_reset_settletime;

u32 prism2sta_ifstate(wlandevice_t *wlandev, u32 ifstate);

void prism2sta_ev_info(wlandevice_t *wlandev, hfa384x_InfFrame_t *inf);
void prism2sta_ev_txexc(wlandevice_t *wlandev, u16 status);
void prism2sta_ev_tx(wlandevice_t *wlandev, u16 status);
void prism2sta_ev_rx(wlandevice_t *wlandev, struct sk_buff *skb);
void prism2sta_ev_alloc(wlandevice_t *wlandev);

int prism2mgmt_mibset_mibget(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_scan(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_scan_results(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_start(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_wlansniff(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_readpda(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_ramdl_state(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_ramdl_write(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_flashdl_state(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_flashdl_write(wlandevice_t *wlandev, void *msgp);
int prism2mgmt_autojoin(wlandevice_t *wlandev, void *msgp);

/*---------------------------------------------------------------
* conversion functions going between wlan message data types and
* Prism2 data types
---------------------------------------------------------------*/
/* byte area conversion functions*/
void prism2mgmt_pstr2bytearea(u8 *bytearea, p80211pstrd_t *pstr);
void prism2mgmt_bytearea2pstr(u8 *bytearea, p80211pstrd_t *pstr, int len);

/* byte string conversion functions*/
void prism2mgmt_pstr2bytestr(hfa384x_bytestr_t *bytestr, p80211pstrd_t *pstr);
void prism2mgmt_bytestr2pstr(hfa384x_bytestr_t *bytestr, p80211pstrd_t *pstr);

/* functions to convert Group Addresses */
void prism2mgmt_get_grpaddr(u32 did, p80211pstrd_t *pstr, hfa384x_t *priv);
int prism2mgmt_set_grpaddr(u32 did,
			   u8 *prism2buf, p80211pstrd_t *pstr,
			   hfa384x_t *priv);
int prism2mgmt_get_grpaddr_index(u32 did);

void prism2sta_processing_defer(struct work_struct *data);

void prism2sta_commsqual_defer(struct work_struct *data);
void prism2sta_commsqual_timer(unsigned long data);

#endif
