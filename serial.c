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

    // No parity
    options.c_cflag &= ~PARENB;
    
    // Raw input
    options.c_cflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Raw output
    options.c_cflag &= ~OPOST;

    // No handshake
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag &= ~IXON;

    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 5;

    // Apply the configuration
    tcsetattr(fd, TCSANOW, &options);

    // Clear the input and output buffers
    tcflush(fd, TCIOFLUSH);

    return fd;
}

// OPEN PORT
int open_port(void){
    int fd;
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    struct termios options;
    tcgetattr(fd, &options);

    // Set the baud rate
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // Set up receiver and set to local mode 
    options.c_cflag |= (CLOCAL | CREAD | CS8); 
    options.c_iflag |= IGNPAR | ICRNL; 

    // options.c_cflag |= CS8;
    // options.c_cflag &= ~CSTOPB; 
    // options.c_cflag &= ~PARENB;

    if ((tcsetattr(fd, TCSANOW, &options)) != 0){
        perror("error from tcsetattr");
    }

    if (fd == -1){
        // could not open port
        perror("open_port: unable to open /dev/ttyUSB0 - ");
    }
    
    return (fd);

}

int write_cmd(int fd){
    tcflush(fd, TCIFLUSH);
    char transmit[5];
    strcpy(trasmit, "");
    write(fd, transmit, 5);
    strcpy(transmit, "Bin;");
    int n = write(fd, transmit, 5);
    if (n == -1){ 
        printf("write failed\n");
        return -1;
    }
    return 1;
}

int read_resp(int fd){
    char buf[1024];
    int n = read(fd, buf, sizeof(buf));
    if (n < 0){
        perror("read failed\n");
        return -1;
    }
    printf("read successful\n");
    return 1;
}