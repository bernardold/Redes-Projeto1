#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "socketsFunctions.h"

//#define MINHAPORTA 8081   /* Porta que os usuarios irão se conectar*/
#define BACKLOG 5     /* Quantas conexões pendentes serão indexadas */
#define MAXBUFFERSIZE 256

char result[256];
float info[2] = {0.0};
int opt = -1;

void connectionHandler(int);

int main(int argc, char **argv)
{
    int serverSocket, clientSocket;  /* escuta em serverSocket, nova conexão
                                   em clientSocket */
    int portnum;
    struct sockaddr_in server_address;    /* informação do meu endereco */
    struct sockaddr_in client_address; /* informação do endereco do conector */
    int clientLength;
    char buffer[MAXBUFFERSIZE];
    int pid;

    int i = 0, opt = -1;

    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        error("ERROR opening socket");
    }

    portnum = atoi(argv[1]);

    bzero((char *) &server_address, sizeof(server_address)); /* Zera o resto da estrutura */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portnum);
    server_address.sin_addr.s_addr = INADDR_ANY; // ou inet_addr("127.0.0.1") /* coloca IP automaticamente */

    if(bind(serverSocket, (struct sockaddr *) &server_address, sizeof(struct sockaddr)) == -1) 
    {
        error("ERROR on binding socket");
    }

    if(listen(serverSocket, BACKLOG) < 0) 
    {
        error("ERROR on listening");
    }

    printf("Servidor ativo!\n");

    clientLength = sizeof(client_address);
    while(1) 
    {   
            if((clientSocket = accept(serverSocket, (struct sockaddr *) &client_address, &clientLength)) < 0)
            {
                error("ERROR on accept");
            }
            printf("Servidor: chegando conexão de %d\n", inet_ntoa(client_address.sin_addr));
            i++;

            pid = fork();
            if(pid < 0) 
            {   
                error("ERROR on fork");
            }
            else if(pid == 0) // apenas os processos filhos entram aqui
            {   
                close(serverSocket);
                // HANDLE INICIO
                connectionHandler(clientSocket);
                // HANDLE FIM
                printf("Servidor: terminada a conexao com %d\n", inet_ntoa(client_address.sin_addr));
                exit(0);
            }
            printf("i: %d\n", i);
            if(i == 2)
            {
                while(1)
                {
                    printf("entrou!\n");
                    scanf("%d", &opt);
                }
            }
            close(clientSocket);
            while(waitpid(-1, NULL, WNOHANG) > 0); /* Limpa os processos filhos */
    }
    return 0;
}

void connectionHandler(int clientSocket)
{
    char buffer[MAXBUFFERSIZE];
    char id[2];

    if(write(clientSocket, "Seja bem-vindo!\n", 16) < 0)
    {
        perror("ERROR on writing to socket");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        bzero(buffer, MAXBUFFERSIZE);  // zera o buffer
        printf("opt: %d\n", opt);
        if(opt == 0)
        {
            if(writeToSocket(clientSocket, "C0") < 0)
            {
                perror("ERROR on writing to socket");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }

            if(readFromSocket(clientSocket, buffer, MAXBUFFERSIZE) < 0) 
            {
                error("ERROR reading from socket");
            }

            id[0] = buffer[0];
            id[1] = buffer[1];
            if(!strcmp(id, "C0"))
            {    
                printf("Client 0 aqui. ");
                printf("Here is the message: %s\n", buffer);
            }
            opt = -1;
        }
        else if(opt == 1)
        {
            if(writeToSocket(clientSocket, "C1") < 0)
            {
                perror("ERROR on writing to socket");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }

            if(readFromSocket(clientSocket, buffer, MAXBUFFERSIZE) < 0) 
            {
                error("ERROR reading from socket");
            }

            id[0] = buffer[0];
            id[1] = buffer[1];
            if(!strcmp(id, "C1"))
            {    
                printf("Client 1 aqui. ");
                printf("Here is the message: %s\n", buffer);
            }
            opt = -1;
        }
        else if(opt == 5)
        {
            exit(0);
        }
    }
    close(clientSocket);   
}