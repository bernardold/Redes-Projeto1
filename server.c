#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socketsFunction.h"

#define MAXBUFFERSIZE 256
#define N 3

char client0[MAXBUFFERSIZE];
char client1[MAXBUFFERSIZE];
char client2[MAXBUFFERSIZE];


float temperaturaAviao(float temp0, float temp1, float temp2)
{
    return ((temp0 + temp1 + temp2) / 3.0);
}

int main(int argc, char **argv)
{
    int serverSocket[N], clientSocket[N];  /* escuta em serverSocket, nova conexão
                                   em clientSocket */
    int portnum[] = {51111, 51112, 51113, 51114, 51115};
    struct sockaddr_in server_address[N];    /* informação do meu endereco */
    struct sockaddr_in client_address[N]; /* informação do endereco do conector */
    unsigned int clientLength;
    char buffer[N][MAXBUFFERSIZE] = {{'\0', '\0', '\0'}};
    char id;

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
    while(1) 
    {   
        for(i = 0; i < N; i++)
        {
            clientSocket[i] = acceptConnection(serverSocket[i], &client_address[i], &clientLength);
            printf("Servidor: chegando conexão de %s\n", inet_ntoa(client_address[i].sin_addr));
            if(writeToSocket(clientSocket[i], "Connection established with server!\n") < 0)
            {
                perror("ERROR on writing to socket");
                close(clientSocket[i]);
                exit(EXIT_FAILURE);
            }
        }

        while(1)
        {
            for(i = 0; i < N; i++)
            {
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
                else if(id == '2')
                {
                    strcpy(client2, buffer[2] + 1);
                }
                else if(id == '3')
                {
                    strcpy(client1, buffer[3] + 1);
                }
                else if(id == '4')
                {
                    strcpy(client2, buffer[4] + 1);
                }
                else if(id == '5')
                {
                    strcpy(client1, buffer[5] + 1);
                }
                else if(id == '6')
                {
                    strcpy(client2, buffer[6] + 1);
                }
                //usleep(1000000);
                system("clear");
                printf("TEMPERATURA\n");
                printf("\tFrente: %.1f ºC\n\tMeio: %.1f ºC\n\tAtra's: %.1f ºC\n", atof(client0), atof(client1), atof(client2));
                printf("\tMe'dia: %.1f ºC (virtual)\n", temperaturaAviao(atof(client0), atof(client1), atof(client2)));
            }
        }   
    }

    return 0;
}