#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "serial.h"
#include "rotor.h"

#define PORT "/dev/ttyUSB1"

int main(void){

  sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8};

  int fd = serial_init(az_rotor.baud_rate, az_rotor.port_num, az_rotor.num_bits);
  int wrsp = serial_write(fd, "R1n;");
  // sleep(2);
  printf("fd %d\n", fd);
  printf("write response: %d\n", wrsp);
  char response [255];
  int bytes_read = serial_read(fd, response, 255);
  printf("Num bytes read %d\n", bytes_read);
  printf("String received: %s\n", response);
  // int len;
  // char text[255];
  // struct termios options; // serial port settings

  //opening serial port device file
  // fd = open(PORT, O_RDWR | O_NDELAY | O_NOCTTY); // read write access, no delay to read, no modem signals

  // if (fd < 0){
  //   printf("ERROR\n");
  //   return -1;
  // }

  // // set up serial port
  // options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; // CONTROL FLAGS: 9600 baud rate, 8 data bits
  // options.c_iflag = IGNPAR; // INPUT FLAGS: ignore parity errors
  // options.c_oflag = 0; // OUTPUT FLAGS
  // options.c_lflag = 0; // LOCAL FLAGS

  // // apply settings
  // tcflush(fd, TCIFLUSH); // clearing input buffer
  // tcsetattr(fd, TCSANOW, &options);

  // strcpy(text, "Bin;");
  // len = strlen(text);
  // len = write(fd, text, len);
  // printf("We sent %d bytes\n", len);

  // printf("Pausing for 5 seconds for response\n");

  // 40000 microsecond delay to wait for response 
  // usleep(40000);

  // memset(text, 0, 255);
  // len = read(fd, text, 255);
  // printf("Received %d bytes\n", len);
  // printf("Received string %s\n", text);

  // close(fd);
  // return 0;

}
