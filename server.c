/* Programa desenvolvido por Nash Leon 
   Thanks Ramona e unsekurity team.
   http://unsekurity.virtualave.net
*/
/* Velhos headers de sempre */

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
#define MAXBUFFERSIZE 256

int main(int argc, char **argv)
{
    int serverSocket, newSocket;  /* escuta em serverSocket, nova conexão
                                   em newSocket */
    int portnum;
    struct sockaddr_in server_address;    /* informação do meu endereco */
    struct sockaddr_in client_address; /* informação do endereco do conector */
    int clientLength;
    char buffer[MAXBUFFERSIZE];
    int pid;

    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    serverSocket = createSocket();

    portnum = atoi(argv[1]);

    bzero((char *) &server_address, sizeof(server_address)); /* Zera o resto da estrutura */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portnum);
    server_address.sin_addr.s_addr = INADDR_ANY; // ou inet_addr("127.0.0.1") /* coloca IP automaticamente */

    bindToAddress(serverSocket, server_address);

    listenToConnections();

    printf("Servidor ativo!\n");

    while(1) 
    {   
        newSocket = acceptConnection(serverSocket, client_address);
        printf("Servidor: chegando conexão de %d\n", inet_ntoa(client_address.sin_addr));

        pid = fork();
        if(pid < 0) 
        {   
            error("ERROR on fork");
        }
        else if(pid == 0) // apenas os processos filhos entram aqui
        {   
            close(serverSocket);
            // HANDLE INICIO
            if(writeToSocket(newSocket, "Seja bem-vindo!\n") < 0)
            {
                perror("ERROR on writing to socket");
                close(newSocket);
                exit(EXIT_FAILURE);
            }
            while(1)
            {
                bzero(buffer, MAXBUFFERSIZE);  // zera o buffer
                if(readFromSocket(newSocket, &buffer, MAXBUFFERSIZE) < 0) 
                {
                    error("ERROR reading from socket");
                }
                if(!strcmp(buffer, "exit"))
                    break;
                printf("Here is the message: %s\n", buffer);
            }
            close(newSocket);
            // HANDLE FIM
            printf("Servidor: terminada a conexao com %d\n", inet_ntoa(client_address.sin_addr));
            exit(0);
        }
        close(newSocket);
        while(waitpid(-1, NULL, WNOHANG) > 0); /* Limpa os processos filhos */
    }

    return 0;
}