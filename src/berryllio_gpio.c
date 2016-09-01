#include "berryllio_gpio.h"

#include "berryllio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define GPIO_BASE_ADDR (IO_BASE_ADDR + 0x200000)

static const int8_t pin_to_gpio[] = { -1, -1,  // 3.3V  5V
					     2, -1,  // IO2  5V
					     3, -1,  // IO3  GND
					     4, 14,  // IO4  IO14
					    -1, 15,  // GND  IO15
					    17, 18,  // IO17 IO18
					    27, -1,  // IO27 GND
					    22, 23,  // IO22 IO23
					    -1, 24,  // 3.3V IO24
					     1, -1,  // IO10 GND
					     9, 25,  // IO9  IO25
					    11, 8,   // IO11 IO8
					    -1, 7 }; // GND  IO7

static void init_gpio(void)
{
	// mmap
}
