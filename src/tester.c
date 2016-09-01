#include "berryllio_gpio.h"

#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	init_gpio();
	set_pin_input(2);
	while(1) {
		int val = read_pin(2);
		printf("val = %i\n", val);
		sleep(1);
	}
}
