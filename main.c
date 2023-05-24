#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define PORT "/dev/ttyUSB0"

int main(void){
  int fd, len;
  char text[255];
  struct termios options; // serial port settings

  //opening serial port device file
  fd = open(PORT, O_RDWR | O_NDELAY | O_NOCTTY); // read write access, no delay to read, no modem signals
  printf("fd %d", fd);
  if (fd < 0){
    printf("ERROR\n");
    return -1;
  }

  // set up serial port
  // tcgetattr(fd, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; // CONTROL FLAGS: 9600 baud rate, 8 data bits
  options.c_iflag = IGNPAR; // INPUT FLAGS: ignore parity errors
  options.c_oflag = 0; // OUTPUT FLAGS
  options.c_lflag = 0; // LOCAL FLAGS

  // apply settings
  tcflush(fd, TCIFLUSH); // clearing input buffer
  tcsetattr(fd, TCSANOW, &options);

  strcpy(text, "Bin;");
  len = strlen(text);
  len = write(fd, text, len);
  printf("We sent %d bytes\n", len);

  printf("Pausing for 5 seconds for response\n");

  sleep(5);

  memset(text, 0, 255);
  len = read(fd, text, 255);
  printf("Received %d bytes\n", len);
  printf("Received string %s\n", text);

  close(fd);
  return 0;

}

// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <termios.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <string.h>
// #define PORT "/dev/ttyUSB0"


// int main(void){
// struct termios tty; 
// struct termios tty_old; 
// memset (&tty,0, sizeof(tty)); 

// int USB = open(PORT, O_RDWR | O_NOCTTY);
// /* Error Handling */
// if ( tcgetattr ( USB, &tty ) != 0 ) { 
//     printf("Error from tcgetattr: %s\n",strerror(errno)); 
//   } 
//   /* Save old tty parameters */
//   tty_old = tty; 
//   /* Set Baud Rate */
//   cfsetospeed (&tty, (speed_t)B9600); 
//   cfsetispeed (&tty, (speed_t)B9600); 
//   /* Setting other Port Stuff */
//   tty.c_cflag &= ~PARENB; 
//   // Make 8n1
//   tty.c_cflag &= ~CSTOPB; 
//   tty.c_cflag &= ~CSIZE; 
//   tty.c_cflag |= CS8; 
//   tty.c_cflag &= ~CRTSCTS; 
//   // no flow control
//   tty.c_cc[VMIN] = 2; 
//   // read doesn't block
//   tty.c_cc[VTIME] = 5; 
//   // 0.5 seconds read timeout
//   tty.c_cflag |= CREAD | CLOCAL; 
//   // turn on READ & ignore ctrl lines/* Make raw */
//   cfmakeraw(&tty); 
//   /* Flush Port, then applies attributes */
//   tcflush( USB, TCIFLUSH ); 
//   if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) { 
//     printf("Error from tcgetattr: %s\n",strerror(errno)); 
//   }
//   unsigned char cmd[] = "Bin;"; 
//   int n_written = 0, spot = 0; 
//   printf("Writing\n");
//   do { n_written = write( USB, &cmd[spot], 1 ); 
//   spot += n_written; } 
//   while (cmd[spot-1] != '\r' && n_written > 0);
//   int n = 0;
//   spot = 0; 
//   printf("Finished writing\n");
//   char buf = '\0'; 
//   /* Whole response*/
//   char response[1024]; 
//   printf("Reading\n");
//   memset(response, '\0', sizeof response); 
//   do { n = read( USB, &buf, 1 ); 
//   sprintf( &response[spot], "%c", buf ); 
//   spot += n; 
//   } while( buf != '\r' && n > 0); 
//   printf("Finished reading\n");
//   if (n < 0) { 
//     printf("Error from tcgetattr: %s\n",strerror(errno)); 
//   } 
//   else if (n == 0) { 
//   printf("Read nothing\n"); 
//   } 
//   else { 
//     printf("Response: %s\n", response); 
//   }

// }
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <termios.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <string.h>
// #define PORT "/dev/ttyUSB0"
// int main(void) { 
//   struct termios s_alicat; 
//   int tty_fd, count; 
//   char receive[255], transmit[5]; 
//   // Ports connected in USB as sudo 
//   if ((tty_fd = open(PORT, O_RDWR | O_NOCTTY)) < 0) { 
//     printf("Line failed to open with %d\n", tty_fd); 
//     return -1; 
//   } else { 
//     printf("fd is %d\n", tty_fd); 
//   } 
//   if (tcgetattr(tty_fd, &s_alicat) < 0) { 
//     printf("Error from tcgetattr: %s\n", strerror(errno)); 
//     return -1; 
//   } 
//   cfsetospeed(&s_alicat, (speed_t)B9600); 
//   cfsetispeed(&s_alicat, (speed_t)B9600); 
//   // Set up receiver and set to local mode 
//   s_alicat.c_cflag |= (CLOCAL | CREAD | CS8); 
//   s_alicat.c_iflag |= IGNPAR | ICRNL; 
//   // Ignore parity errorss 
//   tcflush(tty_fd, TCIFLUSH); 
//   //discard file information not transmitted 
//   if (tcsetattr(tty_fd, TCSANOW, &s_alicat) != 0) { 
//     printf("Error from tcsetattr: %s\n", strerror(errno)); 
//     return -1; 
//   } 
//   // Clear the port before kicking off communications 
//   // strcpy(transmit, "\r\r"); 
//   // write(tty_fd, transmit, 2); 
//   printf("Writing to device\n"); 
//   strcpy(transmit, "Bin;"); 
//   if ((count = write(tty_fd, transmit, 2)) < 0) { 
//     printf("Failed to write to device"); 
//   } 
//   int j = 0; count = 0;
//   printf("About to read\n");  
//   /* Attempt to read data at most 3 times if there is no data * coming back. */ 
//   while (count == 0 && j < 3) { 
//     usleep(100000); 
//     if ((count = read(tty_fd, receive, sizeof(receive) - 1)) < 0) { 
//       printf("Error receiving text %d\n", count); 
//     } else { 
//       printf("count is %d\n", count); 
//       receive[count] = 0; 
//       printf("String is %s", receive); 
//     } 
//     j++; 
//   } 
//   printf("Closing port...\n"); 
//   int p = 0; 
//   if ((p = close(tty_fd)) < 0) { 
//     printf("Port failed to close %d\n", p); 
//     return -1; 
//   } 
//   return 0; 
// }


// // #include <stdlib.h>
// // #include <stdio.h>
// // #include <termios.h>
// // #include <unistd.h>
// // #include <sys/stat.h>
// // #include <fcntl.h>
// // #include <errno.h>
// // #include <string.h>

// // #define PORT "/dev/ttyUSB0"

// // int main(void)
// // {

// //   struct termios s_alicat;

// //   int tty_fd, count;

// //   char receive[255], transmit[5];

// //   // Ports connected in USB as sudo
// //   if ((tty_fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){
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
