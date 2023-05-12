#include <stdio.h>  
#include "rotor.h"
#include "serial.h"

int rot_init(sROTOR_CONFIG rotor)
{
    // Initiate serial communication
    int response = serial_init(rotor.baud_rate, rotor.port_num, rotor.num_bits, rotor.num_stop_bits);
    if (response < 0){
        printf("Failed to initialize serial communication\n");
        return -1
    }
    return 1;
};

sROTOR_INFO rot_get_info(){

};