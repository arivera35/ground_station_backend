#include <stdio.h>
#include <stdlib.h>
#include "rotor.h"
#include "serial.h"

int main(){			
	
	sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB1", .num_bits = 8, .num_stop_bits = 1};
	sROTOR_CONFIG el_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8, .num_stop_bits = 1};

	int response = rot_init(az_rotor, el_rotor);
	
	printf("response: %d\n", response);
}
