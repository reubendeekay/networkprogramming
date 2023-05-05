#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 1500

int main()
{
    // creating a socket using the socket function
    // Sock: Socket file descriptor
    // AF_INET: Address family used for the socket in this case IPv4
    // SOCK_DGRAM: The type of socket to create in this case a datagram socket since it is a connectionless socket
    // 0: The protocol to use this case, the default protocol for the given address family and socket type
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
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
    // Converts a port number to network byte order
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // binding the socket to the server information using the bind function
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Server is ready to receive from port %d...\n", SERVER_PORT);

    // Creating a client_addr struct that will store information about the client
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Recieve data from the client using the recvfrom function and storing it in client_data variable
    char client_data[110];
    int n = recvfrom(sock, client_data, sizeof(client_data), 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (n < 0)
    {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    client_data[100] = '\0';

    // Process client request
    // Checking if the data received already exists in the file
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
        // char line_details variable that will hold each line in the file that will be used to compare with client_data;
        char line_details[100];
        sscanf(line, "%s", line_details);
        if (strcmp(client_data, line_details) == 0)
        {
            // Record already exists in the file
            // close the file
            fclose(fp);
            // Formulate and send reply to the client informing client that their record already exists
            int j = sendto(sock, "Record already exists!!!", strlen("Record already exists!!!"), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
            if (j < 0)
            {
                perror("sendto");
                exit(EXIT_FAILURE);
            }
            close(j);
            // close the socket file descriptor
            close(sock);
            return 1;
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

    // Write the contents of what was received from the client into the file
    fprintf(fp, "%s\n", client_data);

    // Close the file
    fclose(fp);

    // Formulate and Send response of success to the client
    int j = sendto(sock, "Records have been saved sucessfully", strlen("Records have been saved successfully"), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (j < 0)
    {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    close(j);
    // Close the socket file descriptor
    close(sock);
    return 0;
}
