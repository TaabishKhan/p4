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
   struct sockaddr_in client_addr;
   socklen_t client_len = sizeof(client_addr);
   
   /**********************************************
    * IMPORTANT!
    * ALL TODOS FOR THIS FUNCTION MUST BE COMPLETED FOR THE INTERIM SUBMISSION!!!!
    **********************************************/
   
   
   
   // TODO: Aquire the mutex lock
   if(pthread_mutex_lock(&socket_mutex) != 0) {
    perror("Failed to acquire mutex lock");
    return -1; // Return a negative value to indicate an error
   }
   // TODO: Accept a new connection on the passive socket and save the fd to newsock
   int newsock = accept(master_fd, (struct sockaddr*)&client_addr, &client_len);
   if (newsock < 0) {
      perror("Accept failed");
      pthread_mutex_unlock(&socket_mutex); // Ensure the mutex is unlocked even on failure
      return -1; // Return a negative value to indicate an error
   }
   // TODO: Release the mutex lock
   if (pthread_mutex_unlock(&socket_mutex) != 0) {
    perror("Failed to release mutex lock");
    close(newsock); // Close the socket if unlocking fails
    return -1; // Return a negative value to indicate an error
   }

   // TODO: Return the file descriptor for the new client connection
   printf("UTILS.O: Connection accepted from %s:%d\n",
   inet_ntoa(client_addr.sin_addr),
   ntohs(client_addr.sin_port));
   fflush(stdout);

   return newsock;
  
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
    packet_t packet;
    packet.size = size;

    // Send the packet with the file size first and error check
    if (send (socket, &packet, sizeof(packet_t), 0) < 0) {
        perror("Failed to send file size packet");
        return -1; // Failed Transmission
    }

    int bytes_sent = 0;
    // Loop for sending data
    while (bytes_sent < size) {
        int result = send(socket, buffer + bytes_sent, size - bytes_sent, 0);

        // Error check the last sent file data
        if (result < 0) {
            perror("Failed to send file data");
            return -1; // Failed transmission
        }

        // Increment sent counter
        bytes_sent += result;
    }
    return 0; // Successful transmission
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
    packet_t packet;
    packet.size = *filelength;
 
    //TODO: receive the response packet
    if (recv(fd, filelength, sizeof(size_t), 0) <= 0) {
        perror("Failed to receive file size");
        return NULL;
    }
  
    //TODO: get the size of the image from the packet
    *filelength = packet.size;

    //TODO: recieve the file data and save into a buffer variable.
    char *buffer = (char *)malloc(*filelength);
    if (!buffer) {
        perror("Failed to allocate memory for file data");
        return NULL;
    }

    size_t bytes_received = 0;
    while (bytes_received < *filelength) {
      ssize_t result = recv(fd, buffer + bytes_received, *filelength - bytes_received, 0);
      if (result <= 0) {
          perror("Failed to receive file data");
          free(buffer);
          return NULL;
      }
      bytes_received += result;
    }

    //TODO: return the buffer
    return buffer;
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
    int sockfd;
    struct sockaddr_in server_addr;

    //TODO: create a socket and save the file descriptor to sockfd
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Socket creation failed");
      exit(EXIT_FAILURE);
    }
   
    //TODO: assign IP, PORT to the sockaddr_in struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);

    //TODO: connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      perror("Connection to server failed");
      close(sockfd);
      exit(EXIT_FAILURE);
    }
   
    //TODO: return the file descriptor for the socket
    return sockfd;
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
    if (send(socket, &size, sizeof(size), 0) < 0) {
      perror("Failed to send file size");
      return -1;
    }

    //TODO: send the file data
    char buffer[1024];
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(socket, buffer, bytes_read, 0) < 0) {
            perror("Failed to send file data");
            return -1;
        }
    }
   

    // TODO: return 0 on success, -1 on failure
    return 0;
   
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
    char buffer[1024];
    

    //TODO: open the file for writing binary data
    FILE *file = fopen(filename, "wb");

    // Error checking
    if (!file) {
        perror("Failed to open write file");
        return -1;
    }
   
    
   //TODO: create a packet_t to hold the packet data
   packet_t packet;
    
   //TODO: receive the response packet
  if (recv (socket, &packet.size, sizeof(packet.size), 0) <= 0){

    // Error
    perror("Failed to receive the file's size");
    fclose(file);
    return -1;
  }

  //TODO: get the size of the image from the packet
  size_t total_bytes_received = 0;
  size_t file_size = packet.size;
   

   //TODO: recieve the file data and write it to the file
    
    //TODO: return 0 on success, -1 on failure
    return 0;
}

