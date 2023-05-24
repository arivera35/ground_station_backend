#include "serial.h"
#include "rotor.h"

int main(void){

  sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8};

  int fd = serial_init(az_rotor.baud_rate, az_rotor.port_num, az_rotor.num_bits);
  int wrsp = serial_write(fd, "R1n;");
  printf("write response: %d\n", wrsp);
  char response [255];
  int bytes_read = serial_read(fd, response, 255);
  printf("Num bytes read %d\n", bytes_read);
  printf("String received: %s\n", response);
  serial_close(fd);
}
