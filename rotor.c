#include <stdio.h>  
#include "rotor.h"
#include "serial.h"

int [] rot_init(sROTOR_CONFIG rotor_az, sROTOR_CONFIG rotor_el)
{
    // Initiate serial communication
    int serial_fd_az = serial_init(rotor_az.baud_rate, rotor_az.port_num, rotor_az.num_bits, rotor_az.num_stop_bits);
    int serial_fd_el = serial_init(rotor_el.baud_rate, rotor_el.port_num, rotor_el.num_bits, rotor_el.num_stop_bits);

    if (serial_fd_az < 0 && serial_fd_el < 0){
        printf("Failed to initialize serial communication\n");
        return -1;
    }
    if (serial_fd_az < 0 && serial_fd_el > 0){
        printf("Failed to initiate serial communication with AZ rotor");
        return -1;
    }
    if (serial_fd_el < 0 && serial_fd_az > 0){
        printf("Failed to initiate serial communcation with EL rotor");
        return -1;
    }
    int fd [2];
    fd [0] = serial_fd_az;
    fd [1] = serial_fd_el;
    return fd;
};

// sROTOR_INFO rot_get_info(){
//     char command [] = 
// };

sROTOR_INFO rot_get_info(int serial_fd)
{

    sROTOR_INFO info;

    // Send the command "R1n;" to obtain version ID
    char command[] = "R1n;";
    int bytes_written = write(serial_fd, command, sizeof(command) - 1);

    if (bytes_written == -1) {
        printf("Error writing to serial port\n");
        info.id = "-9999";
        info.rot_azimuth = 0.0;
        info.rot_elevation = 0.0;
    } else {
        // Wait for response and read the version ID
        char response[100];
        int bytes_read = read(serial_fd, response, sizeof(response) - 1);
        if (bytes_read == -1) {
            printf("Error reading from serial port\n");
            info.id = "-9999";
            info.rot_azimuth = 0.0;
            info.rot_elevation = 0.0;
        } else {
            // Parse the response and extract the version ID
            // Assuming the response is in a specific format, adjust the code accordingly.
            // For simplicity, let's assume the version ID is a float value.
            char version_id [] = response;

            // Update the fields of info with the obtained version ID
            info.id = version_id;
            info.rot_azimuth = 0.00;
            info.rot_elevation = 0.00;
        }
    }

    return info;
}
