#include "serial.h"
#include "rotor.h"
#include "rot_commands.h"
#include "tle.h"
#include "predict.h"

int main(void){

	char catnr [5];
	printf("Enter NORAD catalog number: ");
	scanf("%s", catnr);
	TLEData tle_data;
	set_cat_num(catnr, &tle_data);
	printf("TLE line 1: %s\n", tle_data.line1);
	printf("TLE line 2: %s\n", tle_data.line2);

	// Create orbit object
	predict_orbital_elements_t *sat = predict_parse_tle(tle_data.line1, tle_data.line2);
	if (!sat) {
		fprintf(stderr, "Failed to initialize orbit from tle!");
		exit(1);
	}
	printf("Satellite object: %d\n", sat->satellite_number);

	predict_observer_t *obs = predict_create_observer("Francis GS", 31.767600*M_PI/180.0, -106.435*M_PI/180.0, 1129);
	if (!obs) {
		fprintf(stderr, "Failed to initialize observer!");
		exit(1);
	}

	while(true){

		predict_julian_date_t curr_time = predict_to_julian(time(NULL));

		struct predict_position sat_orbit;
		predict_orbit(sat, &sat_orbit, curr_time);
		printf("Satellite lat=%f, lon=%f, alt=%f\n", sat_orbit.latitude*180.0/M_PI, sat_orbit.longitude*180.0/M_PI, sat_orbit.altitude);
		printf("velocity=%lf, position=%lf\n", *sat_orbit.velocity, *sat_orbit.position);

		struct predict_observation sat_obs;
		predict_observe_orbit(obs, &sat_orbit, &sat_obs);
		printf("azi=%f, ele=%f\n", sat_obs.azimuth*180.0/M_PI, sat_obs.elevation*180.0/M_PI);
		fflush(stdout);
		usleep(4000000);
	}

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


// #include <stdio.h>
// #include <math.h>
// #include <unistd.h>
// #include <stdlib.h>

// #include "predict.h"

// int main(int argc, char **argv)
// {

// 	const char *tle_line_1 = "1 25544U 98067A   23156.53030566  .00015207  00000+0  26904-3 0  9993";
// 	const char *tle_line_2 = "2 25544  51.6425  26.3209 0005524  54.2799  18.0165 15.50502749399986";

// 	// Create orbit object
// 	predict_orbital_elements_t *iss = predict_parse_tle(tle_line_1, tle_line_2);
// 	if (!iss) {
// 		fprintf(stderr, "Failed to initialize orbit from tle!");
// 		exit(1);
// 	}

// 	// Create observer object
// 	predict_observer_t *obs = predict_create_observer("Francis GS", 31.767600*M_PI/180.0, -106.435*M_PI/180.0, 1125);
// 	if (!obs) {
// 		fprintf(stderr, "Failed to initialize observer!");
// 		exit(1);
// 	}

// 	printf("\e[1;1H\e[2J"); //clear screen

// 	while (true) {
// 		printf("\033[0;0H"); //print from start of the terminal

// 		predict_julian_date_t curr_time = predict_to_julian(time(NULL));

//     // printf("Julian date: %f\n", curr_time);

// 		// Predict ISS
// 		struct predict_position iss_orbit;
// 		predict_orbit(iss, &iss_orbit, curr_time);
// 		printf("ISS: lat=%f, lon=%f, alt=%f\n", iss_orbit.latitude*180.0/M_PI, iss_orbit.longitude*180.0/M_PI, iss_orbit.altitude);

// 		// Observe ISS
// 		struct predict_observation iss_obs;
// 		predict_observe_orbit(obs, &iss_orbit, &iss_obs);
// 		printf("ISS: azi=%f (rate: %f), ele=%f (rate: %f)\n", iss_obs.azimuth*180.0/M_PI, iss_obs.azimuth_rate*180.0/M_PI, iss_obs.elevation*180.0/M_PI, iss_obs.elevation_rate*180.0/M_PI);

// 		// Apparent elevation
// 		double apparent_elevation = predict_apparent_elevation(iss_obs.elevation);
// 		printf("Apparent ISS elevation: %.2f\n\n", apparent_elevation*180.0/M_PI);

// 		// Predict and observe MOON
// 		struct predict_observation moon_obs;
// 		predict_observe_moon(obs, curr_time, &moon_obs);
// 		printf("MOON: %f, %f\n", moon_obs.azimuth*180.0/M_PI, moon_obs.elevation*180.0/M_PI);

// 		// Predict and observe SUN
// 		struct predict_observation sun_obs;
// 		predict_observe_sun(obs, curr_time, &sun_obs);
// 		printf("SUN: %f, %f\n", sun_obs.azimuth*180.0/M_PI, sun_obs.elevation*180.0/M_PI);

// 		//Sleep
// 		fflush(stdout);
// 		usleep(1000000);
// 	}

// 	// Free memory
// 	predict_destroy_orbital_elements(iss);
// 	predict_destroy_observer(obs);

// 	return 0;
// }