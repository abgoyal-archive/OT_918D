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
 *   fs/cifs/ntlmssp.h
 *
 *   Copyright (c) International Business Machines  Corp., 2002,2007
 *   Author(s): Steve French (sfrench@us.ibm.com)
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#define NTLMSSP_SIGNATURE "NTLMSSP"
/* Message Types */
#define NtLmNegotiate     cpu_to_le32(1)
#define NtLmChallenge     cpu_to_le32(2)
#define NtLmAuthenticate  cpu_to_le32(3)
#define UnknownMessage    cpu_to_le32(8)

/* Negotiate Flags */
#define NTLMSSP_NEGOTIATE_UNICODE         0x01 /* Text strings are unicode */
#define NTLMSSP_NEGOTIATE_OEM             0x02 /* Text strings are in OEM */
#define NTLMSSP_REQUEST_TARGET            0x04 /* Srv returns its auth realm */
/* define reserved9                       0x08 */
#define NTLMSSP_NEGOTIATE_SIGN          0x0010 /* Request signing capability */
#define NTLMSSP_NEGOTIATE_SEAL          0x0020 /* Request confidentiality */
#define NTLMSSP_NEGOTIATE_DGRAM         0x0040
#define NTLMSSP_NEGOTIATE_LM_KEY        0x0080 /* Use LM session key */
/* defined reserved 8                   0x0100 */
#define NTLMSSP_NEGOTIATE_NTLM          0x0200 /* NTLM authentication */
#define NTLMSSP_NEGOTIATE_NT_ONLY       0x0400 /* Lanman not allowed */
#define NTLMSSP_ANONYMOUS               0x0800
#define NTLMSSP_NEGOTIATE_DOMAIN_SUPPLIED 0x1000 /* reserved6 */
#define NTLMSSP_NEGOTIATE_WORKSTATION_SUPPLIED 0x2000
#define NTLMSSP_NEGOTIATE_LOCAL_CALL    0x4000 /* client/server same machine */
#define NTLMSSP_NEGOTIATE_ALWAYS_SIGN   0x8000 /* Sign. All security levels  */
#define NTLMSSP_TARGET_TYPE_DOMAIN     0x10000
#define NTLMSSP_TARGET_TYPE_SERVER     0x20000
#define NTLMSSP_TARGET_TYPE_SHARE      0x40000
#define NTLMSSP_NEGOTIATE_EXTENDED_SEC 0x80000 /* NB:not related to NTLMv2 pwd*/
/* #define NTLMSSP_REQUEST_INIT_RESP     0x100000 */
#define NTLMSSP_NEGOTIATE_IDENTIFY    0x100000
#define NTLMSSP_REQUEST_ACCEPT_RESP   0x200000 /* reserved5 */
#define NTLMSSP_REQUEST_NON_NT_KEY    0x400000
#define NTLMSSP_NEGOTIATE_TARGET_INFO 0x800000
/* #define reserved4                 0x1000000 */
#define NTLMSSP_NEGOTIATE_VERSION    0x2000000 /* we do not set */
/* #define reserved3                 0x4000000 */
/* #define reserved2                 0x8000000 */
/* #define reserved1                0x10000000 */
#define NTLMSSP_NEGOTIATE_128       0x20000000
#define NTLMSSP_NEGOTIATE_KEY_XCH   0x40000000
#define NTLMSSP_NEGOTIATE_56        0x80000000

/* Although typedefs are not commonly used for structure definitions */
/* in the Linux kernel, in this particular case they are useful      */
/* to more closely match the standards document for NTLMSSP from     */
/* OpenGroup and to make the code more closely match the standard in */
/* appearance */

typedef struct _SECURITY_BUFFER {
	__le16 Length;
	__le16 MaximumLength;
	__le32 BufferOffset;	/* offset to buffer */
} __attribute__((packed)) SECURITY_BUFFER;

typedef struct _NEGOTIATE_MESSAGE {
	__u8 Signature[sizeof(NTLMSSP_SIGNATURE)];
	__le32 MessageType;     /* NtLmNegotiate = 1 */
	__le32 NegotiateFlags;
	SECURITY_BUFFER DomainName;	/* RFC 1001 style and ASCII */
	SECURITY_BUFFER WorkstationName;	/* RFC 1001 and ASCII */
	/* SECURITY_BUFFER for version info not present since we
	   do not set the version is present flag */
	char DomainString[0];
	/* followed by WorkstationString */
} __attribute__((packed)) NEGOTIATE_MESSAGE, *PNEGOTIATE_MESSAGE;

typedef struct _CHALLENGE_MESSAGE {
	__u8 Signature[sizeof(NTLMSSP_SIGNATURE)];
	__le32 MessageType;   /* NtLmChallenge = 2 */
	SECURITY_BUFFER TargetName;
	__le32 NegotiateFlags;
	__u8 Challenge[CIFS_CRYPTO_KEY_SIZE];
	__u8 Reserved[8];
	SECURITY_BUFFER TargetInfoArray;
	/* SECURITY_BUFFER for version info not present since we
	   do not set the version is present flag */
} __attribute__((packed)) CHALLENGE_MESSAGE, *PCHALLENGE_MESSAGE;

typedef struct _AUTHENTICATE_MESSAGE {
	__u8 Signature[sizeof(NTLMSSP_SIGNATURE)];
	__le32 MessageType;  /* NtLmsAuthenticate = 3 */
	SECURITY_BUFFER LmChallengeResponse;
	SECURITY_BUFFER NtChallengeResponse;
	SECURITY_BUFFER DomainName;
	SECURITY_BUFFER UserName;
	SECURITY_BUFFER WorkstationName;
	SECURITY_BUFFER SessionKey;
	__le32 NegotiateFlags;
	/* SECURITY_BUFFER for version info not present since we
	   do not set the version is present flag */
	char UserString[0];
} __attribute__((packed)) AUTHENTICATE_MESSAGE, *PAUTHENTICATE_MESSAGE;
