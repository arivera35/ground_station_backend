#include "serial.h"
#include "rotor.h"
#include "rot_commands.h"

int main(void){

  sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB1", .num_bits = 8};
  sROTOR_CONFIG el_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8};

  int fd = serial_init(el_rotor.baud_rate, el_rotor.port_num, el_rotor.num_bits);
  serial_write(fd, STATUS);
  char response [255];
  int bytes_read = serial_read(fd, response, 255);
  printf("Num bytes read %d\n", bytes_read);
  printf("String received: %s\n", response);
  serial_close(fd);
  
}
