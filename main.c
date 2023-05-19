#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define PORT "/dev/ttyUSB0"

int main(void) { 
  struct termios s_alicat; 
  int tty_fd, count; 
  char receive[8], transmit[5]; 
  // Ports connected in USB as sudo 
  if ((tty_fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) { 
    printf("Line failed to open with %d\n", tty_fd); 
    return -1; 
  } else { 
    printf("fd is %d\n", tty_fd); 
  } 
  if (tcgetattr(tty_fd, &s_alicat) < 0) { 
    printf("Error from tcgetattr: %s\n", strerror(errno)); 
    return -1; 
  } 
  cfsetospeed(&s_alicat, B9600); 
  cfsetispeed(&s_alicat, B9600); 
  // Set up receiver and set to local mode 
  s_alicat.c_cflag |= (CLOCAL | CREAD | CS8); 
  s_alicat.c_iflag |= IGNPAR | ICRNL;
  
  
  s_alicat.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
  s_alicat.c_iflag |= (ICRNL | IGNPAR);

  s_alicat.c_cflag &= ~CRTSCTS;
  s_alicat.c_iflag &= ~(IXON | IXOFF | IXANY);
  s_alicat.c_lflag |= ICANON;
  s_alicat.c_lflag &= ~(ECHO | ECHOE | ISIG);
  s_alicat.c_oflag |= OPOST;



  // Ignore parity errorss 
  // tcflush(tty_fd, TCIFLUSH); 
  //discard file information not transmitted 
  if (tcsetattr(tty_fd, TCSANOW, &s_alicat) != 0) { 
    printf("Error from tcsetattr: %s\n", strerror(errno)); 
    return -1; 
  } 
  // Clear the port before kicking off communications 
  strcpy(transmit, "\0\0\0"); 
  write(tty_fd, transmit, 5); 
  strcpy(transmit, "R1n;"); 
  if ((count = write(tty_fd, transmit, 5)) < 0) { 
    printf("Failed to write to device"); 
  } 
  int j = 0; count = 0; /* Attempt to read data at most 3 times if there is no data * coming back. */ 
  while (count == 0 && j < 3) { 
    // usleep(100000); 
    if ((count = read(tty_fd, receive, sizeof(receive) - 1)) < 0) { 
      printf("Error receiving text %d\n", count); 
    } 
    else { 
      printf("count is %d\n", count); 
      receive[count] = '\0'; 
      printf("String is %s", receive); 
    } 
    j++; 
  } 
  printf("Closing port...\n"); 
  int p = 0; 
  if ((p = close(tty_fd)) < 0) { 
    printf("Port failed to close %d\n", p); 
    return -1; 
  } 
  
  return 0; 
}


// #include <stdlib.h>
// #include <stdio.h>
// #include <termios.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <string.h>

// #define PORT "/dev/ttyUSB0"

// int main(void)
// {

//   struct termios s_alicat;

//   int tty_fd, count;

//   char receive[255], transmit[5];

//   // Ports connected in USB as sudo
//   if ((tty_fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){
//     printf("Line failed to open with %d\n", tty_fd);
//     return -1;
//   }

//   else{
//     printf("fd is %d\n", tty_fd);
//   }

//   if (tcgetattr(tty_fd, &s_alicat) < 0){
//     printf("Error from tcgetattr: %s\n", strerror(errno));
//     return -1;
//   }

//   cfsetospeed(&s_alicat, B9600);

//   cfsetispeed(&s_alicat, B9600);

//   // Set up receiver and set to local mode
//   s_alicat.c_cflag |= (CLOCAL | CREAD | CS8);

//   s_alicat.c_iflag |= IGNPAR | ICRNL; // Ignore parity errorss
//   tcflush(tty_fd, TCIFLUSH); //discard file information not transmitted
//   if (tcsetattr(tty_fd, TCSANOW, &s_alicat) != 0){
//     printf("Error from tcsetattr: %s\n", strerror(errno));
//     return -1;
//   }

//   // Clear the port before kicking off communications
//   strcpy(transmit, "\r\r\r");

//   write(tty_fd, transmit, 5);

//   strcpy(transmit, "R1n;");

//   if ((count = write(tty_fd, transmit, 5)) < 0){
// 	printf("Failed to write to device");
//   }

//   int j = 0;
//   count = 0;

//   /* Attempt to read data at most 3 times if there is no data 
//    * coming back.
//    */
//   while (count == 0 && j < 3){
//     usleep(300000);
//     if ((count = read(tty_fd, receive, sizeof(receive) - 1)) < 0){
// 		printf("Error receiving text %d\n", count);
//     }
//     else{
// 		printf("count is %d\n",  count);
// 		receive[count] = 0;
// 		printf("String is %s", receive);
//     }
//     j++;
//   }

//   usleep(100000);

// 	printf("Response %s\n", receive);
//   printf("Closing port...\n");

//   int p = 0;

//   if ((p = close(tty_fd)) < 0){
//     printf("Port failed to close %d\n", p);
//     return -1;
//   }

//   return 0;

// }


// #include <stdio.h>
// #include <stdlib.h>
// // #include "rotor.h"
// // #include "serial.h"
// #include <SerialPort.h>
// #include <SerialStream.h>



// int main()
// {			


// 	using namespace LibSerial;

// 	SerialPort my_port("/dev/ttyUSB1");

// 	my_port.SetBaudRate( BAUD_9600);
// 	my_port.SetCharacterSize( CHAR_SIZE_8);
// 	my_port.SetNumOfStopBits( STOP_BITS_1);

// 	const int msg_size = 4;
// 	char msg[msg_size] = "R1n;"
// 	my_port.Write(msg, msg_size);
	


// 	// int fd = open_port();	
// 	// printf("fd :%d\n", fd);
// 	// int write_output = write_cmd(fd);
// 	// printf("Write output: %d\n", write_output);
// 	// int read_output = read_resp(fd);
// 	// printf("Read output: %d\n", read_output);
// 	// sROTOR_CONFIG az_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB1", .num_bits = 8, .num_stop_bits = 1};
// 	// sROTOR_CONFIG el_rotor = {.baud_rate = 9600, .port_num = "/dev/ttyUSB0", .num_bits = 8, .num_stop_bits = 1};

// 	// int serial_fd_az = rot_init(az_rotor);
// 	// int serial_fd_el = rot_init(el_rotor);
	
// 	// printf("Serial fd az: %d\n", serial_fd_az);
// 	// printf("Serial fd el: %d\n", serial_fd_el);

// 	// usleep(10000);

// 	// char* info = rot_get_info(serial_fd_az);
// 	// printf("Version ID: %s\n", info);

// }
