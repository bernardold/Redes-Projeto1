#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

//#define PORTA 8081    /* Porta para conectar */
#define MAXBUFFERSIZE 256 /* máximo número de bytes que poderemos enviar
                           por vez */

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int clientSocket, charCount, portnum;
    struct hostent *server;
    struct sockaddr_in server_address;
    char buffer[MAXBUFFERSIZE];

    if(argc != 3) 
    {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    portnum = atoi(argv[2]);

    if((server = gethostbyname(argv[1])) == NULL)   /* envia host info */
    {
        error("ERROR (gethostbyname), no such host");
        exit(EXIT_FAILURE);
    }

    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        error("ERROR opening socket");
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portnum);
    server_address.sin_addr = *((struct in_addr *) server->h_addr); // ou  *((struct in_addr *) server->h_addr); 

    if(connect(clientSocket, (struct sockaddr *) &server_address, sizeof(struct sockaddr)) == -1) 
    {
        error("ERROR connecting");
    }

    bzero(buffer, MAXBUFFERSIZE);

    if((charCount = read(clientSocket, buffer, MAXBUFFERSIZE)) < 0) 
    {
        error("ERROR reading from socket");
    }
    buffer[charCount] = '\0';
    printf("Recebido do servidor: %s", buffer);

    while(1)
    {
        printf("Please enter the message: ");
        bzero(buffer, MAXBUFFERSIZE);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer) - 1] = '\0';
        if(write(clientSocket, buffer, strlen(buffer)) < 0) 
        {   
            error("ERROR writing to socket");
        }
        if(!strcmp(buffer, "exit"))
        {
            break;
        }
    }
    close(clientSocket);

    return 0;
}
