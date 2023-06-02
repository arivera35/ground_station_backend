#include "serial.h"
#include "rotor.h"
#include "rot_commands.h"
#include "tle.h"

int main(void){

  char catnr [5];
  printf("Enter NORAD catalog number: ");
  scanf("%s", catnr);
  TLEData tle_data;
  set_cat_num(catnr, &tle_data);
  printf("TLE Name: %s\n", tle_data.name);
  printf("Line Number: %d\n", tle_data.line_number);
  printf("Catalog Number: %d\n", tle_data.catalog_number);
  printf("Classification: %c\n", tle_data.classification);
  printf("International Designator: %02d-%03d-%s\n", tle_data.international_designator_launch_year,
          tle_data.international_designator_launch_number, tle_data.international_designator_launch_piece);
  printf("Epoch year: %d and epoch day: %lf\n", tle_data.epoch_year, tle_data.epoch_day);
  printf("First Time Derivative: %lf\n", tle_data.first_time_derivative);
  printf("Second Time Derivative: %lf\n", tle_data.second_time_derivative);
  printf("BSTAR: %lf\n", tle_data.bstar);
  printf("Element Set Type: %d\n", tle_data.ephemeris);
  printf("Element Number: %d\n", tle_data.element_number);
  printf("Checksum line 1: %d\n", tle_data.checksum_line1);
  printf("Inclination: %lf\n", tle_data.inclination);
  printf("Right Ascension: %lf\n", tle_data.right_ascension);
  printf("Eccentricity: %lf\n", tle_data.eccentricity);
  printf("Argument of Perigee: %lf\n", tle_data.argument_of_perigee);
  printf("Mean Anomaly: %lf\n", tle_data.mean_anomaly);
  printf("Mean Motion: %lf\n", tle_data.mean_motion);
  printf("Revolution Number: %d\n", tle_data.revolution_number);

}
/*  SERIAL TESTS
  sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB1", .num_bits = 8};
  sROTOR_CONFIG el_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8};

  int fd = rot_init(az_rotor);

  // int fd = serial_init(el_rotor.baud_rate, el_rotor.port_num, el_rotor.num_bits);
  serial_write(fd, VERSION_ID);
  char response [255];
  int bytes_read = serial_read(fd, response, 255);
  printf("Num bytes read %d\n", bytes_read);
  printf("String received: %s\n", response);
  serial_close(fd);
*/
  // int tle_res = get_tle();
// }
