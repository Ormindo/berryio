#include "berryllio_gpio.h"

#include "berryllio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h> 
#include <sys/mman.h>
#include <fcntl.h>

#define GPIO_ADDR_OFFSET 0x200000
#define GPIO_BASE_ADDR (IO_BASE_ADDR + GPIO_ADDR_OFFSET)
#define GPIO_MEM_SIZE 0xB0

#define GPIO_COUNT 53

#define GPFSEL_OFFSET 0
#define GPFSEL_GPIO_COUNT 10

#define GPIO_OUTPUT_SET_OFFSET 0x1C

#define GPIO_OUTPUT_CLEAR_OFFSET 0x28

#define GPIO_INPUT_READ_OFFSET 0x34

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

static volatile uint32_t* gpios = NULL;
						
void init_gpio(void)
{
	int mem = open("/dev/mem", O_RDWR);
	if (mem < 0)
	 	exit(EXIT_FAILURE);

	gpios = mmap(NULL, GPIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED , mem, GPIO_BASE_ADDR);

}

static int check_pin(uint8_t pin)
{
	if (pin > GPIO_COUNT - 1)
		return -1;
	
	int8_t gpio = pin_to_gpio[pin];
	if (gpio < 0)
		return -1;
	
	return 0;
}

int set_pin_input(uint8_t pin)
{
	if (check_pin(pin) < 0)
		return -1;
	
	volatile uint32_t* gpfsel = gpios + GPFSEL_OFFSET + 4*(pin/GPFSEL_GPIO_COUNT);
	
	uint8_t offset = (pin % GPFSEL_GPIO_COUNT) * 3;
	uint32_t bits = 0b111 << offset;
	(*gpfsel) = (*gpfsel) & ~bits;
}

int read_pin(uint8_t pin)
{
	if (check_pin(pin) < 0)
		return -1;
	
	volatile uint32_t* input = gpios + GPIO_INPUT_READ_OFFSET + 4*(pin/31);
	
	uint8_t offset = (1 << pin%31);
	return ((*input & offset) == offset);
}
