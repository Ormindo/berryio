#include "berryllio_gpio.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int err = 0;
	int pin = 2;
	if (argc > 1) {
		pin = strtol(argv[1], NULL, 10);
	}

	err = init_gpio();
	if (err < 0) {
		fprintf(stderr, "init_gpio() failed\n");
		exit(EXIT_FAILURE);
	}

	err = set_pin_input(pin);
	if (err < 0) {
		fprintf(stderr, "set_pin_input() failed\n");
		exit(EXIT_FAILURE);
	}

	while(1) {
		int val = read_pin(pin);
		printf("val = %i\n", val);
		sleep(1);
	}
}
