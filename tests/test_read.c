#include "berryllio_gpio.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int pin = 2;
	if (argc > 1) {
		pin = strtol(argv[1], NULL, 10);
	}

	init_gpio();
	set_pin_input(pin);
	while(1) {
		int val = read_pin(pin);
		printf("val = %i\n", val);
		sleep(1);
	}
}
