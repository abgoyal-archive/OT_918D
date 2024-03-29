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
 *  Copyright (C) 2003 PMC-Sierra Inc.
 *  Author: Manish Lachwani (lachwani@pmc-sierra.com)
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Description:
 *
 * This code reads the ATMEL 24CXX EEPROM. The PMC-Sierra Yosemite board uses the ATMEL
 * 24C32/24C64 which uses two byte addressing as compared to 24C16. Note that this program
 * uses the serial port like /dev/ttyS0, to communicate with the EEPROM. Hence, you are
 * expected to have a connectivity from the EEPROM to the serial port. This program does
 * __not__ communicate using the I2C protocol
 */

#include "atmel_read_eeprom.h"

static void delay(int delay)
{
	while (delay--);
}

static void send_bit(unsigned char bit)
{
	scl_lo;
	delay(TXX);
	if (bit)
		sda_hi;
	else
		sda_lo;

	delay(TXX);
	scl_hi;
	delay(TXX);
}

static void send_ack(void)
{
	send_bit(0);
}

static void send_byte(unsigned char byte)
{
	int	i = 0;

	for (i = 7; i >= 0; i--)
		send_bit((byte >> i) & 0x01);
}

static void send_start(void)
{
	sda_hi;
	delay(TXX);
	scl_hi;
	delay(TXX);
	sda_lo;
	delay(TXX);
}

static void send_stop(void)
{
	sda_lo;
	delay(TXX);
	scl_hi;
	delay(TXX);
	sda_hi;
	delay(TXX);
}

static void do_idle(void)
{
	sda_hi;
	scl_hi;
	vcc_off;
}

static int recv_bit(void)
{
	int	status;

	scl_lo;
	delay(TXX);
	sda_hi;
	delay(TXX);
	scl_hi;
	delay(TXX);

	return 1;
}

static unsigned char recv_byte(void) {
        int i;
        unsigned char byte=0;

        for (i=7;i>=0;i--)
                byte |= (recv_bit() << i);

        return byte;
}

static int recv_ack(void)
{
	unsigned int	ack;

	ack = (unsigned int)recv_bit();
	scl_lo;

	if (ack) {
		do_idle();
		printk(KERN_ERR "Error reading the Atmel 24C32/24C64 EEPROM\n");
		return -1;
	}

	return ack;
}

/*
 * This function does the actual read of the EEPROM. It needs the buffer into which the
 * read data is copied, the size of the EEPROM being read and the buffer size
 */
int read_eeprom(char *buffer, int eeprom_size, int size)
{
	int	i = 0, err;

	send_start();
	send_byte(W_HEADER);
	recv_ack();

	/* EEPROM with size of more than 2K need two byte addressing */
	if (eeprom_size > 2048) {
		send_byte(0x00);
		recv_ack();
	}

	send_start();
	send_byte(R_HEADER);
	err = recv_ack();
	if (err == -1)
		return err;

	for (i = 0; i < size; i++) {
		*buffer++ = recv_byte();
		send_ack();
	}

	/* Note : We should do some check if the buffer contains correct information */

	send_stop();
}
