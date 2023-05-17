/*
    Serial 
*/
#include <stdlib.h>
#include <stdio.h>  
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <termios.h>

// #include "serial.c"
// #include "serial_test.c"
// #include "rotor.h"

int serial_init(int baud_rate, char port_num [], int num_bits, int num_stop_bits);

int open_port(void);

int write_cmd(int fd);

int read_resp(int fd);

//int serial_close();

//int serial_flush();
