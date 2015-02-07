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

#ifndef _OSDEF_H_
#define _OSDEF_H_

/* #define WINCE_HEADER*/
/*#define WIN2000*/
/* #define TC */
#define LINUX_KERNEL
/* #define LINUX_XF86 */

/**********************************************************************/
#ifdef LINUX_KERNEL
//#include <linux/config.h>
#endif


/**********************************************************************/
#ifdef TC
#endif
#ifdef WIN2000
#endif
#ifdef WINCE_HEADER
#endif
#ifdef LINUX_XF86
#define LINUX
#endif
#ifdef LINUX_KERNEL
#define LINUX
#endif

/**********************************************************************/
#ifdef TC
#define XGI_SetMemory(MemoryAddress,MemorySize,value) memset(MemoryAddress, value, MemorySize);
#endif
#ifdef WIN2000
#define XGI_SetMemory(MemoryAddress,MemorySize,value) MemFill((PVOID) MemoryAddress,(ULONG) MemorySize,(UCHAR) value);
#endif
#ifdef WINCE_HEADER
#define XGI_SetMemory(MemoryAddress,MemorySize,value) memset(MemoryAddress, value, MemorySize);
#endif
#ifdef LINUX_XF86
#define XGI_SetMemory(MemoryAddress,MemorySize,value) memset(MemoryAddress, value, MemorySize)
#endif
#ifdef LINUX_KERNEL
#define XGI_SetMemory(MemoryAddress,MemorySize,value) memset(MemoryAddress, value, MemorySize)
#endif
/**********************************************************************/

/**********************************************************************/

#ifdef TC
#define XGI_MemoryCopy(Destination,Soruce,Length) memmove(Destination, Soruce, Length);
#endif
#ifdef WIN2000
#define XGI_MemoryCopy(Destination,Soruce,Length)  /*VideoPortMoveMemory((PUCHAR)Destination , Soruce,length);*/
#endif
#ifdef WINCE_HEADER
#define XGI_MemoryCopy(Destination,Soruce,Length) memmove(Destination, Soruce, Length);
#endif
#ifdef LINUX_XF86
#define XGI_MemoryCopy(Destination,Soruce,Length) memcpy(Destination,Soruce,Length)
#endif
#ifdef LINUX_KERNEL
#define XGI_MemoryCopy(Destination,Soruce,Length) memcpy(Destination,Soruce,Length)
#endif

/**********************************************************************/

#ifdef OutPortByte
#undef OutPortByte
#endif /* OutPortByte */

#ifdef OutPortWord
#undef OutPortWord
#endif /* OutPortWord */

#ifdef OutPortLong
#undef OutPortLong
#endif /* OutPortLong */

#ifdef InPortByte
#undef InPortByte
#endif /* InPortByte */

#ifdef InPortWord
#undef InPortWord
#endif /* InPortWord */

#ifdef InPortLong
#undef InPortLong
#endif /* InPortLong */

/**********************************************************************/
/*  TC                                                                */
/**********************************************************************/

#ifdef TC
#define OutPortByte(p,v) outp((unsigned short)(p),(unsigned char)(v))
#define OutPortWord(p,v) outp((unsigned short)(p),(unsigned short)(v))
#define OutPortLong(p,v) outp((unsigned short)(p),(unsigned long)(v))
#define InPortByte(p)    inp((unsigned short)(p))
#define InPortWord(p)    inp((unsigned short)(p))
#define InPortLong(p)    ((inp((unsigned short)(p+2))<<16) | inp((unsigned short)(p)))
#endif

/**********************************************************************/
/*  LINUX XF86                                                        */
/**********************************************************************/

#ifdef LINUX_XF86
#define OutPortByte(p,v) outb((CARD16)(p),(CARD8)(v))
#define OutPortWord(p,v) outw((CARD16)(p),(CARD16)(v))
#define OutPortLong(p,v) outl((CARD16)(p),(CARD32)(v))
#define InPortByte(p)    inb((CARD16)(p))
#define InPortWord(p)    inw((CARD16)(p))
#define InPortLong(p)    inl((CARD16)(p))
#endif

#ifdef LINUX_KERNEL
#define OutPortByte(p,v) outb((u8)(v),(p))
#define OutPortWord(p,v) outw((u16)(v),(p))
#define OutPortLong(p,v) outl((u32)(v),(p))
#define InPortByte(p)    inb(p)
#define InPortWord(p)    inw(p)
#define InPortLong(p)    inl(p)
#endif

/**********************************************************************/
/*  WIN 2000                                                          */
/**********************************************************************/

#ifdef WIN2000
#define OutPortByte(p,v) VideoPortWritePortUchar ((PUCHAR) (p), (UCHAR) (v))
#define OutPortWord(p,v) VideoPortWritePortUshort((PUSHORT) (p), (USHORT) (v))
#define OutPortLong(p,v) VideoPortWritePortUlong ((PULONG) (p), (ULONG) (v))
#define InPortByte(p)    VideoPortReadPortUchar  ((PUCHAR) (p))
#define InPortWord(p)    VideoPortReadPortUshort ((PUSHORT) (p))
#define InPortLong(p)    VideoPortReadPortUlong  ((PULONG) (p))
#endif


/**********************************************************************/
/*  WIN CE                                                          */
/**********************************************************************/

#ifdef WINCE_HEADER
#define OutPortByte(p,v) WRITE_PORT_UCHAR ((PUCHAR) (p), (UCHAR) (v))
#define OutPortWord(p,v) WRITE_PORT_USHORT((PUSHORT) (p), (USHORT) (v))
#define OutPortLong(p,v) WRITE_PORT_ULONG ((PULONG) (p), (ULONG) (v))
#define InPortByte(p)    READ_PORT_UCHAR  ((PUCHAR) (p))
#define InPortWord(p)    READ_PORT_USHORT ((PUSHORT) (p))
#define InPortLong(p)    READ_PORT_ULONG  ((PULONG) (p))
#endif
#endif // _OSDEF_H_
