#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SERVER_PORT 3007

void handle_connection(int j);

int main(int argc, char **argv)
{
    // creating a socket using the socket function
    // Sock: Socket file descriptor
    // AF_INET: Address family used for the socket in this case IPv4
    // SOCK_STREAM: The type of socket to create in this case a stream socket since it is a connection-oriented socket
    // 0: The protocol to use this case, the default protocol for the given address family and socket type
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Creating a server_addr struct that will store information about the server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    // The address family used for the socket
    server_addr.sin_family = AF_INET;
    // Converts string IP address to binary form
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Converts a port number to network byte order
    server_addr.sin_port = htons(SERVER_PORT);

    // binding the socket to the server information using the bind function
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections. 8:Maximum number of pending connections
    if (listen(sock, 8) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", SERVER_PORT);

    // Infinite loop to handle client connections
    while (1)
    {

        // Creating a client_addr struct that will store information about the client
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Accept incoming connections using the accept function
        // Sock: Socket file descriptor
        //(struct sockaddr*)&client_addr: Pointer to the client information
        //&client_addr_len: Size of the client information
        int j = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (j < 0)
        {
            perror("accept");
            continue;
        }

        // creating child processes
        pid_t pid = fork();
        if (pid == 0)
        {
            // child process
            close(sock);
            handle_connection(j);
            close(j);
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
            // parent process
            close(j);
        }
        else
        {
            perror("fork");
        }
    }
    // close the socket file descriptor
    close(sock);

    return 0;
}

void handle_connection(int j)
{

    // Recieve data from the client using the recv function and storing it in client_data variable
    char client_data[110];
    int n = recv(j, client_data, sizeof(client_data), 0);
    if (n < 0)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    client_data[100] = '\0';

    // Process client request
    // Checking if the data received already exists int the file

    // Tokenize the string in the client_data_copy variable
    // token = strtok(client_data_copy,",");
    // Extract the serial number(1st token) and store it in a variable called ser
    // ser = atoi(token);

    // Open the file in read mode
    FILE *fp = fopen("registration.txt", "r");
    // Check if the file was opened successfully
    if (fp == NULL)
    {
        printf("Error opening file!");
    }

    // Read each line from the file and check if received record already exists
    char line[100];
    while (fgets(line, 100, fp))
    {
        // char line_details variable that will hold each line in the file that will be used to compare with clien_data;
        char line_details[100];
        sscanf(line, "%s", line_details);
        if (strcmp(client_data, line_details) == 0)
        {
            // Record already exists in the file
            // close the file
            fclose(fp);
            // Send reply to the client informing client that their record already exists in the file
            send(j, "Record already exists!!!", strlen("Record already exists!!!"), 0);
            // close the socket file descriptor
            close(j);
        }
    }

    // Record does not exist in the file
    //  Close the file
    fclose(fp);

    // Open the file in append mode
    fopen("registration.txt", "a");
    // Check if the file was opened successfully
    if (fp == NULL)
    {
        printf("Error opening file!");
    }
    // Write the contents of what was recieved from the client into the file
    fprintf(fp, "%s\n", client_data);
    // close the file
    fclose(fp);
    // Send reply of success to the client
    send(j, "Records have been saved successfully", strlen("Records have been saved successfully"), 0);
}
