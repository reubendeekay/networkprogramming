#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <netdb.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1500

int main() {
    //Creating a socket using the socket function
    //Sock: Socket file descriptor
    //AF_INET: Address family used for the socket in this case IPv4
    //SOCK_DGRAM: The type of sockt to create in this case a datagram socket since it is connectionless
    //0: The protocol to use this case, the default protocol for the given address family and socket type
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    //Creating a server_addr struct that will store information about the server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

	struct hostent *host;   
    //The address family used for the socket
    server_addr.sin_family = AF_INET;
    //Converts a port number to network byte order
    server_addr.sin_port = htons(SERVER_PORT);
    //Gethostbyname function
    host = gethostbyname(SERVER_IP);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);

	
    //Variable for storing name
    char name[40];
    //Variable for storing registration_number
    char registration_number[20];
    //Variable for storing serial number
    int serial_number;

    //Capturing serial number from user and storing it a variable
    printf("Enter your serialnumber:(MiddlepartofregnoLastpartofregno) ");
    scanf("%d", &serial_number);
    //Capturing registration number from user and storing it in a variable 
    printf("Enter your registration number: ");
    scanf("%s", registration_number);
    //Capturing full name form user and storing it in a variable
    printf("Enter your full name:(FirstnameLastname) ");
    scanf("%s", name);
    
    //Constructing a message(client_data) to send to the server by concatenating user's input into a single string
    char client_data[100];
    sprintf(client_data, "%d,%s,%s", serial_number, registration_number, name);
    
    //Sending the message to the server using the sendto function
    int j = sendto(sock, client_data, strlen(client_data), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (j < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

	//Creating a client_addr struct that will store information about the client
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    //Receiving response from the server using recvfrom fucntion and printing it out
    char server_response[100];
    int k = recvfrom(sock, server_response, sizeof(server_response), 0, (struct sockaddr*)&client_addr, &client_addr_len);
    if (k < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }
    else{
    	printf("%s\n",server_response);
    	bzero(server_response,sizeof(server_response));
	}

    //Close the socket file descriptor
    close(sock);

    return 0;
}

