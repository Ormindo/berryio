#include "berryllio_gpio.h"

#include "berryllio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <sys/mman.h>
#include <fcntl.h>

#define GPIO_ADDR_OFFSET 0x200000
#define GPIO_BASE_ADDR (IO_BASE_ADDR + GPIO_ADDR_OFFSET)
#define GPIO_MEM_SIZE 0xB0

#define PIN_COUNT 26

#define GPFSEL_OFFSET 0
#define GPFSEL_GPIO_COUNT 10

#define GPSET_OFFSET 0x1C
#define GPSET_GPIO_COUNT 32

#define GPCLR_OFFSET 0x28
#define GPCLR_GPIO_COUNT 32

#define GPLEV_OFFSET 0x34
#define GPLEV_GPIO_COUNT 32

// ============================================================================ 
//                                  PROTOTYPES
// ============================================================================

static int8_t pin_to_gpio(uint8_t pin);
static void gpfsel_write(uint8_t gpio, uint8_t value);
static void write_output(uint8_t gpio);
static void clear_output(uint8_t gpio);

// ============================================================================ 
//                                  GLOBAL VARS
// ============================================================================

// Raspberry Pi B+ pin map
static const int8_t pin_gpio_map[] = {
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
	int8_t gpio = pin_to_gpio(pin);
	if (gpio < 0)
		return -1;

	// 0 sets gpio as input
	gpfsel_write(gpio, 0);
	
	return 0;
}

int set_pin_output(uint8_t pin)
{
	int8_t gpio = pin_to_gpio(pin);
	if (gpio < 0)
		return -1;

	// 1 sets gpio as output
	gpfsel_write(gpio, 1);

	return 0;
}

int read_pin(uint8_t pin)
{
	int8_t gpio = pin_to_gpio(pin);
	if (gpio < 0)
		return -1;

	// 32 GPIOs per input register
	volatile uint32_t* gplev = gpios + GPLEV_OFFSET / (sizeof gplev) + (gpio / GPLEV_GPIO_COUNT);
	
	uint8_t offset = (1 << gpio%32);
	return ((*gplev & offset) == offset);
}

int write_pin(uint8_t pin, bool value)
{
	int8_t gpio = pin_to_gpio(pin);
	if (gpio < 0)
		return -1;

	if (value) {
		write_output(gpio);
	} else {
		clear_output(gpio);
	}

	return 0;
}

// ============================================================================
//                               STATIC FUNCTIONS
// ============================================================================

// Checks if pin number is valid and if pin is a gpio
static int8_t pin_to_gpio(uint8_t pin)
{
	if (pin > PIN_COUNT - 1)
		return -1;
	
	return pin_gpio_map[pin];
}

// Writes 'value' to the GPFSEL register bits associated with 'gpio'
static void gpfsel_write(uint8_t gpio, uint8_t value)
{
	// GPFSEL_GPIO_COUNT GPIOs per GPFSEL register
	volatile uint32_t* gpfsel = gpios + GPFSEL_OFFSET / (sizeof gpfsel) + (gpio / GPFSEL_GPIO_COUNT);
	
	uint8_t offset = (gpio % GPFSEL_GPIO_COUNT) * 3;
	uint32_t clear_mask = ~(0b111 << offset);
	value = value << offset;
	// Clear then set the 3 targeted bits
	(*gpfsel) = ((*gpfsel) & clear_mask) | value;
}

static void write_output(uint8_t gpio)
{
	volatile uint32_t* gpset = gpios + GPSET_OFFSET / (sizeof gpset) + (gpio / GPSET_GPIO_COUNT);
	uint8_t value = (1 << gpio%32);
	// zeroes are ignored, hence setting the right bit is enough
	*gpset = value;
}

static void clear_output(uint8_t gpio)
{
	volatile uint32_t* gpclr = gpios + GPCLR_OFFSET / (sizeof gpclr) + (gpio / GPCLR_GPIO_COUNT);
	uint8_t value = (1 << gpio%32);
	// zeroes are ignored, hence setting the right bit is enough
	*gpclr = value;
}
