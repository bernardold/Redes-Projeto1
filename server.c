#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//#define MINHAPORTA 8081   /* Porta que os usuarios irão se conectar*/
#define BACKLOG 5     /* Quantas conexões pendentes serão indexadas */
#define MAXBUFFERSIZE 256

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int serverSocket, newSocket;  /* escuta em serverSocket, nova conexão
                                   em newSocket */
    int portnum;
    struct sockaddr_in server_address;    /* informação do meu endereco */
    struct sockaddr_in client_address; /* informação do endereco do conector */
    unsigned int clientLength;
    char buffer[MAXBUFFERSIZE];
    int pid;

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
        if((newSocket = accept(serverSocket, (struct sockaddr *) &client_address, &clientLength)) < 0)
        {
            error("ERROR on accept");
        }
        printf("Servidor: chegando conexão de %s\n", inet_ntoa(client_address.sin_addr));

        pid = fork();
        if(pid < 0) 
        {   
            error("ERROR on fork");
        }
        else if(pid == 0) // apenas os processos filhos entram aqui
        {   
            close(serverSocket);
            // HANDLE INICIO
            if(write(newSocket, "Seja bem-vindo!\n", 16) < 0)
            {
                perror("ERROR on writing to socket");
                close(newSocket);
                exit(EXIT_FAILURE);
            }
            while(1)
            {
                bzero(buffer, MAXBUFFERSIZE);  // zera o buffer
                if(read(newSocket, buffer, MAXBUFFERSIZE) < 0) 
                {
                    error("ERROR reading from socket");
                }
                if(!strcmp(buffer, "exit"))
                    break;
                printf("Here is the message: %s\n", buffer);
            }
            close(newSocket);
            // HANDLE FIM
            printf("Servidor: terminada a conexao com %s\n", inet_ntoa(client_address.sin_addr));
            exit(0);
        }
        close(newSocket);
        while(waitpid(-1, NULL, WNOHANG) > 0); /* Limpa os processos filhos */
    }
    return 0;
}