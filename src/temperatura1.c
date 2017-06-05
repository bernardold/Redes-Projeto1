#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#include "socketsFunction.h"

int main(int argc, char *argv[])
{
    int clientSocket, portnum;
    struct hostent *server;
    struct sockaddr_in server_address;
    char buffer[MAXBUFFERSIZE], msg[MAXBUFFERSIZE];

    srand(getpid() * ((unsigned) time(NULL)));

    float temp = 5.0 + (rand() % 30) + ((rand() % 101) / 10.0);

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

    printf("Sensor Temperatura 1 conectado\n");

    while(1)
    {   
        bzero(buffer, MAXBUFFERSIZE);
        bzero(msg, MAXBUFFERSIZE);
        strcpy(msg, "1");
        if(rand() % 2 == 0)
            temp += 1.1;
        else
            temp -= 1.1;
        if(temp > 30.0)
            temp = 30.0;
        else if(temp < 5.0)
            temp = 5.0;
        // sprintf(buffer, "%d", (int) r);
        sprintf(buffer, "%.1f", temp);
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
