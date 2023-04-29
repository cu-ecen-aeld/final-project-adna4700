/*

Reference: https://www.geeksforgeeks.org/socket-programming-cc/

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h> //to use syslog
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <syslog.h>
#include <fcntl.h>
#include <pthread.h>   //for multithreading
#include <sys/queue.h> //for linked list
#include <time.h>
#include <sys/time.h>
#include <poll.h>

#define PORT 9000
#define MAX_SIZE 10

void graceful_exit();
int sock_fd = -1, cli_fd;

void graceful_exit()
{
    // Closing the server socket
    if (sock_fd > -1)
    {
       shutdown(sock_fd, SHUT_RDWR);
       close(sock_fd);
       // Closing both recv and send on the server socket
    }
 
    closelog();

}

int main()
{
	openlog("client", 0, LOG_USER);
 
	int received_b = 0; 
	char buffer[10];
	char *ip_addr = "128.0.0.1";
	struct sockaddr_in server;
	

	 // Creating a socket
   	 sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    	 if (sock_fd == -1)
   	 {
      		 printf("Error creating server socket\r\n");
       		 // syslog the errors
       		 syslog(LOG_ERR, "Error creating server socket. Error code:%d\r\n", errno);
        	// return -1 on error
        	graceful_exit();
       		 return -1;
    	}
    	else
    	{
        	printf("Socket created\r\n");
        	syslog(LOG_DEBUG, "Server socket created successfully\r\n");
    	}

	bzero(&server, sizeof(server));

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(ip_addr);
        server.sin_port = htons(PORT);
	
	// client-server socket connection
        cli_fd = connect(sock_fd, (struct sockaddr*)&server, sizeof(server));
	
	if(cli_fd != 0)
	{
		printf("Error in establishing client server connection\r\n");
        	syslog(LOG_DEBUG, "Error in establishing client server connection\r\n");
		graceful_exit();
		exit(0);
	}
	else
	{
        	printf("The client-server connection elstablished\r\n");
        	syslog(LOG_DEBUG, "The client-server connection elstablished\r\n");		
	}
	
	while((received_b = recv(sock_fd, buffer, 10, 0)) > 0)
	{
		    for (int i = 0; i < received_b; i++)
    			{
        			putchar(buffer[i]);
    			}
	}
	if(received_b == -1)
	{
		printf("Error in reading from client\r\n");
        	syslog(LOG_DEBUG, "Error in reading from client\r\n");
		graceful_exit();
		exit(0);		
	}

	graceful_exit();
	return 0;
}







