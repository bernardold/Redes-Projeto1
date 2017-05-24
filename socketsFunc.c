#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

//#define MINHAPORTA 8081   /* Porta que os usuarios irão se conectar*/
#define BACKLOG 5     /* Quantas conexões pendentes serão indexadas */

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/*

    SOCKETS

*/
int createSocket(void)
{
    int socketfd;
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        error("ERROR opening socket");
    }

    return socketfd;
}

int writeToSocket(int socketfd, char *msg)
{
    return write(socketfd, msg, strlen(msg));
}

int readFromSocket(int socketfd, char *buffer, int buffersize)
{
    return read(socketfd, buffer, buffersize);
}

/*

    SERVER

*/
void bindToAddress(int serverSocket, struct sockaddr_in server_address)
{
    if(bind(serverSocket, (struct sockaddr *) &server_address, sizeof(struct sockaddr)) == -1) 
    {
        error("ERROR on binding socket");
    }
}

void listenToConnections(int serverSocket)
{
    if(listen(serverSocket, BACKLOG) < 0) 
    {
        error("ERROR on listening");
    }
}

void acceptConnection(int serverSocket, struct sockaddr_in client_address)
{
    int socket;
    int clientLength = sizeof(client_address);

    if((socket = accept(serverSocket, (struct sockaddr *) &client_address, &clientLength)) < 0)
    {
        error("ERROR on accept");
    }
}

/*

    CLIENT

*/
void connectToServer(int clientSocket, struct sockaddr_in server_address)
{
    if(connect(clientSocket, (struct sockaddr *) &server_address, sizeof(struct sockaddr)) == -1) 
    {
        error("ERROR connecting");
    }
}