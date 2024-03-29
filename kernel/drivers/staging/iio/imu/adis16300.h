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

#ifndef SPI_ADIS16300_H_
#define SPI_ADIS16300_H_

#define ADIS16300_STARTUP_DELAY	220 /* ms */

#define ADIS16300_READ_REG(a)    a
#define ADIS16300_WRITE_REG(a) ((a) | 0x80)

#define ADIS16300_FLASH_CNT  0x00 /* Flash memory write count */
#define ADIS16300_SUPPLY_OUT 0x02 /* Power supply measurement */
#define ADIS16300_XGYRO_OUT 0x04 /* X-axis gyroscope output */
#define ADIS16300_XACCL_OUT 0x0A /* X-axis accelerometer output */
#define ADIS16300_YACCL_OUT 0x0C /* Y-axis accelerometer output */
#define ADIS16300_ZACCL_OUT 0x0E /* Z-axis accelerometer output */
#define ADIS16300_TEMP_OUT  0x10 /* Temperature output */
#define ADIS16300_XINCLI_OUT 0x12 /* X-axis inclinometer output measurement */
#define ADIS16300_YINCLI_OUT 0x14 /* Y-axis inclinometer output measurement */
#define ADIS16300_AUX_ADC   0x16 /* Auxiliary ADC measurement */

/* Calibration parameters */
#define ADIS16300_XGYRO_OFF 0x1A /* X-axis gyroscope bias offset factor */
#define ADIS16300_XACCL_OFF 0x20 /* X-axis acceleration bias offset factor */
#define ADIS16300_YACCL_OFF 0x22 /* Y-axis acceleration bias offset factor */
#define ADIS16300_ZACCL_OFF 0x24 /* Z-axis acceleration bias offset factor */

#define ADIS16300_GPIO_CTRL 0x32 /* Auxiliary digital input/output control */
#define ADIS16300_MSC_CTRL  0x34 /* Miscellaneous control */
#define ADIS16300_SMPL_PRD  0x36 /* Internal sample period (rate) control */
#define ADIS16300_SENS_AVG  0x38 /* Dynamic range and digital filter control */
#define ADIS16300_SLP_CNT   0x3A /* Sleep mode control */
#define ADIS16300_DIAG_STAT 0x3C /* System status */

/* Alarm functions */
#define ADIS16300_GLOB_CMD  0x3E /* System command */
#define ADIS16300_ALM_MAG1  0x26 /* Alarm 1 amplitude threshold */
#define ADIS16300_ALM_MAG2  0x28 /* Alarm 2 amplitude threshold */
#define ADIS16300_ALM_SMPL1 0x2A /* Alarm 1 sample size */
#define ADIS16300_ALM_SMPL2 0x2C /* Alarm 2 sample size */
#define ADIS16300_ALM_CTRL  0x2E /* Alarm control */
#define ADIS16300_AUX_DAC   0x30 /* Auxiliary DAC data */

#define ADIS16300_ERROR_ACTIVE			(1<<14)
#define ADIS16300_NEW_DATA			(1<<15)

/* MSC_CTRL */
#define ADIS16300_MSC_CTRL_MEM_TEST		(1<<11)
#define ADIS16300_MSC_CTRL_INT_SELF_TEST	(1<<10)
#define ADIS16300_MSC_CTRL_NEG_SELF_TEST	(1<<9)
#define ADIS16300_MSC_CTRL_POS_SELF_TEST	(1<<8)
#define ADIS16300_MSC_CTRL_GYRO_BIAS		(1<<7)
#define ADIS16300_MSC_CTRL_ACCL_ALIGN		(1<<6)
#define ADIS16300_MSC_CTRL_DATA_RDY_EN		(1<<2)
#define ADIS16300_MSC_CTRL_DATA_RDY_POL_HIGH	(1<<1)
#define ADIS16300_MSC_CTRL_DATA_RDY_DIO2	(1<<0)

/* SMPL_PRD */
#define ADIS16300_SMPL_PRD_TIME_BASE	(1<<7)
#define ADIS16300_SMPL_PRD_DIV_MASK	0x7F

/* DIAG_STAT */
#define ADIS16300_DIAG_STAT_ZACCL_FAIL	(1<<15)
#define ADIS16300_DIAG_STAT_YACCL_FAIL	(1<<14)
#define ADIS16300_DIAG_STAT_XACCL_FAIL	(1<<13)
#define ADIS16300_DIAG_STAT_XGYRO_FAIL	(1<<10)
#define ADIS16300_DIAG_STAT_ALARM2	(1<<9)
#define ADIS16300_DIAG_STAT_ALARM1	(1<<8)
#define ADIS16300_DIAG_STAT_FLASH_CHK	(1<<6)
#define ADIS16300_DIAG_STAT_SELF_TEST	(1<<5)
#define ADIS16300_DIAG_STAT_OVERFLOW	(1<<4)
#define ADIS16300_DIAG_STAT_SPI_FAIL	(1<<3)
#define ADIS16300_DIAG_STAT_FLASH_UPT	(1<<2)
#define ADIS16300_DIAG_STAT_POWER_HIGH	(1<<1)
#define ADIS16300_DIAG_STAT_POWER_LOW	(1<<0)

/* GLOB_CMD */
#define ADIS16300_GLOB_CMD_SW_RESET	(1<<7)
#define ADIS16300_GLOB_CMD_P_AUTO_NULL	(1<<4)
#define ADIS16300_GLOB_CMD_FLASH_UPD	(1<<3)
#define ADIS16300_GLOB_CMD_DAC_LATCH	(1<<2)
#define ADIS16300_GLOB_CMD_FAC_CALIB	(1<<1)
#define ADIS16300_GLOB_CMD_AUTO_NULL	(1<<0)

/* SLP_CNT */
#define ADIS16300_SLP_CNT_POWER_OFF	(1<<8)

#define ADIS16300_MAX_TX 18
#define ADIS16300_MAX_RX 18

#define ADIS16300_SPI_SLOW	(u32)(300 * 1000)
#define ADIS16300_SPI_BURST	(u32)(1000 * 1000)
#define ADIS16300_SPI_FAST	(u32)(2000 * 1000)

/**
 * struct adis16300_state - device instance specific data
 * @us:			actual spi_device
 * @work_trigger_to_ring: bh for triggered event handling
 * @work_cont_thresh: CLEAN
 * @inter:		used to check if new interrupt has been triggered
 * @last_timestamp:	passing timestamp from th to bh of interrupt handler
 * @indio_dev:		industrial I/O device structure
 * @trig:		data ready trigger registered with iio
 * @tx:			transmit buffer
 * @rx:			recieve buffer
 * @buf_lock:		mutex to protect tx and rx
 **/
struct adis16300_state {
	struct spi_device		*us;
	struct work_struct		work_trigger_to_ring;
	struct iio_work_cont		work_cont_thresh;
	s64				last_timestamp;
	struct iio_dev			*indio_dev;
	struct iio_trigger		*trig;
	u8				*tx;
	u8				*rx;
	struct mutex			buf_lock;
};

int adis16300_spi_read_burst(struct device *dev, u8 *rx);

int adis16300_set_irq(struct device *dev, bool enable);

int adis16300_reset(struct device *dev);

int adis16300_check_status(struct device *dev);

#ifdef CONFIG_IIO_RING_BUFFER
/* At the moment triggers are only used for ring buffer
 * filling. This may change!
 */

enum adis16300_scan {
	ADIS16300_SCAN_SUPPLY,
	ADIS16300_SCAN_GYRO_X,
	ADIS16300_SCAN_ACC_X,
	ADIS16300_SCAN_ACC_Y,
	ADIS16300_SCAN_ACC_Z,
	ADIS16300_SCAN_TEMP,
	ADIS16300_SCAN_ADC_0,
	ADIS16300_SCAN_INCLI_X,
	ADIS16300_SCAN_INCLI_Y,
};

void adis16300_remove_trigger(struct iio_dev *indio_dev);
int adis16300_probe_trigger(struct iio_dev *indio_dev);

ssize_t adis16300_read_data_from_ring(struct device *dev,
				      struct device_attribute *attr,
				      char *buf);


int adis16300_configure_ring(struct iio_dev *indio_dev);
void adis16300_unconfigure_ring(struct iio_dev *indio_dev);

int adis16300_initialize_ring(struct iio_ring_buffer *ring);
void adis16300_uninitialize_ring(struct iio_ring_buffer *ring);
#else /* CONFIG_IIO_RING_BUFFER */

static inline void adis16300_remove_trigger(struct iio_dev *indio_dev)
{
}

static inline int adis16300_probe_trigger(struct iio_dev *indio_dev)
{
	return 0;
}

static inline ssize_t
adis16300_read_data_from_ring(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	return 0;
}

static int adis16300_configure_ring(struct iio_dev *indio_dev)
{
	return 0;
}

static inline void adis16300_unconfigure_ring(struct iio_dev *indio_dev)
{
}

static inline int adis16300_initialize_ring(struct iio_ring_buffer *ring)
{
	return 0;
}

static inline void adis16300_uninitialize_ring(struct iio_ring_buffer *ring)
{
}

#endif /* CONFIG_IIO_RING_BUFFER */
#endif /* SPI_ADIS16300_H_ */
