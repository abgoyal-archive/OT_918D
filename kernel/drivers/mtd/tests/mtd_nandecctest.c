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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/random.h>
#include <linux/string.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <linux/mtd/nand_ecc.h>

#if defined(CONFIG_MTD_NAND) || defined(CONFIG_MTD_NAND_MODULE)

static void inject_single_bit_error(void *data, size_t size)
{
	unsigned long offset = random32() % (size * BITS_PER_BYTE);

	__change_bit(offset, data);
}

static unsigned char data[512];
static unsigned char error_data[512];

static int nand_ecc_test(const size_t size)
{
	unsigned char code[3];
	unsigned char error_code[3];
	char testname[30];

	BUG_ON(sizeof(data) < size);

	sprintf(testname, "nand-ecc-%zu", size);

	get_random_bytes(data, size);

	memcpy(error_data, data, size);
	inject_single_bit_error(error_data, size);

	__nand_calculate_ecc(data, size, code);
	__nand_calculate_ecc(error_data, size, error_code);
	__nand_correct_data(error_data, code, error_code, size);

	if (!memcmp(data, error_data, size)) {
		printk(KERN_INFO "mtd_nandecctest: ok - %s\n", testname);
		return 0;
	}

	printk(KERN_ERR "mtd_nandecctest: not ok - %s\n", testname);

	printk(KERN_DEBUG "hexdump of data:\n");
	print_hex_dump(KERN_DEBUG, "", DUMP_PREFIX_OFFSET, 16, 4,
			data, size, false);
	printk(KERN_DEBUG "hexdump of error data:\n");
	print_hex_dump(KERN_DEBUG, "", DUMP_PREFIX_OFFSET, 16, 4,
			error_data, size, false);

	return -1;
}

#else

static int nand_ecc_test(const size_t size)
{
	return 0;
}

#endif

static int __init ecc_test_init(void)
{
	srandom32(jiffies);

	nand_ecc_test(256);
	nand_ecc_test(512);

	return 0;
}

static void __exit ecc_test_exit(void)
{
}

module_init(ecc_test_init);
module_exit(ecc_test_exit);

MODULE_DESCRIPTION("NAND ECC function test module");
MODULE_AUTHOR("Akinobu Mita");
MODULE_LICENSE("GPL");
