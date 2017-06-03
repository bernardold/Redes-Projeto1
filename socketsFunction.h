#ifndef SOCKETSFUNCTION_H
#define SOCKETSFUNCTION_H

#include <unistd.h>

//#define MINHAPORTA 8081   /* Porta que os usuarios irão se conectar*/
#define BACKLOG 5     /* Quantas conexões pendentes serão indexadas */

void error(const char *msg);

/*

    SOCKETS

*/
int createSocket(void);
int writeToSocket(int socketfd, char *msg);
int readFromSocket(int socketfd, char *buffer, int buffersize);

/*

    SERVER

*/
void bindToAddress(int serverSocket, struct sockaddr_in *server_address);

void listenToConnections(int serverSocket);

int acceptConnection(int serverSocket, struct sockaddr_in *client_address, unsigned int *clientLength);

/*

    CLIENT

*/
void connectToServer(int clientSocket, struct sockaddr_in *server_address);

#endif
