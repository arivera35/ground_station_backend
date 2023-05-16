/*
    Rotator 
*/
#include <string.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <unistd.h> 

typedef struct {
    char port_num [50];
    int baud_rate;
    int num_bits;
    int num_stop_bits;
}sROTOR_CONFIG;

typedef struct {
    float rot_azimuth;
    float rot_elevation;
}sROTOR_INFO;

/*sROTOR_CONFIG az_rotor = {
	.port_num = "/dev/ttyUSB1", 
	.baud_rate = 9600, 
	.num_bits = 8, 
	.num_stop_bits = 1
};
*/

//sROTOR_CONFIG el_rotor = {.port_num = "/dev/ttyUSB0", .baud_rate = 9600, .num_bits = 8, .num_stop_bits = 1};

// Open serial communication with controller
int rot_init(sROTOR_CONFIG rotor_az, sROTOR_CONFIG rotor_el);

/*
// *** for testing *** sends R1n; to obtain version ID of controller software
sROTOR_INFO rot_get_info(){};

// Get rotor azimuth and elevation position 
sROTOR_INFO rot_get_positon(){};

// Set rotor azimuth and elevation position, updates rot_azimuth and rot_elevation 
int rot_set_position(float azimuth, float elevation){};

// Park rotor, returns 1 if successful stop, returns -1 otherwise 
int rot_park(){};

// Stop rotor, returns 1 if successful stop, returns -1 otherwise 
int rot_stop(){};
*/
