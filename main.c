// #include "serial.h"
// #include "rotor.h"
// #include "rot_commands.h"
// #include "tle.h"

// int main(void){

//   char catnr [5];
//   printf("Enter NORAD catalog number: ");
//   scanf("%s", catnr);
//   TLEData tle_data;
//   set_cat_num(catnr, &tle_data);
//   printf("TLE Name: %s\n", tle_data.name);
//   // printf("Line Number: %d\n", tle_data.line_number);
//   printf("Catalog Number: %d\n", tle_data.catalog_number);
//   // printf("Classification: %c\n", tle_data.classification);
//   printf("International Designator: %02d-%03d\n", tle_data.international_designator_launch_year,
//           tle_data.international_designator_launch_number);
//   printf("Epoch year: %d and epoch day: %lf\n", tle_data.epoch_year, tle_data.epoch_day);
//   printf("First Time Derivative: %lf\n", tle_data.first_time_derivative);
//   printf("Second Time Derivative: %d\n", tle_data.second_time_derivative);
//   printf("BSTAR: %lf\n", tle_data.bstar);
//   // printf("Element Set Type: %d\n", tle_data.ephemeris);
//   // printf("Element Number: %d\n", tle_data.element_number);
//   // printf("Checksum line 1: %d\n", tle_data.checksum_line1);
//   printf("Inclination: %lf\n", tle_data.inclination);
//   printf("Right Ascension: %lf\n", tle_data.right_ascension);
//   printf("Eccentricity: %lf\n", tle_data.eccentricity);
//   printf("Argument of Perigee: %lf\n", tle_data.argument_of_perigee);
//   printf("Mean Anomaly: %lf\n", tle_data.mean_anomaly);
//   printf("Mean Motion: %lf\n", tle_data.mean_motion);
//   printf("Revolution Number: %d\n", tle_data.revolution_number);

// }
// /*  SERIAL TESTS
//   sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB1", .num_bits = 8};
//   sROTOR_CONFIG el_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8};

//   int fd = rot_init(az_rotor);

//   // int fd = serial_init(el_rotor.baud_rate, el_rotor.port_num, el_rotor.num_bits);
//   serial_write(fd, VERSION_ID);
//   char response [255];
//   int bytes_read = serial_read(fd, response, 255);
//   printf("Num bytes read %d\n", bytes_read);
//   printf("String received: %s\n", response);
//   serial_close(fd);
// */
//   // int tle_res = get_tle();
// // }


#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

#include "predict.h"

int main(int argc, char **argv)
{

	const char *tle_line_1 = "1 25544U 98067A   23156.53030566  .00015207  00000+0  26904-3 0  9993";
	const char *tle_line_2 = "2 25544  51.6425  26.3209 0005524  54.2799  18.0165 15.50502749399986";

	// Create orbit object
	predict_orbital_elements_t *iss = predict_parse_tle(tle_line_1, tle_line_2);
	if (!iss) {
		fprintf(stderr, "Failed to initialize orbit from tle!");
		exit(1);
	}

	// Create observer object
	predict_observer_t *obs = predict_create_observer("Francis GS", 31.767600*M_PI/180.0, -106.435*M_PI/180.0, 1125);
	if (!obs) {
		fprintf(stderr, "Failed to initialize observer!");
		exit(1);
	}

	printf("\e[1;1H\e[2J"); //clear screen

	while (true) {
		printf("\033[0;0H"); //print from start of the terminal

		predict_julian_date_t curr_time = predict_to_julian(time(NULL));

    printf("Julian date: %f\n", curr_time);

		// Predict ISS
		struct predict_position iss_orbit;
		predict_orbit(iss, &iss_orbit, curr_time);
		printf("ISS: lat=%f, lon=%f, alt=%f\n", iss_orbit.latitude*180.0/M_PI, iss_orbit.longitude*180.0/M_PI, iss_orbit.altitude);

		// Observe ISS
		struct predict_observation iss_obs;
		predict_observe_orbit(obs, &iss_orbit, &iss_obs);
		printf("ISS: azi=%f (rate: %f), ele=%f (rate: %f)\n", iss_obs.azimuth*180.0/M_PI, iss_obs.azimuth_rate*180.0/M_PI, iss_obs.elevation*180.0/M_PI, iss_obs.elevation_rate*180.0/M_PI);

		// Apparent elevation
		double apparent_elevation = predict_apparent_elevation(iss_obs.elevation);
		printf("Apparent ISS elevation: %.2f\n\n", apparent_elevation*180.0/M_PI);

		// Predict and observe MOON
		struct predict_observation moon_obs;
		predict_observe_moon(obs, curr_time, &moon_obs);
		printf("MOON: %f, %f\n", moon_obs.azimuth*180.0/M_PI, moon_obs.elevation*180.0/M_PI);

		// Predict and observe SUN
		struct predict_observation sun_obs;
		predict_observe_sun(obs, curr_time, &sun_obs);
		printf("SUN: %f, %f\n", sun_obs.azimuth*180.0/M_PI, sun_obs.elevation*180.0/M_PI);

		//Sleep
		fflush(stdout);
		usleep(1000000);
	}

	// Free memory
	predict_destroy_orbital_elements(iss);
	predict_destroy_observer(obs);

	return 0;
}