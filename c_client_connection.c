#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_PORT 5000

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("Usage: TCPClient <IP address of the server");
        exit(1);
    }
    // Creating a socket using the socket function
    // Sock: Socket file descriptor
    // AF_INET: Address family used for the socket in this case IPv4
    // SOCK_STREAM: The type of sockt to create in this case a stream socket since it is a connection-oriented socket
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
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // Converts a port number to network byte order
    server_addr.sin_port = htons(SERVER_PORT);

    // Conecting to the server using the connect function
    // Sock: Socket file descriptor
    //(struct sockaddr*)&server_addr: A pointer to the server information
    // sizeof(server_addr): the size of the server information struct
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server\n");

    // Variable for storing name
    char name[40];
    // Variable for storing registration_number
    char registration_number[20];
    // Variable for storing serial number
    int serial_number;

    // Capturing serial number from user and storing it a variable
    printf("Enter your serialnumber:(MiddlepartofregnoLastpartofregno) ");
    scanf("%d", &serial_number);
    // Capturing registration number from user and storing it in a variable
    printf("Enter your registration number: ");
    scanf("%s", registration_number);
    // Capturing full name from user and storing it in a variable
    printf("Enter your full name:(FirstnameLastname) ");
    scanf("%s", name);

    // Constructing a message(client_data) to send to the server by concatenating user's input into a single string
    char client_data[100];
    sprintf(client_data, "%d,%s,%s", serial_number, registration_number, name);

    // Sending the message to the server using the send function
    if (send(sock, client_data, strlen(client_data), 0) < 0)
    {
        perror("send");
        exit(EXIT_FAILURE);
    }

    // Variable for receiving response from the server
    char server_response[100];

    // Receiving response from the server
    int j = recv(sock, server_response, sizeof(server_response), 0);
    if (j < 0)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("%s\n", server_response);
        bzero(server_response, sizeof(server_response));
    }

    // Close the socket file descriptor
    close(sock);

    return 0;
}
