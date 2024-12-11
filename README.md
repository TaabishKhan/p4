# p4
Project: Multi-Threaded File Transmission System
CSCI 4061 - Fall 2024

Group Members

    Taabish Khan: khan0882@umn.edu
    Omar Yasin: yasin023@umn.edu
    Michael Sharp: sharp474@umn.edu

Compilation
To clean and compile the project:
make clean
make

    make clean: Removes compiled files and clears output directories.
    make: Compiles all source code and links the server and client executables.

Execution
Start the Server
./server <port>

    <port>: The port number where the server listens for client requests.

Example:
./server 8000

    8000: Server listens on port 8000.

Run the Client
./client <server_port>

    <server_port>: Port number to connect to the server.

Example:
./client 8000

    8000: Connects to the server on port 8000.

Example:
Command Line
$ ./server 8000 database 50 50 20
$ ./client img 8000 output

File Descriptions

    server.c: Implements server functionalities, including initializing the server, accepting connections, and transmitting files to clients.
    client.c: Implements client functionalities, including connecting to the server and receiving files.
    utils.h: Contains helper functions and data structures shared between the server and client.
    Makefile: Automates the compilation of the server and client programs.

Key Features
Server Functions

    init(port): Initializes the server socket, binds it to the provided port, and sets it to listen for connections.
    accept_connection(): Handles incoming client connections using a thread-safe mechanism with mutex locks.
    send_file_to_client(socket, buffer, size): Sends file data to a connected client.

Client Functions

    setup_connection(port): Sets up a connection to the server at the specified port.
    send_file_to_server(socket, file, size): Sends file data to the server.
    receive_file_from_server(socket, filename): Receives a file from the server and saves it to the specified location.

Group Contributions

    Taabish Khan: Designed thread-safe mechanisms for connection handling using mutex locks. Implemented core server functions for initialization and client communication.
    Omar Yasin: Optimized data transmission functions for efficient file transfers. Debugged and resolved issues in socket communication.
    Michael Sharp: Handled file input/output operations, ensuring robust error handling and memory management for file buffers.


To parallelize individual requests in your program, you can create a new thread for each incoming client connection. The server will:

    Accept a connection: Wait for a client to connect using a loop.
    Create a thread: For each accepted connection, spawn a new thread to handle the request.
    Process the request: The thread will handle all operations for the specific client (e.g., reading data, sending responses).
    Close the connection: Once the request is complete, the thread will close the client connection and exit.

This approach ensures that multiple client requests can be processed simultaneously, improving efficiency and responsiveness.