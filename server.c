#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "socketsFunction.h"

//#define MINHAPORTA 8081   /* Porta que os usuarios irão se conectar*/
#define MAXBUFFERSIZE 256
#define N 2

struct ThreadArgs { /* Structure of arguments to pass to client thread */
    int clntSock; /* socket descriptor for client */
};

char client0[MAXBUFFERSIZE];
char client1[MAXBUFFERSIZE];

int main(int argc, char **argv)
{
    int serverSocket[N], clientSocket[N];  /* escuta em serverSocket, nova conexão
                                   em clientSocket */
    int portnum[] = {51111, 51112};
    struct sockaddr_in server_address[N];    /* informação do meu endereco */
    struct sockaddr_in client_address[N]; /* informação do endereco do conector */
    unsigned int clientLength;
    char buffer[2][MAXBUFFERSIZE] = {'\0', '\0'};
    char id;

    pthread_t threadID[N];
    struct ThreadArgs *threadArgs;
    int argu[N];

    int i = 0;

    if(argc != 1) 
    {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < N; i++)
    {
        serverSocket[i] = createSocket();

        bzero((char *) &server_address, sizeof(server_address)); /* Zera o resto da estrutura */
        server_address[i].sin_family = AF_INET;
        server_address[i].sin_port = htons(portnum[i]);
        server_address[i].sin_addr.s_addr = INADDR_ANY; // ou inet_addr("127.0.0.1") /* coloca IP automaticamente */

        bindToAddress(serverSocket[i], &server_address[i]);

        listenToConnections(serverSocket[i]);
    }


    printf("Servidor ativo!\n");

    clientLength = sizeof(client_address[0]);
    i = 0;
    while(1) 
    {   
        while(i < N)
        {
            clientSocket[i] = acceptConnection(serverSocket[i], &client_address[i], &clientLength);
            printf("Servidor: chegando conexão de %s\n", inet_ntoa(client_address[i].sin_addr));
            if(writeToSocket(clientSocket[i], "Connection established with server!\n") < 0)
            {
                perror("ERROR on writing to socket");
                close(clientSocket[i]);
                exit(EXIT_FAILURE);
            }
            i++;
        }
        //else{
        while(1)
        {
            for(i = 0; i < N; i++)
            {
                //bzero(buffer[i], MAXBUFFERSIZE);  // zera o buffer
                if(readFromSocket(clientSocket[i], buffer[i], MAXBUFFERSIZE) < 0) 
                {
                    error("ERROR reading from socket");
                }
                id = buffer[i][0];
                if(id == '0')
                {
                    strcpy(client0, buffer[0] + 1);
                }
                else if(id == '1')
                {
                    strcpy(client1, buffer[1] + 1);
                }
                //usleep(1000000);
                //system("clear");
                printf("Client 0: %s\nClient 1: %s\n", client0, client1);
            }
        }   
    }

    return 0;
}