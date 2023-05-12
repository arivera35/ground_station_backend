/*
    Serial 
*/
#include <stdlib.h>
#include <stdio.h>  
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <termios.h>

int serial_init(int baud_rate, char port_num [], int num_bits, int num_stop_bits);

//int serial_close();

//int serial_flush();
