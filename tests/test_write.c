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

	err = set_pin_output(pin);
	if (err < 0) {
		fprintf(stderr, "set_pin_output() failed\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < 10; i++) {
		printf("output set to 1\n");
		write_pin(pin, true);
		sleep(1);
		printf("output set to 0\n");
		write_pin(pin, false);
		sleep(1);
	}
}
