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

int serial_init(int baud_rate, char port_num [], int num_bits);

int serial_write(int fd, char cmd []);

// int serial_read(int fd);

// int write_cmd(int fd);

// int read_resp(int fd);

//int serial_close();

//int serial_flush();
