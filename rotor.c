#include <stdio.h>  
#include "rotor.h"
#include "serial.h"

int rot_init(sROTOR_CONFIG rotor_az, sROTOR_CONFIG rotor_el)
{
    // Initiate serial communication
    int response_az = serial_init(rotor_az.baud_rate, rotor_az.port_num, rotor_az.num_bits, rotor_az.num_stop_bits);
    int response_el = serial_init(rotor_el.baud_rate, rotor_el.port_num, rotor_el.num_bits, rotor_el.num_stop_bits);

    if (response_az < 0 && response_el < 0){
        printf("Failed to initialize serial communication\n");
        return -1;
    }
    if (response_az < 0 && response_el > 0){
        printf("Failed to initiate serial communication with AZ rotor");
        return -1;
    }
    if (response_el < 0 && response_az > 0){
        printf("Failed to initiate serial communcation with EL rotor");
        return -1;
    }
    return 1;
};

sROTOR_INFO rot_get_info(){

};