#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#include "socketsFunction.h"

//#define PORTA 8081    /* Porta para conectar */
#define MAXBUFFERSIZE 256 /* máximo número de bytes que poderemos enviar
                           por vez */

// void error(const char *msg)
// {
//     perror(msg);
//     exit(EXIT_FAILURE);
// }

int main(int argc, char *argv[])
{
    int clientSocket, charCount, portnum;
    struct hostent *server;
    struct sockaddr_in server_address;
    char buffer[MAXBUFFERSIZE], msg[MAXBUFFERSIZE], c;

    int r;

    srand(getpid() * ((unsigned) time(NULL)));

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

    clientSocket = createSocket(); 

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portnum);
    server_address.sin_addr = *((struct in_addr *) server->h_addr); // ou  *((struct in_addr *) server->h_addr); 

    connectToServer(clientSocket, &server_address);

    bzero(buffer, MAXBUFFERSIZE);

    if((charCount = readFromSocket(clientSocket, buffer, MAXBUFFERSIZE)) < 0) 
    {
        error("ERROR reading from socket");
    }
    buffer[charCount] = '\0';
    printf("Recebido do servidor: %s", buffer);

    while(1)
    {   
        bzero(buffer, MAXBUFFERSIZE);
        bzero(msg, MAXBUFFERSIZE);
        r = rand() % 100;
        // sprintf(buffer, "%d", (int) r);
        sprintf(buffer, "%d", r);
        strcpy(msg, "3");
        strcat(msg, buffer);
        msg[strlen(msg)] = '\0';

        if(writeToSocket(clientSocket, msg) < 0) 
        {   
            error("ERROR writing to socket");
        }
        usleep(1300000);
    }
    close(clientSocket);

    return 0;
}
