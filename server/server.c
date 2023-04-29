/*
References: https://man7.org/linux/man-pages/man2/getsockname.2.html
https://beej.us/guide/bgnet/html/#a-simple-stream-server
https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
https://nullraum.net/how-to-create-a-daemon-in-c/
https://stackoverflow.com/questions/14846768/in-c-how-do-i-redirect-stdout-fileno-to-dev-null-using-dup2-and-then-redirect
https://man7.org/linux/man-pages/man7/queue.7.html
https://www.geeksforgeeks.org/handling-multiple-clients-on-server-with-multithreading-using-socket-programming-in-c-cpp/
https://man7.org/linux/man-pages/man3/slist.3.html
https://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html
https://linux.die.net/man/3/clock_gettime
https://man7.org/linux/man-pages/man3/strftime.3.html
https://www.geeksforgeeks.org/socket-programming-cc/
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

#include <gpiod.h>
#include <math.h>
#include <string.h>

#define MQ7_PIN 27 // Connect MQ7 digital output to Raspberry Pi GPIO 1

#define PORT 9000
#define MAX_SIZE 10
#define BACKLOG (5)

//server socket fd always non negative so initialized to a negative integer
int sock_fd = -1;

void graceful_exit();
bool signal_caught = false;  // initializing to false to indicate there is no SIGTERM or SIGINT caught

 struct gpiod_chip *chip;
 struct gpiod_line *mq7_line;

void signal_handler(int signal)
{
    if ((signal == SIGINT) || (signal == SIGTERM))
    {
        //to indicate that we caught the signal
        signal_caught = true;
        printf("Signal Caught is %d, exiting\n", signal);
        graceful_exit();
        exit(0);
    }
}

void graceful_exit()
{
    // Closing the server socket
    if (sock_fd > -1)
    {
       shutdown(sock_fd, SHUT_RDWR);
       close(sock_fd);
       // Closing both recv and send on the server socket 
   	 gpiod_line_release(mq7_line); // Release GPIO line
         gpiod_chip_close(chip);
    }
 
    closelog();
}


int main(int argc, char *argv[])
{
    openlog("server", 0, LOG_USER);
    printf("1\r\n");

    int cli_fd;
    socklen_t cli_len;
	
    //address variables fior the client socket
    struct sockaddr_in server, client;

    int mq7_value = 0;
	
    char *buffer_n = "CO is in the normal range 69 ppm to 80 ppm\r\n";
    char *buffer_g = "HARMFULL!!! CO level above 100 ppm\r\n";
    printf("2\r\n");
	
    chip = gpiod_chip_open("/dev/gpiochip0"); // Open GPIO chip
    mq7_line = gpiod_chip_get_line(chip, MQ7_PIN); // Get GPIO line
    gpiod_line_request_input(mq7_line, "mq7"); // Set GPIO line as input
    printf("3\r\n");

    // Registering error signals
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    printf("4\r\n");    

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

    int opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("Error in socket set. Error code:%d\r\n", errno);
        syslog(LOG_ERR, "Error in socket()\r\n");
        graceful_exit();
        return -1;
    }
    else
    {
        printf("Socket options set\r\n");
        syslog(LOG_DEBUG, "Server socket created successfully\r\n");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)))
    {
        printf("Binding failed\r\n");
        // print the error message
        syslog(LOG_ERR, "Bind failed. Error: %d\r\n", errno);
        graceful_exit();
        return (-1);
    }
    else
    {
        printf("Binding done\r\n");
    }


    if (listen(sock_fd, BACKLOG) < 0)
    {
        printf("Error in listening. Error code:%d\r\n", errno);
        syslog(LOG_ERR, "Error listening\r\n");
        graceful_exit();
        return -1;
    }
    else
    {
        printf("Waiting for connections.....\r\n");
    }

/**/
 	cli_len = sizeof(client);

        cli_fd = accept(sock_fd, (struct sockaddr *)&client, &cli_len);

        if (cli_fd < 0)
        {
            printf("Error in accept. Error code:%d", errno);
            syslog(LOG_ERR, "Error in accept");
        }
        else
        {
            printf("Connection accepted\n");
            syslog(LOG_ERR, "New Connection accepted");
        }

        // Printing IP address of client
        char address[30]; // To store the IP address converted into string

        // const char *inet_ntop(int af, const void *restrict src,
        const char *ip_addr = inet_ntop(AF_INET, &client.sin_addr, address, sizeof(address));
        int client_port = htons(client.sin_port);
        if (ip_addr == NULL)
        {
            printf("Error in obtaining ip address of the client\r\n");
        }
        else
        {
            printf("Accepted connection from %s %d\r\n", ip_addr, client_port);
            // Log this info
            syslog(LOG_INFO, "Accepted connection from %s %d\r\n", ip_addr, client_port);
        }

/**/
    while (1)
    {
	
	mq7_value = gpiod_line_get_value(mq7_line); // Read GPIO value

        if(mq7_value == 1)
        {
		printf("%s", buffer_n);
		send(cli_fd,"CO is in the normal range 69 ppm to 80 ppm\r\n", sizeof("CO is in the normal range 69 ppm to 80 ppm\r\n"), 0);
	}
	else
	{
		printf("%s", buffer_g);
		send(cli_fd,"HARMFULL!!! CO level above 100 ppm\r\n", sizeof("HARMFULL!!! CO level above 100 ppm\r\n"), 0);
	}

        usleep(100000); // Wait for 100 milliseconds
	
      
    }

    graceful_exit();

 
    return 0;
}
