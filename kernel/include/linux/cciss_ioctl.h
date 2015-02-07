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

#ifndef CCISS_IOCTLH
#define CCISS_IOCTLH

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/cciss_defs.h>

#define CCISS_IOC_MAGIC 'B'


typedef struct _cciss_pci_info_struct
{
	unsigned char 	bus;
	unsigned char 	dev_fn;
	unsigned short	domain;
	__u32 		board_id;
} cciss_pci_info_struct; 

typedef struct _cciss_coalint_struct
{
	__u32  delay;
	__u32  count;
} cciss_coalint_struct;

typedef char NodeName_type[16];

typedef __u32 Heartbeat_type;

#define CISS_PARSCSIU2 	0x0001
#define CISS_PARCSCIU3 	0x0002
#define CISS_FIBRE1G	0x0100
#define CISS_FIBRE2G	0x0200
typedef __u32 BusTypes_type;

typedef char FirmwareVer_type[4];
typedef __u32 DriverVer_type;

#define MAX_KMALLOC_SIZE 128000

typedef struct _IOCTL_Command_struct {
  LUNAddr_struct	   LUN_info;
  RequestBlock_struct      Request;
  ErrorInfo_struct  	   error_info; 
  WORD			   buf_size;  /* size in bytes of the buf */
  BYTE			   __user *buf;
} IOCTL_Command_struct;

typedef struct _BIG_IOCTL_Command_struct {
  LUNAddr_struct	   LUN_info;
  RequestBlock_struct      Request;
  ErrorInfo_struct  	   error_info;
  DWORD			   malloc_size; /* < MAX_KMALLOC_SIZE in cciss.c */
  DWORD			   buf_size;    /* size in bytes of the buf */
  				        /* < malloc_size * MAXSGENTRIES */
  BYTE			   __user *buf;
} BIG_IOCTL_Command_struct;

typedef struct _LogvolInfo_struct{
	__u32	LunID;
	int	num_opens;  /* number of opens on the logical volume */
	int	num_parts;  /* number of partitions configured on logvol */
} LogvolInfo_struct;

#define CCISS_GETPCIINFO _IOR(CCISS_IOC_MAGIC, 1, cciss_pci_info_struct)

#define CCISS_GETINTINFO _IOR(CCISS_IOC_MAGIC, 2, cciss_coalint_struct)
#define CCISS_SETINTINFO _IOW(CCISS_IOC_MAGIC, 3, cciss_coalint_struct)

#define CCISS_GETNODENAME _IOR(CCISS_IOC_MAGIC, 4, NodeName_type)
#define CCISS_SETNODENAME _IOW(CCISS_IOC_MAGIC, 5, NodeName_type)

#define CCISS_GETHEARTBEAT _IOR(CCISS_IOC_MAGIC, 6, Heartbeat_type)
#define CCISS_GETBUSTYPES  _IOR(CCISS_IOC_MAGIC, 7, BusTypes_type)
#define CCISS_GETFIRMVER   _IOR(CCISS_IOC_MAGIC, 8, FirmwareVer_type)
#define CCISS_GETDRIVVER   _IOR(CCISS_IOC_MAGIC, 9, DriverVer_type)
#define CCISS_REVALIDVOLS  _IO(CCISS_IOC_MAGIC, 10)
#define CCISS_PASSTHRU	   _IOWR(CCISS_IOC_MAGIC, 11, IOCTL_Command_struct)
#define CCISS_DEREGDISK	   _IO(CCISS_IOC_MAGIC, 12)

/* no longer used... use REGNEWD instead */ 
#define CCISS_REGNEWDISK  _IOW(CCISS_IOC_MAGIC, 13, int)

#define CCISS_REGNEWD	   _IO(CCISS_IOC_MAGIC, 14)
#define CCISS_RESCANDISK   _IO(CCISS_IOC_MAGIC, 16)
#define CCISS_GETLUNINFO   _IOR(CCISS_IOC_MAGIC, 17, LogvolInfo_struct)
#define CCISS_BIG_PASSTHRU _IOWR(CCISS_IOC_MAGIC, 18, BIG_IOCTL_Command_struct)

#ifdef __KERNEL__
#ifdef CONFIG_COMPAT

/* 32 bit compatible ioctl structs */
typedef struct _IOCTL32_Command_struct {
  LUNAddr_struct	   LUN_info;
  RequestBlock_struct      Request;
  ErrorInfo_struct  	   error_info;
  WORD			   buf_size;  /* size in bytes of the buf */
  __u32			   buf; /* 32 bit pointer to data buffer */
} IOCTL32_Command_struct;

typedef struct _BIG_IOCTL32_Command_struct {
  LUNAddr_struct	   LUN_info;
  RequestBlock_struct      Request;
  ErrorInfo_struct  	   error_info;
  DWORD			   malloc_size; /* < MAX_KMALLOC_SIZE in cciss.c */
  DWORD			   buf_size;    /* size in bytes of the buf */
  				        /* < malloc_size * MAXSGENTRIES */
  __u32 		buf;	/* 32 bit pointer to data buffer */
} BIG_IOCTL32_Command_struct;

#define CCISS_PASSTHRU32   _IOWR(CCISS_IOC_MAGIC, 11, IOCTL32_Command_struct)
#define CCISS_BIG_PASSTHRU32 _IOWR(CCISS_IOC_MAGIC, 18, BIG_IOCTL32_Command_struct)

#endif /* CONFIG_COMPAT */
#endif /* __KERNEL__ */
#endif  
