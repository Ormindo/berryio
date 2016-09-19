#include "berryllio_gpio.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void do_pwm(int period, float duty, uint8_t pin);

int main (int argc, char* argv[])
{
	int err = init_gpio();
	if (err < 0) {
		fprintf(stderr, "init_gpio() failed\n");
		exit(EXIT_FAILURE);
	}

	uint8_t pin = 2;
	if (argc > 1) {
		pin = strtol(argv[1], NULL, 10);
	}

	err = set_pin_output(pin);
	if (err < 0) {
		fprintf(stderr, "set_pin_output() failed\n");
		exit(EXIT_FAILURE);
	}

	float duty = 0.0;
	for(int i = 0; i < 100; i++) {
		do_pwm(200, duty, pin);
		duty += 0.01;
	}
}

static void do_pwm(int period, float duty, uint8_t pin)
{
	if (duty < 0.0)
		duty = 0.0;
	else if (duty > 1.0)
		duty = 1.0;

	uint32_t uptime_duration = 1000 * period * duty;
	uint32_t downtime_duration = 1000 * (period - (period*duty));

	write_pin(pin, true);
	usleep(uptime_duration);

	write_pin(pin, false);
	usleep(downtime_duration);
}
