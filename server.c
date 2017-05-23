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

#define MINHAPORTA 8081   /* Porta que os usuarios irão se conectar*/
#define BACKLOG 10     /* Quantas conexões pendentes serão indexadas */

int main(int argc, char **argv)
{
    int Meusocket, Novosocket;  /* escuta em Meusocket, nova conexão
                                   em Novosocket */
    struct sockaddr_in meu_endereco;    /* informação do meu endereco */
    struct sockaddr_in endereco_dele; /* informação do endereco do conector */
    int tamanho;
    char buffer[100];
    int portnum;

    if ((Meusocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }

    portnum = atoi(argv[1]);

    meu_endereco.sin_family = AF_INET;
    meu_endereco.sin_port = htons(portnum);
    meu_endereco.sin_addr.s_addr = INADDR_ANY; /* coloca IP automaticamente */
    bzero(&(meu_endereco.sin_zero), 8);        /* Zera o resto da estrutura */

    if (bind(Meusocket, (struct sockaddr *) &meu_endereco, sizeof(struct sockaddr)) == -1) 
    {
        perror("bind");
        exit(1);
    }
    if (listen(Meusocket, BACKLOG) < 0) 
    {
        perror("listen");
        exit(1);
    }

    printf("Server UP!\n");

    while(1) 
    {
        tamanho = sizeof(struct sockaddr_in);
        if ((Novosocket = accept(Meusocket, (struct sockaddr *) &endereco_dele, &tamanho)) < 0)
        {
            perror("accept");
            continue;
        }
        printf("Servidor: chegando conexão de %d\n", inet_ntoa(endereco_dele.sin_addr));
        if (!fork()) 
        {
            if (write(Novosocket, "Seja bem vindo!\n", 16) < 0)
            {
                perror("write");
                close(Novosocket);
                exit(0);
            }
            while(1)
            {
                bzero(buffer, 100);
                //n = read(Novosocket, buffer, 255);
                if (read(Novosocket, buffer, 100) < 0) 
                {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                printf("Here is the message: %s\n", buffer);
                if(!strcmp(buffer, "exit"))
                    break;
            }
            printf("Servidor: terminada a conexao com %d\n", inet_ntoa(endereco_dele.sin_addr));
        }
        close(Novosocket);
        while(waitpid(-1, NULL, WNOHANG) > 0); /* Limpa o processo crianca.fork() */
    }

    return 0;
}