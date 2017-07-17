/*
 * Copyright (C) 2017  sergio inacio <inaciose@gmail.com>
 * Partially based on the sunxi-tools/uart0-helloworld-sdboot.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Copyright (C) 2016  Siarhei Siamashka <siarhei.siamashka@gmail.com>
 * Partially based on the uart code from ar100-info
 *
 * (C) Copyright 2013 Stefan Kristiansson <stefan.kristiansson@saunalahti.fi>
 * Partially based on the sunxi gpio code from U-Boot
 *
 * (C) Copyright 2012 Henrik Nordstrom <henrik@henriknordstrom.net>
 * Based on earlier arch/arm/cpu/armv7/sunxi/gpio.c:
 *
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*****************************************************************************/

#include "board.h"

/* A workaround for https://patchwork.ozlabs.org/patch/622173 */
void __attribute__((section(".start"))) __attribute__((naked)) start(void)
{
  asm volatile("b     main             \n"
         ".long 0xffffffff       \n"
         ".long 0xffffffff       \n"
         ".long 0xffffffff       \n");
}

void sconsole( void ) {
    char schar;
    schar = uart0_getc();
    if(schar == 13) {
      ledon();
      uart0_putc(schar);
      uart0_putc('\n');
      uart0_putc('>');
      uart0_putc(' ');      
    } else {
      uart0_putc(schar);
    }
}
 
int main(void)
{
	int soc_code; // not used
  
  soc_detection_init();
	gpio_init();
	uart0_init();  
  soc_code = soc_detection_show();  
  
  /* uart echo console */
  uart0_puts("Serial line console\n");
  uart0_putc('>');
  uart0_putc(' ');
  
  /* green led */
  ledconf();
  //ledon();
  int ledtimer = 0;
  int ledflag = 0;
  
  while (1) {
    sconsole();
    // bof: led
        
    ledtimer++;
    if(ledtimer == 16000) {
      ledtimer = 0;
      if(ledflag) {
        ledoff();
        ledflag = 0;
      } else {
        ledon();
        ledflag = 1;
      }
    }
    
    // eof: led    
  }
  
  /* end or loop program */  
	switch (get_boot_device()) {
	case BOOT_DEVICE_FEL:
		uart0_puts("Returning back to FEL.\n");
		return 0;
	case BOOT_DEVICE_MMC0:
		uart0_puts("Booted from MMC0, entering an infinite loop.\n");
		while (1) {}
	case BOOT_DEVICE_SPI:
		uart0_puts("Booted from SPI0, entering an infinite loop.\n");
		while (1) {}
	default:
		uart0_puts("Booted from unknown media, entering an infinite loop.\n");
		while (1) {}
	};

	return 0;
}
