#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#include "socketsFunction.h"

int main(int argc, char *argv[])
{
    int clientSocket, portnum;
    struct hostent *server;
    struct sockaddr_in server_address;
    char buffer[MAXBUFFERSIZE], msg[MAXBUFFERSIZE];

    srand(getpid() * ((unsigned) time(NULL)));

    int numPessoas = (rand() % 20) + 20;

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

    printf("Sensor Classe Executiva conectado\n");

    while(1)
    {   
        bzero(buffer, MAXBUFFERSIZE);
        bzero(msg, MAXBUFFERSIZE);
        if(rand() % 2 == 0)
            numPessoas += 1;
        else
            numPessoas -= 1;
        if(numPessoas > 40)
            numPessoas = 40;
        else if(numPessoas < 20)
            numPessoas = 20;
        sprintf(buffer, "%d", numPessoas);
        strcpy(msg, "4");
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
