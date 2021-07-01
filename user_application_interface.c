/**
 * @file   user_application_interface.c
 * @author Sonu Verma
 * @date   24 April 2021
 * @version 0.1
 * @brief  	user space application to interact with the mentioned device driver
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 512               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];

   printf("USERSPACE: Starting device driver test code example from user space..\n");

   fd = open("/dev/pen0", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("USERSPACE: Failed to open the device...");
      return errno;
   }
   printf("USERSPACE: Type in a short string to send to the kernel module:\n");

   scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)
   printf("USERSPACE: Writing message to the device [%s].\n", stringToSend);
   ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
   if (ret < 0){
      perror("USERSPACE: Failed to write the message to the device.");
      return errno;
   }

   printf("USERSPACE: Press ENTER to read back from the device...\n");
   getchar();

   printf("USERSPACE: Reading from the device...\n");
   ret = read(fd, receive, strlen(stringToSend));        // Read the response from the LKM
   if (ret < 0){
      perror("USERSPACE: Failed to read the message from the device.");
      return errno;
   }
   printf("USERSPACE: The received message is: [%s]\n", receive);
   printf("USERSPACE: End of the program\n");
   return 0;
}
