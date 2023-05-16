#include "serial.h"
#include "rotor.h"

int serial_init(int baud_rate, char port_num[], int num_bits, int num_stop_bits) 
{
    
    int fd;

    // Open the serial port
    fd = open(port_num, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Error opening serial port");
        return -1;
    }

    // Configure the serial port
    struct termios options;
    tcgetattr(fd, &options);

    // Set the baud rate
    switch (baud_rate) {
        case 9600:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
        default:
            fprintf(stderr, "Unsupported baud rate\n");
            close(fd);
            return -1;
    }

    // Set data bits
    options.c_cflag &= ~CSIZE;  // Clear existing data bits settings
    switch (num_bits) {
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr, "Unsupported number of data bits\n");
            close(fd);
            return -1;
    }

    // Set stop bits
    if (num_stop_bits == 1){
        options.c_cflag &= ~CSTOPB;  // 1 stop bit
    }
    else {
        fprintf(stderr, "Unsupported number of stop bits\n");
        close(fd);
        return -1;
    }

    // Apply the configuration
    tcsetattr(fd, TCSANOW, &options);

    // Clear the input and output buffers
    tcflush(fd, TCIOFLUSH);

    // Set the blocking mode (optional)
    fcntl(fd, F_SETFL, 0);

    return fd;
}
