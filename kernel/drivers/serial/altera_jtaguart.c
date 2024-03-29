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
 * altera_jtaguart.c -- Altera JTAG UART driver
 *
 * Based on mcf.c -- Freescale ColdFire UART driver
 *
 * (C) Copyright 2003-2007, Greg Ungerer <gerg@snapgear.com>
 * (C) Copyright 2008, Thomas Chou <thomas@wytron.com.tw>
 * (C) Copyright 2010, Tobias Klauser <tklauser@distanz.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/altera_jtaguart.h>

#define DRV_NAME "altera_jtaguart"

/*
 * Altera JTAG UART register definitions according to the Altera JTAG UART
 * datasheet: http://www.altera.com/literature/hb/nios2/n2cpu_nii51009.pdf
 */

#define ALTERA_JTAGUART_SIZE			8

#define ALTERA_JTAGUART_DATA_REG		0

#define ALTERA_JTAGUART_DATA_DATA_MSK		0x000000FF
#define ALTERA_JTAGUART_DATA_RVALID_MSK		0x00008000
#define ALTERA_JTAGUART_DATA_RAVAIL_MSK		0xFFFF0000
#define ALTERA_JTAGUART_DATA_RAVAIL_OFF		16

#define ALTERA_JTAGUART_CONTROL_REG		4

#define ALTERA_JTAGUART_CONTROL_RE_MSK		0x00000001
#define ALTERA_JTAGUART_CONTROL_WE_MSK		0x00000002
#define ALTERA_JTAGUART_CONTROL_RI_MSK		0x00000100
#define ALTERA_JTAGUART_CONTROL_RI_OFF		8
#define ALTERA_JTAGUART_CONTROL_WI_MSK		0x00000200
#define ALTERA_JTAGUART_CONTROL_AC_MSK		0x00000400
#define ALTERA_JTAGUART_CONTROL_WSPACE_MSK	0xFFFF0000
#define ALTERA_JTAGUART_CONTROL_WSPACE_OFF	16

/*
 * Local per-uart structure.
 */
struct altera_jtaguart {
	struct uart_port port;
	unsigned int sigs;	/* Local copy of line sigs */
	unsigned long imr;	/* Local IMR mirror */
};

static unsigned int altera_jtaguart_tx_empty(struct uart_port *port)
{
	return (readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) &
		ALTERA_JTAGUART_CONTROL_WSPACE_MSK) ? TIOCSER_TEMT : 0;
}

static unsigned int altera_jtaguart_get_mctrl(struct uart_port *port)
{
	return TIOCM_CAR | TIOCM_DSR | TIOCM_CTS;
}

static void altera_jtaguart_set_mctrl(struct uart_port *port, unsigned int sigs)
{
}

static void altera_jtaguart_start_tx(struct uart_port *port)
{
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);

	pp->imr |= ALTERA_JTAGUART_CONTROL_WE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
}

static void altera_jtaguart_stop_tx(struct uart_port *port)
{
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);

	pp->imr &= ~ALTERA_JTAGUART_CONTROL_WE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
}

static void altera_jtaguart_stop_rx(struct uart_port *port)
{
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);

	pp->imr &= ~ALTERA_JTAGUART_CONTROL_RE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
}

static void altera_jtaguart_break_ctl(struct uart_port *port, int break_state)
{
}

static void altera_jtaguart_enable_ms(struct uart_port *port)
{
}

static void altera_jtaguart_set_termios(struct uart_port *port,
					struct ktermios *termios,
					struct ktermios *old)
{
	/* Just copy the old termios settings back */
	if (old)
		tty_termios_copy_hw(termios, old);
}

static void altera_jtaguart_rx_chars(struct altera_jtaguart *pp)
{
	struct uart_port *port = &pp->port;
	unsigned char ch, flag;
	unsigned long status;

	while ((status = readl(port->membase + ALTERA_JTAGUART_DATA_REG)) &
	       ALTERA_JTAGUART_DATA_RVALID_MSK) {
		ch = status & ALTERA_JTAGUART_DATA_DATA_MSK;
		flag = TTY_NORMAL;
		port->icount.rx++;

		if (uart_handle_sysrq_char(port, ch))
			continue;
		uart_insert_char(port, 0, 0, ch, flag);
	}

	tty_flip_buffer_push(port->state->port.tty);
}

static void altera_jtaguart_tx_chars(struct altera_jtaguart *pp)
{
	struct uart_port *port = &pp->port;
	struct circ_buf *xmit = &port->state->xmit;
	unsigned int pending, count;

	if (port->x_char) {
		/* Send special char - probably flow control */
		writel(port->x_char, port->membase + ALTERA_JTAGUART_DATA_REG);
		port->x_char = 0;
		port->icount.tx++;
		return;
	}

	pending = uart_circ_chars_pending(xmit);
	if (pending > 0) {
		count = (readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) &
				ALTERA_JTAGUART_CONTROL_WSPACE_MSK) >>
			ALTERA_JTAGUART_CONTROL_WSPACE_OFF;
		if (count > pending)
			count = pending;
		if (count > 0) {
			pending -= count;
			while (count--) {
				writel(xmit->buf[xmit->tail],
				       port->membase + ALTERA_JTAGUART_DATA_REG);
				xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
				port->icount.tx++;
			}
			if (pending < WAKEUP_CHARS)
				uart_write_wakeup(port);
		}
	}

	if (pending == 0) {
		pp->imr &= ~ALTERA_JTAGUART_CONTROL_WE_MSK;
		writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
	}
}

static irqreturn_t altera_jtaguart_interrupt(int irq, void *data)
{
	struct uart_port *port = data;
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);
	unsigned int isr;

	isr = (readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) >>
	       ALTERA_JTAGUART_CONTROL_RI_OFF) & pp->imr;

	spin_lock(&port->lock);

	if (isr & ALTERA_JTAGUART_CONTROL_RE_MSK)
		altera_jtaguart_rx_chars(pp);
	if (isr & ALTERA_JTAGUART_CONTROL_WE_MSK)
		altera_jtaguart_tx_chars(pp);

	spin_unlock(&port->lock);

	return IRQ_RETVAL(isr);
}

static void altera_jtaguart_config_port(struct uart_port *port, int flags)
{
	port->type = PORT_ALTERA_JTAGUART;

	/* Clear mask, so no surprise interrupts. */
	writel(0, port->membase + ALTERA_JTAGUART_CONTROL_REG);
}

static int altera_jtaguart_startup(struct uart_port *port)
{
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);
	unsigned long flags;
	int ret;

	ret = request_irq(port->irq, altera_jtaguart_interrupt, IRQF_DISABLED,
			DRV_NAME, port);
	if (ret) {
		pr_err(DRV_NAME ": unable to attach Altera JTAG UART %d "
		       "interrupt vector=%d\n", port->line, port->irq);
		return ret;
	}

	spin_lock_irqsave(&port->lock, flags);

	/* Enable RX interrupts now */
	pp->imr = ALTERA_JTAGUART_CONTROL_RE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);

	spin_unlock_irqrestore(&port->lock, flags);

	return 0;
}

static void altera_jtaguart_shutdown(struct uart_port *port)
{
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);

	/* Disable all interrupts now */
	pp->imr = 0;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);

	spin_unlock_irqrestore(&port->lock, flags);

	free_irq(port->irq, port);
}

static const char *altera_jtaguart_type(struct uart_port *port)
{
	return (port->type == PORT_ALTERA_JTAGUART) ? "Altera JTAG UART" : NULL;
}

static int altera_jtaguart_request_port(struct uart_port *port)
{
	/* UARTs always present */
	return 0;
}

static void altera_jtaguart_release_port(struct uart_port *port)
{
	/* Nothing to release... */
}

static int altera_jtaguart_verify_port(struct uart_port *port,
				       struct serial_struct *ser)
{
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_ALTERA_JTAGUART)
		return -EINVAL;
	return 0;
}

/*
 *	Define the basic serial functions we support.
 */
static struct uart_ops altera_jtaguart_ops = {
	.tx_empty	= altera_jtaguart_tx_empty,
	.get_mctrl	= altera_jtaguart_get_mctrl,
	.set_mctrl	= altera_jtaguart_set_mctrl,
	.start_tx	= altera_jtaguart_start_tx,
	.stop_tx	= altera_jtaguart_stop_tx,
	.stop_rx	= altera_jtaguart_stop_rx,
	.enable_ms	= altera_jtaguart_enable_ms,
	.break_ctl	= altera_jtaguart_break_ctl,
	.startup	= altera_jtaguart_startup,
	.shutdown	= altera_jtaguart_shutdown,
	.set_termios	= altera_jtaguart_set_termios,
	.type		= altera_jtaguart_type,
	.request_port	= altera_jtaguart_request_port,
	.release_port	= altera_jtaguart_release_port,
	.config_port	= altera_jtaguart_config_port,
	.verify_port	= altera_jtaguart_verify_port,
};

#define ALTERA_JTAGUART_MAXPORTS 1
static struct altera_jtaguart altera_jtaguart_ports[ALTERA_JTAGUART_MAXPORTS];

#if defined(CONFIG_SERIAL_ALTERA_JTAGUART_CONSOLE)

int __init early_altera_jtaguart_setup(struct altera_jtaguart_platform_uart
				       *platp)
{
	struct uart_port *port;
	int i;

	for (i = 0; i < ALTERA_JTAGUART_MAXPORTS && platp[i].mapbase; i++) {
		port = &altera_jtaguart_ports[i].port;

		port->line = i;
		port->type = PORT_ALTERA_JTAGUART;
		port->mapbase = platp[i].mapbase;
		port->membase = ioremap(port->mapbase, ALTERA_JTAGUART_SIZE);
		port->iotype = SERIAL_IO_MEM;
		port->irq = platp[i].irq;
		port->flags = ASYNC_BOOT_AUTOCONF;
		port->ops = &altera_jtaguart_ops;
	}

	return 0;
}

#if defined(CONFIG_SERIAL_ALTERA_JTAGUART_CONSOLE_BYPASS)
static void altera_jtaguart_console_putc(struct console *co, const char c)
{
	struct uart_port *port = &(altera_jtaguart_ports + co->index)->port;
	unsigned long status;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	while (((status = readl(port->membase + ALTERA_JTAGUART_CONTROL_REG)) &
		ALTERA_JTAGUART_CONTROL_WSPACE_MSK) == 0) {
		if ((status & ALTERA_JTAGUART_CONTROL_AC_MSK) == 0) {
			spin_unlock_irqrestore(&port->lock, flags);
			return;	/* no connection activity */
		}
		spin_unlock_irqrestore(&port->lock, flags);
		cpu_relax();
		spin_lock_irqsave(&port->lock, flags);
	}
	writel(c, port->membase + ALTERA_JTAGUART_DATA_REG);
	spin_unlock_irqrestore(&port->lock, flags);
}
#else
static void altera_jtaguart_console_putc(struct console *co, const char c)
{
	struct uart_port *port = &(altera_jtaguart_ports + co->index)->port;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	while ((readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) &
		ALTERA_JTAGUART_CONTROL_WSPACE_MSK) == 0) {
		spin_unlock_irqrestore(&port->lock, flags);
		cpu_relax();
		spin_lock_irqsave(&port->lock, flags);
	}
	writel(c, port->membase + ALTERA_JTAGUART_DATA_REG);
	spin_unlock_irqrestore(&port->lock, flags);
}
#endif

static void altera_jtaguart_console_write(struct console *co, const char *s,
					  unsigned int count)
{
	for (; count; count--, s++) {
		altera_jtaguart_console_putc(co, *s);
		if (*s == '\n')
			altera_jtaguart_console_putc(co, '\r');
	}
}

static int __init altera_jtaguart_console_setup(struct console *co,
						char *options)
{
	struct uart_port *port;

	if (co->index < 0 || co->index >= ALTERA_JTAGUART_MAXPORTS)
		return -EINVAL;
	port = &altera_jtaguart_ports[co->index].port;
	if (port->membase == 0)
		return -ENODEV;
	return 0;
}

static struct uart_driver altera_jtaguart_driver;

static struct console altera_jtaguart_console = {
	.name	= "ttyJ",
	.write	= altera_jtaguart_console_write,
	.device	= uart_console_device,
	.setup	= altera_jtaguart_console_setup,
	.flags	= CON_PRINTBUFFER,
	.index	= -1,
	.data	= &altera_jtaguart_driver,
};

static int __init altera_jtaguart_console_init(void)
{
	register_console(&altera_jtaguart_console);
	return 0;
}

console_initcall(altera_jtaguart_console_init);

#define	ALTERA_JTAGUART_CONSOLE	(&altera_jtaguart_console)

#else

#define	ALTERA_JTAGUART_CONSOLE	NULL

#endif /* CONFIG_ALTERA_JTAGUART_CONSOLE */

static struct uart_driver altera_jtaguart_driver = {
	.owner		= THIS_MODULE,
	.driver_name	= "altera_jtaguart",
	.dev_name	= "ttyJ",
	.major		= ALTERA_JTAGUART_MAJOR,
	.minor		= ALTERA_JTAGUART_MINOR,
	.nr		= ALTERA_JTAGUART_MAXPORTS,
	.cons		= ALTERA_JTAGUART_CONSOLE,
};

static int __devinit altera_jtaguart_probe(struct platform_device *pdev)
{
	struct altera_jtaguart_platform_uart *platp = pdev->dev.platform_data;
	struct uart_port *port;
	int i;

	for (i = 0; i < ALTERA_JTAGUART_MAXPORTS && platp[i].mapbase; i++) {
		port = &altera_jtaguart_ports[i].port;

		port->line = i;
		port->type = PORT_ALTERA_JTAGUART;
		port->mapbase = platp[i].mapbase;
		port->membase = ioremap(port->mapbase, ALTERA_JTAGUART_SIZE);
		port->iotype = SERIAL_IO_MEM;
		port->irq = platp[i].irq;
		port->ops = &altera_jtaguart_ops;
		port->flags = ASYNC_BOOT_AUTOCONF;

		uart_add_one_port(&altera_jtaguart_driver, port);
	}

	return 0;
}

static int __devexit altera_jtaguart_remove(struct platform_device *pdev)
{
	struct uart_port *port;
	int i;

	for (i = 0; i < ALTERA_JTAGUART_MAXPORTS; i++) {
		port = &altera_jtaguart_ports[i].port;
		if (port)
			uart_remove_one_port(&altera_jtaguart_driver, port);
	}

	return 0;
}

static struct platform_driver altera_jtaguart_platform_driver = {
	.probe	= altera_jtaguart_probe,
	.remove	= __devexit_p(altera_jtaguart_remove),
	.driver	= {
		.name	= DRV_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init altera_jtaguart_init(void)
{
	int rc;

	rc = uart_register_driver(&altera_jtaguart_driver);
	if (rc)
		return rc;
	rc = platform_driver_register(&altera_jtaguart_platform_driver);
	if (rc) {
		uart_unregister_driver(&altera_jtaguart_driver);
		return rc;
	}
	return 0;
}

static void __exit altera_jtaguart_exit(void)
{
	platform_driver_unregister(&altera_jtaguart_platform_driver);
	uart_unregister_driver(&altera_jtaguart_driver);
}

module_init(altera_jtaguart_init);
module_exit(altera_jtaguart_exit);

MODULE_DESCRIPTION("Altera JTAG UART driver");
MODULE_AUTHOR("Thomas Chou <thomas@wytron.com.tw>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
