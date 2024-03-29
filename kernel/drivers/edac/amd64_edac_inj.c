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

#include "amd64_edac.h"

static ssize_t amd64_inject_section_show(struct mem_ctl_info *mci, char *buf)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	return sprintf(buf, "0x%x\n", pvt->injection.section);
}

/*
 * store error injection section value which refers to one of 4 16-byte sections
 * within a 64-byte cacheline
 *
 * range: 0..3
 */
static ssize_t amd64_inject_section_store(struct mem_ctl_info *mci,
					  const char *data, size_t count)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	unsigned long value;
	int ret = 0;

	ret = strict_strtoul(data, 10, &value);
	if (ret != -EINVAL) {

		if (value > 3) {
			amd64_printk(KERN_WARNING,
				     "%s: invalid section 0x%lx\n",
				     __func__, value);
			return -EINVAL;
		}

		pvt->injection.section = (u32) value;
		return count;
	}
	return ret;
}

static ssize_t amd64_inject_word_show(struct mem_ctl_info *mci, char *buf)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	return sprintf(buf, "0x%x\n", pvt->injection.word);
}

/*
 * store error injection word value which refers to one of 9 16-bit word of the
 * 16-byte (128-bit + ECC bits) section
 *
 * range: 0..8
 */
static ssize_t amd64_inject_word_store(struct mem_ctl_info *mci,
					const char *data, size_t count)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	unsigned long value;
	int ret = 0;

	ret = strict_strtoul(data, 10, &value);
	if (ret != -EINVAL) {

		if (value > 8) {
			amd64_printk(KERN_WARNING,
				     "%s: invalid word 0x%lx\n",
				     __func__, value);
			return -EINVAL;
		}

		pvt->injection.word = (u32) value;
		return count;
	}
	return ret;
}

static ssize_t amd64_inject_ecc_vector_show(struct mem_ctl_info *mci, char *buf)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	return sprintf(buf, "0x%x\n", pvt->injection.bit_map);
}

/*
 * store 16 bit error injection vector which enables injecting errors to the
 * corresponding bit within the error injection word above. When used during a
 * DRAM ECC read, it holds the contents of the of the DRAM ECC bits.
 */
static ssize_t amd64_inject_ecc_vector_store(struct mem_ctl_info *mci,
					     const char *data, size_t count)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	unsigned long value;
	int ret = 0;

	ret = strict_strtoul(data, 16, &value);
	if (ret != -EINVAL) {

		if (value & 0xFFFF0000) {
			amd64_printk(KERN_WARNING,
				     "%s: invalid EccVector: 0x%lx\n",
				     __func__, value);
			return -EINVAL;
		}

		pvt->injection.bit_map = (u32) value;
		return count;
	}
	return ret;
}

/*
 * Do a DRAM ECC read. Assemble staged values in the pvt area, format into
 * fields needed by the injection registers and read the NB Array Data Port.
 */
static ssize_t amd64_inject_read_store(struct mem_ctl_info *mci,
					const char *data, size_t count)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	unsigned long value;
	u32 section, word_bits;
	int ret = 0;

	ret = strict_strtoul(data, 10, &value);
	if (ret != -EINVAL) {

		/* Form value to choose 16-byte section of cacheline */
		section = F10_NB_ARRAY_DRAM_ECC |
				SET_NB_ARRAY_ADDRESS(pvt->injection.section);
		pci_write_config_dword(pvt->misc_f3_ctl,
					F10_NB_ARRAY_ADDR, section);

		word_bits = SET_NB_DRAM_INJECTION_READ(pvt->injection.word,
						pvt->injection.bit_map);

		/* Issue 'word' and 'bit' along with the READ request */
		pci_write_config_dword(pvt->misc_f3_ctl,
					F10_NB_ARRAY_DATA, word_bits);

		debugf0("section=0x%x word_bits=0x%x\n", section, word_bits);

		return count;
	}
	return ret;
}

/*
 * Do a DRAM ECC write. Assemble staged values in the pvt area and format into
 * fields needed by the injection registers.
 */
static ssize_t amd64_inject_write_store(struct mem_ctl_info *mci,
					const char *data, size_t count)
{
	struct amd64_pvt *pvt = mci->pvt_info;
	unsigned long value;
	u32 section, word_bits;
	int ret = 0;

	ret = strict_strtoul(data, 10, &value);
	if (ret != -EINVAL) {

		/* Form value to choose 16-byte section of cacheline */
		section = F10_NB_ARRAY_DRAM_ECC |
				SET_NB_ARRAY_ADDRESS(pvt->injection.section);
		pci_write_config_dword(pvt->misc_f3_ctl,
					F10_NB_ARRAY_ADDR, section);

		word_bits = SET_NB_DRAM_INJECTION_WRITE(pvt->injection.word,
						pvt->injection.bit_map);

		/* Issue 'word' and 'bit' along with the READ request */
		pci_write_config_dword(pvt->misc_f3_ctl,
					F10_NB_ARRAY_DATA, word_bits);

		debugf0("section=0x%x word_bits=0x%x\n", section, word_bits);

		return count;
	}
	return ret;
}

/*
 * update NUM_INJ_ATTRS in case you add new members
 */
struct mcidev_sysfs_attribute amd64_inj_attrs[] = {

	{
		.attr = {
			.name = "inject_section",
			.mode = (S_IRUGO | S_IWUSR)
		},
		.show = amd64_inject_section_show,
		.store = amd64_inject_section_store,
	},
	{
		.attr = {
			.name = "inject_word",
			.mode = (S_IRUGO | S_IWUSR)
		},
		.show = amd64_inject_word_show,
		.store = amd64_inject_word_store,
	},
	{
		.attr = {
			.name = "inject_ecc_vector",
			.mode = (S_IRUGO | S_IWUSR)
		},
		.show = amd64_inject_ecc_vector_show,
		.store = amd64_inject_ecc_vector_store,
	},
	{
		.attr = {
			.name = "inject_write",
			.mode = (S_IRUGO | S_IWUSR)
		},
		.show = NULL,
		.store = amd64_inject_write_store,
	},
	{
		.attr = {
			.name = "inject_read",
			.mode = (S_IRUGO | S_IWUSR)
		},
		.show = NULL,
		.store = amd64_inject_read_store,
	},
};
