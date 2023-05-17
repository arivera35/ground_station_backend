#include <stdio.h>
#include <stdlib.h>
#include "rotor.h"
#include "serial.h"

int main()
{			
	int fd = open_port();	
	printf("fd :%d\n", fd);
	int write_output = write_cmd(fd);
	printf("Write output: %d\n", write_output);
	int read_output = read_resp(fd);
	printf("Read output: %d\n", read_output);
	// sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB1", .num_bits = 8, .num_stop_bits = 1};
	// sROTOR_CONFIG el_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8, .num_stop_bits = 1};

	// int serial_fd_az = rot_init(az_rotor);
	// int serial_fd_el = rot_init(el_rotor);
	
	// printf("Serial fd az: %d\n", serial_fd_az);
	// printf("Serial fd el: %d\n", serial_fd_el);

	// usleep(10000);

	// char* info = rot_get_info(serial_fd_az);
	// printf("Version ID: %s\n", info);

}
