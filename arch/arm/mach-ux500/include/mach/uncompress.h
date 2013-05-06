/*
 *  Copyright (C) 2009 ST-Ericsson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __ASM_ARCH_UNCOMPRESS_H
#define __ASM_ARCH_UNCOMPRESS_H

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <linux/io.h>
#include <linux/amba/serial.h>
#include <mach/hardware.h>

u32 ux500_uart_base;

#define __UART_BASE(soc, x)	soc##_UART##x##_BASE
#define UART_BASE(soc, x)	__UART_BASE(soc, x)

static void putc(const char c)
{
	/* Do nothing if the UART is not enabled. */
	if (!ux500_uart_base ||
	    !(__raw_readb(ux500_uart_base + UART011_CR) & 0x1))
		return;

	if (c == '\n')
		putc('\r');

	while (__raw_readb(ux500_uart_base + UART01x_FR) & (1 << 5))
		barrier();
	__raw_writeb(c, ux500_uart_base + UART01x_DR);
}

static void flush(void)
{
	if (!ux500_uart_base ||
	    !(__raw_readb(ux500_uart_base + UART011_CR) & 0x1))
		return;
	while (__raw_readb(ux500_uart_base + UART01x_FR) & (1 << 3))
		barrier();
}

static inline void arch_decomp_setup(void)
{
	/* Check in run time if we run on an U8500 or U5500 */
	if (machine_is_u5500())
		ux500_uart_base = UART_BASE(U5500, CONFIG_UX500_DEBUG_UART);
	else
		ux500_uart_base = UART_BASE(U8500, CONFIG_UX500_DEBUG_UART);
}

#define arch_decomp_wdog() /* nothing to do here */

#endif /* __ASM_ARCH_UNCOMPRESS_H */
