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
	Mantis PCI bridge driver

	Copyright (C) Manu Abraham (abraham.manu@gmail.com)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <linux/input.h>
#include <media/ir-common.h>
#include <linux/pci.h>

#include "dmxdev.h"
#include "dvbdev.h"
#include "dvb_demux.h"
#include "dvb_frontend.h"
#include "dvb_net.h"

#include "mantis_common.h"
#include "mantis_reg.h"
#include "mantis_uart.h"

#define MODULE_NAME "mantis_core"

static struct ir_scancode mantis_ir_table[] = {
	{ 0x29, KEY_POWER	},
	{ 0x28, KEY_FAVORITES	},
	{ 0x30, KEY_TEXT	},
	{ 0x17, KEY_INFO	}, /* Preview */
	{ 0x23, KEY_EPG		},
	{ 0x3b, KEY_F22		}, /* Record List */
	{ 0x3c, KEY_1		},
	{ 0x3e, KEY_2		},
	{ 0x39, KEY_3		},
	{ 0x36, KEY_4		},
	{ 0x22, KEY_5		},
	{ 0x20, KEY_6		},
	{ 0x32, KEY_7		},
	{ 0x26, KEY_8		},
	{ 0x24, KEY_9		},
	{ 0x2a, KEY_0		},

	{ 0x33, KEY_CANCEL	},
	{ 0x2c, KEY_BACK	},
	{ 0x15, KEY_CLEAR	},
	{ 0x3f, KEY_TAB		},
	{ 0x10, KEY_ENTER	},
	{ 0x14, KEY_UP		},
	{ 0x0d, KEY_RIGHT	},
	{ 0x0e, KEY_DOWN	},
	{ 0x11, KEY_LEFT	},

	{ 0x21, KEY_VOLUMEUP	},
	{ 0x35, KEY_VOLUMEDOWN	},
	{ 0x3d, KEY_CHANNELDOWN	},
	{ 0x3a, KEY_CHANNELUP	},
	{ 0x2e, KEY_RECORD	},
	{ 0x2b, KEY_PLAY	},
	{ 0x13, KEY_PAUSE	},
	{ 0x25, KEY_STOP	},

	{ 0x1f, KEY_REWIND	},
	{ 0x2d, KEY_FASTFORWARD	},
	{ 0x1e, KEY_PREVIOUS	}, /* Replay |< */
	{ 0x1d, KEY_NEXT	}, /* Skip   >| */

	{ 0x0b, KEY_CAMERA	}, /* Capture */
	{ 0x0f, KEY_LANGUAGE	}, /* SAP */
	{ 0x18, KEY_MODE	}, /* PIP */
	{ 0x12, KEY_ZOOM	}, /* Full screen */
	{ 0x1c, KEY_SUBTITLE	},
	{ 0x2f, KEY_MUTE	},
	{ 0x16, KEY_F20		}, /* L/R */
	{ 0x38, KEY_F21		}, /* Hibernate */

	{ 0x37, KEY_SWITCHVIDEOMODE }, /* A/V */
	{ 0x31, KEY_AGAIN	}, /* Recall */
	{ 0x1a, KEY_KPPLUS	}, /* Zoom+ */
	{ 0x19, KEY_KPMINUS	}, /* Zoom- */
	{ 0x27, KEY_RED		},
	{ 0x0C, KEY_GREEN	},
	{ 0x01, KEY_YELLOW	},
	{ 0x00, KEY_BLUE	},
};

struct ir_scancode_table ir_mantis = {
	.scan = mantis_ir_table,
	.size = ARRAY_SIZE(mantis_ir_table),
};
EXPORT_SYMBOL_GPL(ir_mantis);

int mantis_input_init(struct mantis_pci *mantis)
{
	struct input_dev *rc;
	struct ir_input_state rc_state;
	char name[80], dev[80];
	int err;

	rc = input_allocate_device();
	if (!rc) {
		dprintk(MANTIS_ERROR, 1, "Input device allocate failed");
		return -ENOMEM;
	}

	sprintf(name, "Mantis %s IR receiver", mantis->hwconfig->model_name);
	sprintf(dev, "pci-%s/ir0", pci_name(mantis->pdev));

	rc->name = name;
	rc->phys = dev;

	ir_input_init(rc, &rc_state, IR_TYPE_OTHER);

	rc->id.bustype	= BUS_PCI;
	rc->id.vendor	= mantis->vendor_id;
	rc->id.product	= mantis->device_id;
	rc->id.version	= 1;
	rc->dev		= mantis->pdev->dev;

	err = __ir_input_register(rc, &ir_mantis, NULL, MODULE_NAME);
	if (err) {
		dprintk(MANTIS_ERROR, 1, "IR device registration failed, ret = %d", err);
		input_free_device(rc);
		return -ENODEV;
	}

	mantis->rc = rc;

	return 0;
}

int mantis_exit(struct mantis_pci *mantis)
{
	struct input_dev *rc = mantis->rc;

	ir_input_unregister(rc);

	return 0;
}
