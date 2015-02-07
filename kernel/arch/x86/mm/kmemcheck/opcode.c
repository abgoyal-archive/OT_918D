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

#include <linux/types.h>

#include "opcode.h"

static bool opcode_is_prefix(uint8_t b)
{
	return
		/* Group 1 */
		b == 0xf0 || b == 0xf2 || b == 0xf3
		/* Group 2 */
		|| b == 0x2e || b == 0x36 || b == 0x3e || b == 0x26
		|| b == 0x64 || b == 0x65 || b == 0x2e || b == 0x3e
		/* Group 3 */
		|| b == 0x66
		/* Group 4 */
		|| b == 0x67;
}

#ifdef CONFIG_X86_64
static bool opcode_is_rex_prefix(uint8_t b)
{
	return (b & 0xf0) == 0x40;
}
#else
static bool opcode_is_rex_prefix(uint8_t b)
{
	return false;
}
#endif

#define REX_W (1 << 3)

/*
 * This is a VERY crude opcode decoder. We only need to find the size of the
 * load/store that caused our #PF and this should work for all the opcodes
 * that we care about. Moreover, the ones who invented this instruction set
 * should be shot.
 */
void kmemcheck_opcode_decode(const uint8_t *op, unsigned int *size)
{
	/* Default operand size */
	int operand_size_override = 4;

	/* prefixes */
	for (; opcode_is_prefix(*op); ++op) {
		if (*op == 0x66)
			operand_size_override = 2;
	}

	/* REX prefix */
	if (opcode_is_rex_prefix(*op)) {
		uint8_t rex = *op;

		++op;
		if (rex & REX_W) {
			switch (*op) {
			case 0x63:
				*size = 4;
				return;
			case 0x0f:
				++op;

				switch (*op) {
				case 0xb6:
				case 0xbe:
					*size = 1;
					return;
				case 0xb7:
				case 0xbf:
					*size = 2;
					return;
				}

				break;
			}

			*size = 8;
			return;
		}
	}

	/* escape opcode */
	if (*op == 0x0f) {
		++op;

		/*
		 * This is move with zero-extend and sign-extend, respectively;
		 * we don't have to think about 0xb6/0xbe, because this is
		 * already handled in the conditional below.
		 */
		if (*op == 0xb7 || *op == 0xbf)
			operand_size_override = 2;
	}

	*size = (*op & 1) ? operand_size_override : 1;
}

const uint8_t *kmemcheck_opcode_get_primary(const uint8_t *op)
{
	/* skip prefixes */
	while (opcode_is_prefix(*op))
		++op;
	if (opcode_is_rex_prefix(*op))
		++op;
	return op;
}
