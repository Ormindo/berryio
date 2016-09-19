#include "berryllio_gpio.h"

#include "berryllio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

// ============================================================================ 
//                                  PROTOTYPES
// ============================================================================

static bool is_pin_gpio(uint8_t pin);
static void gpfsel_write(uint8_t gpio, uint8_t value);

// ============================================================================ 
//                                  GLOBAL VARS
// ============================================================================

// Raspberry Pi B+ pin map
static const int8_t pin_to_gpio[] = {
	-1, -1, // 3.3V 5V
	 2, -1, // IO2  5V
	 3, -1, // IO3  GND
	 4, 14, // IO4  IO14
	-1, 15, // GND  IO15
	17, 18, // IO17 IO18
	27, -1, // IO27 GND
	22, 23, // IO22 IO23
	-1, 24, // 3.3V IO24
	 1, -1, // IO10 GND
	 9, 25, // IO9  IO25
	11, 8,  // IO11 IO8
	-1, 7   // GND  IO7
}; 

static volatile uint32_t* gpios = NULL;

// ============================================================================
//                                   FUNCTIONS
// ============================================================================

int init_gpio(void)
{
	int mem = open("/dev/mem", O_RDWR);
	if (mem < 0)
	        return -1;

	gpios = mmap(NULL, GPIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED , mem, GPIO_BASE_ADDR);
	if ((int)gpios == -1)
		return -1;

	return 0;
}

int set_pin_input(uint8_t pin)
{
	if (!is_pin_gpio(pin))
		return -1;
	
	int8_t gpio = pin_to_gpio[pin];
	gpfsel_write(gpio, 0);
	
	return 0;
}

int read_pin(uint8_t pin)
{
	if (!is_pin_gpio(pin))
		return -1;
	
	int8_t gpio = pin_to_gpio[pin];	
	// 32 GPIOs per input register
	volatile uint32_t* input = gpios + GPIO_INPUT_READ_OFFSET/4 + (gpio/32);
	//volatile uint32_t* input = (uint32_t*)((uint32_t)gpios + GPIO_INPUT_READ_OFFSET + 4*(pin/32));
	
	uint8_t offset = (1 << gpio%32);
	return ((*input & offset) == offset);
}

// ============================================================================
//                               STATIC FUNCTIONS
// ============================================================================

// Checks if pin number is valid and if pin is a gpio
static bool is_pin_gpio(uint8_t pin)
{
	if (pin > GPIO_COUNT - 1)
		return false;
	
	int8_t gpio = pin_to_gpio[pin];
	return gpio >= 0;
}

// Writes 'value' to the GPFSEL register bits associated with 'gpio'
static void gpfsel_write(uint8_t gpio, uint8_t value)
{
	// 10 GPIOs per GPFSEL register
	volatile uint32_t* gpfsel = gpios + GPFSEL_OFFSET/4 + (gpio/GPFSEL_GPIO_COUNT);
	
	uint8_t offset = (gpio % GPFSEL_GPIO_COUNT) * 3;
	uint32_t clear_mask = ~(0b111 << offset);
	value = value << offset;
	// Clear then set the 3 targeted bits
	(*gpfsel) = ((*gpfsel) & clear_mask) | value;
}
