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

#define DTLK_MINOR	0
#define DTLK_IO_EXTENT	0x02

	/* ioctl's use magic number of 0xa3 */
#define DTLK_INTERROGATE 0xa390	/* get settings from the DoubleTalk */
#define DTLK_STATUS 0xa391	/* get status from the DoubleTalk */


#define DTLK_CLEAR 0x18		/* stops speech */

#define DTLK_MAX_RETRIES (loops_per_jiffy/(10000/HZ))

	/* TTS Port Status Flags */
#define TTS_READABLE     0x80	/* mask for bit which is nonzero if a
				   byte can be read from the TTS port */
#define TTS_SPEAKING     0x40	/* mask for SYNC bit, which is nonzero
				   while DoubleTalk is producing
				   output with TTS, PCM or CVSD
				   synthesizers or tone generators
				   (that is, all but LPC) */
#define TTS_SPEAKING2    0x20	/* mask for SYNC2 bit,
				   which falls to zero up to 0.4 sec
				   before speech stops */
#define TTS_WRITABLE     0x10	/* mask for RDY bit, which when set to
             			   1, indicates the TTS port is ready
             			   to accept a byte of data.  The RDY
             			   bit goes zero 2-3 usec after
             			   writing, and goes 1 again 180-190
             			   usec later. */
#define TTS_ALMOST_FULL  0x08	/* mask for AF bit: When set to 1,
				   indicates that less than 300 free
				   bytes are available in the TTS
				   input buffer. AF is always 0 in the
				   PCM, TGN and CVSD modes. */
#define TTS_ALMOST_EMPTY 0x04	/* mask for AE bit: When set to 1,
				   indicates that less than 300 bytes
				   of data remain in DoubleTalk's
				   input (TTS or PCM) buffer. AE is
				   always 1 in the TGN and CVSD
				   modes. */

	/* LPC speak commands */
#define LPC_5220_NORMAL 0x60	/* 5220 format decoding table, normal rate */
#define LPC_5220_FAST 0x64	/* 5220 format decoding table, fast rate */
#define LPC_D6_NORMAL 0x20	/* D6 format decoding table, normal rate */
#define LPC_D6_FAST 0x24	/* D6 format decoding table, fast rate */

	/* LPC Port Status Flags (valid only after one of the LPC
           speak commands) */
#define LPC_SPEAKING     0x80	/* mask for TS bit: When set to 1,
				   indicates the LPC synthesizer is
				   producing speech.*/
#define LPC_BUFFER_LOW   0x40	/* mask for BL bit: When set to 1,
				   indicates that the hardware LPC
				   data buffer has less than 30 bytes
				   remaining. (Total internal buffer
				   size = 4096 bytes.) */
#define LPC_BUFFER_EMPTY 0x20	/* mask for BE bit: When set to 1,
				   indicates that the LPC data buffer
				   ran out of data (error condition if
				   TS is also 1).  */

				/* data returned by Interrogate command */
struct dtlk_settings
{
  unsigned short serial_number;	/* 0-7Fh:0-7Fh */
  unsigned char rom_version[24]; /* null terminated string */
  unsigned char mode;		/* 0=Character; 1=Phoneme; 2=Text */
  unsigned char punc_level;	/* nB; 0-7 */
  unsigned char formant_freq;	/* nF; 0-9 */
  unsigned char pitch;		/* nP; 0-99 */
  unsigned char speed;		/* nS; 0-9 */
  unsigned char volume;		/* nV; 0-9 */
  unsigned char tone;		/* nX; 0-2 */
  unsigned char expression;	/* nE; 0-9 */
  unsigned char ext_dict_loaded; /* 1=exception dictionary loaded */
  unsigned char ext_dict_status; /* 1=exception dictionary enabled */
  unsigned char free_ram;	/* # pages (truncated) remaining for
                                   text buffer */
  unsigned char articulation;	/* nA; 0-9 */
  unsigned char reverb;		/* nR; 0-9 */
  unsigned char eob;		/* 7Fh value indicating end of
                                   parameter block */
  unsigned char has_indexing;	/* nonzero if indexing is implemented */
};
