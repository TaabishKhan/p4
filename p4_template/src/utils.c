#include "../include/utils.h"
#include "../include/server.h"
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <stdint.h>


int master_fd;                // Global file descriptor for the server socket
pthread_mutex_t socket_mutex; // Mutex lock for the server socket
struct sockaddr_in server_addr; // Global sockaddr for the server address


/*
################################################
##############Server Functions##################
################################################
*/

/**********************************************
 * init
   - port is the number of the port you want the server to be
     started on
   - initializes the connection acception/handling system
   - if init encounters any errors, it will call exit().
************************************************/
void init(int port) {
    // Create a socket and save the file descriptor to sd
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Change the socket options to be reusable using setsockopt()
    int opt = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Set socket options failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the provided port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Mark the socket as a passive socket
    if (listen(sd, 20) < 0) {
        perror("Listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Save the file descriptor to the global variable master_fd
    master_fd = sd;

    // Initialize the mutex
    pthread_mutex_init(&socket_mutex, NULL);

    // Log that the server has started
    printf("UTILS.O: Server Started on Port %d\n", port);
    fflush(stdout);
}

/**********************************************
 * accept_connection - takes no parameters
   - returns a file descriptor for further request processing.
   - if the return value is negative, the thread calling
     accept_connection must should ignore request.
***********************************************/
int accept_connection(void) {

   //TODO: create a sockaddr_in struct to hold the address of the new connection
  
   
   /**********************************************
    * IMPORTANT!
    * ALL TODOS FOR THIS FUNCTION MUST BE COMPLETED FOR THE INTERIM SUBMISSION!!!!
    **********************************************/
   
   
   
   // TODO: Aquire the mutex lock

   // TODO: Accept a new connection on the passive socket and save the fd to newsock

   // TODO: Release the mutex lock


   // TODO: Return the file descriptor for the new client connection
   
}


/**********************************************
 * send_file_to_client
   - socket is the file descriptor for the socket
   - buffer is the file data you want to send
   - size is the size of the file you want to send
   - returns 0 on success, -1 on failure 
************************************************/
int send_file_to_client(int socket, char * buffer, int size) 
{
    //TODO: create a packet_t to hold the packet data
 

    //TODO: send the file size packet


    //TODO: send the file data
  
    //TODO: return 0 on success, -1 on failure

}


/**********************************************
 * get_request_server
   - fd is the file descriptor for the socket
   - filelength is a pointer to a size_t variable that will be set to the length of the file
   - returns a pointer to the file data
************************************************/
char * get_request_server(int fd, size_t *filelength)
{
    //TODO: create a packet_t to hold the packet data
 
    //TODO: receive the response packet
  
    //TODO: get the size of the image from the packet

    //TODO: recieve the file data and save into a buffer variable.

    //TODO: return the buffer

}


/*
################################################
##############Client Functions##################
################################################
*/

/**********************************************
 * setup_connection
   - port is the number of the port you want the client to connect to
   - initializes the connection to the server
   - if setup_connection encounters any errors, it will call exit().
************************************************/
int setup_connection(int port)
{
    //TODO: create a sockaddr_in struct to hold the address of the server   

    //TODO: create a socket and save the file descriptor to sockfd
   
    //TODO: assign IP, PORT to the sockaddr_in struct

    //TODO: connect to the server
   
    //TODO: return the file descriptor for the socket
}


/**********************************************
 * send_file_to_server
   - socket is the file descriptor for the socket
   - file is the file pointer to the file you want to send
   - size is the size of the file you want to send
   - returns 0 on success, -1 on failure
************************************************/
int send_file_to_server(int socket, FILE *file, int size) 
{
    //TODO: send the file size packet
   

    //TODO: send the file data
   

    // TODO: return 0 on success, -1 on failure
   
}

/**********************************************
 * receive_file_from_server
   - socket is the file descriptor for the socket
   - filename is the name of the file you want to save
   - returns 0 on success, -1 on failure
************************************************/
int receive_file_from_server(int socket, const char *filename) 
{
    //TODO: create a buffer to hold the file data
    

    //TODO: open the file for writing binary data
   
    
   //TODO: create a packet_t to hold the packet data
    
   //TODO: receive the response packet
  

    //TODO: get the size of the image from the packet
   
   

   //TODO: recieve the file data and write it to the file
    
    //TODO: return 0 on success, -1 on failure
}
