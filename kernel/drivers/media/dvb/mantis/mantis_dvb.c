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

#include <linux/kernel.h>
#include <linux/bitops.h>

#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/i2c.h>

#include "dmxdev.h"
#include "dvbdev.h"
#include "dvb_demux.h"
#include "dvb_frontend.h"
#include "dvb_net.h"

#include "mantis_common.h"
#include "mantis_dma.h"
#include "mantis_ca.h"
#include "mantis_ioc.h"
#include "mantis_dvb.h"

DVB_DEFINE_MOD_OPT_ADAPTER_NR(adapter_nr);

int mantis_frontend_power(struct mantis_pci *mantis, enum mantis_power power)
{
	struct mantis_hwconfig *config = mantis->hwconfig;

	switch (power) {
	case POWER_ON:
		dprintk(MANTIS_DEBUG, 1, "Power ON");
		gpio_set_bits(mantis, config->power, POWER_ON);
		msleep(100);
		gpio_set_bits(mantis, config->power, POWER_ON);
		msleep(100);
		break;

	case POWER_OFF:
		dprintk(MANTIS_DEBUG, 1, "Power OFF");
		gpio_set_bits(mantis, config->power, POWER_OFF);
		msleep(100);
		break;

	default:
		dprintk(MANTIS_DEBUG, 1, "Unknown state <%02x>", power);
		return -1;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(mantis_frontend_power);

void mantis_frontend_soft_reset(struct mantis_pci *mantis)
{
	struct mantis_hwconfig *config = mantis->hwconfig;

	dprintk(MANTIS_DEBUG, 1, "Frontend RESET");
	gpio_set_bits(mantis, config->reset, 0);
	msleep(100);
	gpio_set_bits(mantis, config->reset, 0);
	msleep(100);
	gpio_set_bits(mantis, config->reset, 1);
	msleep(100);
	gpio_set_bits(mantis, config->reset, 1);
	msleep(100);

	return;
}
EXPORT_SYMBOL_GPL(mantis_frontend_soft_reset);

static int mantis_frontend_shutdown(struct mantis_pci *mantis)
{
	int err;

	mantis_frontend_soft_reset(mantis);
	err = mantis_frontend_power(mantis, POWER_OFF);
	if (err != 0) {
		dprintk(MANTIS_ERROR, 1, "Frontend POWER OFF failed! <%d>", err);
		return 1;
	}

	return 0;
}

static int mantis_dvb_start_feed(struct dvb_demux_feed *dvbdmxfeed)
{
	struct dvb_demux *dvbdmx = dvbdmxfeed->demux;
	struct mantis_pci *mantis = dvbdmx->priv;

	dprintk(MANTIS_DEBUG, 1, "Mantis DVB Start feed");
	if (!dvbdmx->dmx.frontend) {
		dprintk(MANTIS_DEBUG, 1, "no frontend ?");
		return -EINVAL;
	}

	mantis->feeds++;
	dprintk(MANTIS_DEBUG, 1, "mantis start feed, feeds=%d",	mantis->feeds);

	if (mantis->feeds == 1)	 {
		dprintk(MANTIS_DEBUG, 1, "mantis start feed & dma");
		mantis_dma_start(mantis);
	}

	return mantis->feeds;
}

static int mantis_dvb_stop_feed(struct dvb_demux_feed *dvbdmxfeed)
{
	struct dvb_demux *dvbdmx = dvbdmxfeed->demux;
	struct mantis_pci *mantis = dvbdmx->priv;

	dprintk(MANTIS_DEBUG, 1, "Mantis DVB Stop feed");
	if (!dvbdmx->dmx.frontend) {
		dprintk(MANTIS_DEBUG, 1, "no frontend ?");
		return -EINVAL;
	}

	mantis->feeds--;
	if (mantis->feeds == 0) {
		dprintk(MANTIS_DEBUG, 1, "mantis stop feed and dma");
		mantis_dma_stop(mantis);
	}

	return 0;
}

int __devinit mantis_dvb_init(struct mantis_pci *mantis)
{
	struct mantis_hwconfig *config = mantis->hwconfig;
	int result = -1;

	dprintk(MANTIS_DEBUG, 1, "dvb_register_adapter");

	result = dvb_register_adapter(&mantis->dvb_adapter,
				      "Mantis DVB adapter",
				      THIS_MODULE,
				      &mantis->pdev->dev,
				      adapter_nr);

	if (result < 0) {

		dprintk(MANTIS_ERROR, 1, "Error registering adapter");
		return -ENODEV;
	}

	mantis->dvb_adapter.priv	= mantis;
	mantis->demux.dmx.capabilities	= DMX_TS_FILTERING	|
					 DMX_SECTION_FILTERING	|
					 DMX_MEMORY_BASED_FILTERING;

	mantis->demux.priv		= mantis;
	mantis->demux.filternum		= 256;
	mantis->demux.feednum		= 256;
	mantis->demux.start_feed	= mantis_dvb_start_feed;
	mantis->demux.stop_feed		= mantis_dvb_stop_feed;
	mantis->demux.write_to_decoder	= NULL;

	dprintk(MANTIS_DEBUG, 1, "dvb_dmx_init");
	result = dvb_dmx_init(&mantis->demux);
	if (result < 0) {
		dprintk(MANTIS_ERROR, 1, "dvb_dmx_init failed, ERROR=%d", result);

		goto err0;
	}

	mantis->dmxdev.filternum	= 256;
	mantis->dmxdev.demux		= &mantis->demux.dmx;
	mantis->dmxdev.capabilities	= 0;
	dprintk(MANTIS_DEBUG, 1, "dvb_dmxdev_init");

	result = dvb_dmxdev_init(&mantis->dmxdev, &mantis->dvb_adapter);
	if (result < 0) {

		dprintk(MANTIS_ERROR, 1, "dvb_dmxdev_init failed, ERROR=%d", result);
		goto err1;
	}

	mantis->fe_hw.source		= DMX_FRONTEND_0;
	result = mantis->demux.dmx.add_frontend(&mantis->demux.dmx, &mantis->fe_hw);
	if (result < 0) {

		dprintk(MANTIS_ERROR, 1, "dvb_dmx_init failed, ERROR=%d", result);
		goto err2;
	}

	mantis->fe_mem.source		= DMX_MEMORY_FE;
	result = mantis->demux.dmx.add_frontend(&mantis->demux.dmx, &mantis->fe_mem);
	if (result < 0) {
		dprintk(MANTIS_ERROR, 1, "dvb_dmx_init failed, ERROR=%d", result);
		goto err3;
	}

	result = mantis->demux.dmx.connect_frontend(&mantis->demux.dmx, &mantis->fe_hw);
	if (result < 0) {
		dprintk(MANTIS_ERROR, 1, "dvb_dmx_init failed, ERROR=%d", result);
		goto err4;
	}

	dvb_net_init(&mantis->dvb_adapter, &mantis->dvbnet, &mantis->demux.dmx);
	tasklet_init(&mantis->tasklet, mantis_dma_xfer, (unsigned long) mantis);
	if (mantis->hwconfig) {
		result = config->frontend_init(mantis, mantis->fe);
		if (result < 0) {
			dprintk(MANTIS_ERROR, 1, "!!! NO Frontends found !!!");
			goto err5;
		} else {
			if (mantis->fe == NULL) {
				dprintk(MANTIS_ERROR, 1, "FE <NULL>");
				goto err5;
			}

			if (dvb_register_frontend(&mantis->dvb_adapter, mantis->fe)) {
				dprintk(MANTIS_ERROR, 1, "ERROR: Frontend registration failed");

				if (mantis->fe->ops.release)
					mantis->fe->ops.release(mantis->fe);

				mantis->fe = NULL;
				goto err5;
			}
		}
	}

	return 0;

	/* Error conditions ..	*/
err5:
	tasklet_kill(&mantis->tasklet);
	dvb_net_release(&mantis->dvbnet);
	dvb_unregister_frontend(mantis->fe);
	dvb_frontend_detach(mantis->fe);
err4:
	mantis->demux.dmx.remove_frontend(&mantis->demux.dmx, &mantis->fe_mem);

err3:
	mantis->demux.dmx.remove_frontend(&mantis->demux.dmx, &mantis->fe_hw);

err2:
	dvb_dmxdev_release(&mantis->dmxdev);

err1:
	dvb_dmx_release(&mantis->demux);

err0:
	dvb_unregister_adapter(&mantis->dvb_adapter);

	return result;
}
EXPORT_SYMBOL_GPL(mantis_dvb_init);

int __devexit mantis_dvb_exit(struct mantis_pci *mantis)
{
	int err;

	if (mantis->fe) {
		/* mantis_ca_exit(mantis); */
		err = mantis_frontend_shutdown(mantis);
		if (err != 0)
			dprintk(MANTIS_ERROR, 1, "Frontend exit while POWER ON! <%d>", err);
		dvb_unregister_frontend(mantis->fe);
		dvb_frontend_detach(mantis->fe);
	}

	tasklet_kill(&mantis->tasklet);
	dvb_net_release(&mantis->dvbnet);

	mantis->demux.dmx.remove_frontend(&mantis->demux.dmx, &mantis->fe_mem);
	mantis->demux.dmx.remove_frontend(&mantis->demux.dmx, &mantis->fe_hw);

	dvb_dmxdev_release(&mantis->dmxdev);
	dvb_dmx_release(&mantis->demux);

	dprintk(MANTIS_DEBUG, 1, "dvb_unregister_adapter");
	dvb_unregister_adapter(&mantis->dvb_adapter);

	return 0;
}
EXPORT_SYMBOL_GPL(mantis_dvb_exit);
