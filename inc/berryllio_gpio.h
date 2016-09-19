#ifndef __BERRYLLIO_GPIO_H__
#define __BERRYLLIO_GPIO_H__

#include <stdint.h>

extern int init_gpio(void);
extern int set_pin_input(uint8_t pin);
extern int read_pin(uint8_t pin);

#endif
