#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#include "socketsFunction.h"

#define N 10

//  Sensores fisicos
char temperatura0[MAXBUFFERSIZE];
char temperatura1[MAXBUFFERSIZE];
char temperatura2[MAXBUFFERSIZE];
char economica[MAXBUFFERSIZE];
char executiva[MAXBUFFERSIZE];
char primeira[MAXBUFFERSIZE];
char reqfrente[MAXBUFFERSIZE];  // requisicao de algum passageiro na frente do aviao e assim por diante
char reqmeio[MAXBUFFERSIZE];
char reqtras[MAXBUFFERSIZE];
char aeromoca[MAXBUFFERSIZE];
// -------------------------------

//  Sensores virtuais
float temperaturaAviao(float temp0, float temp1, float temp2)
{
    return ((temp0 + temp1 + temp2) / 3.0);
}

int numeroPessoas(int c1, int c2, int c3)
{
    return (c1 + c2 + c3);
}

int aeromocaOcupada(int reqfrente, int reqmeio, int reqtras)
{
    if (reqfrente != 0 || reqmeio != 0 || reqtras != 0) return 1;  // aeromoca foi requisitada, esta' ocupada
    return 0;
}

char *posicaoAeromoca(p) 
{
    if (p == 0) return ("na frente");
    else if (p == 1) return ("no meio");
    else return ("no fundo");
}

int satisfeitos(float tempMedia, int lotacao)
{
    if((tempMedia * 100 / (float) lotacao) > 20) return 1;
    return 0;
}
// -------------------------------

int main(int argc, char **argv)
{
    int serverSocket[N], clientSocket[N];  /* escuta em serverSocket, nova conexão em clientSocket */
    int portnum[] = {51110, 51111, 51112, 51113, 51114, 51115, 51116, 51117, 51118, 51119};
    struct sockaddr_in server_address[N];    /* informação do meu endereco */
    struct sockaddr_in client_address[N]; /* informação do endereco do conector */
    unsigned int clientLength;
    char buffer[N][MAXBUFFERSIZE] = {{'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'}};
    char id;

    float tempMedia;
    int lotacao;

    int i = 0;

    for(i = 0; i < N; i++)
    {
        serverSocket[i] = createSocket();

        bzero((char *) &server_address, sizeof(server_address)); /* Zera o resto da estrutura */
        server_address[i].sin_family = AF_INET;
        server_address[i].sin_port = htons(portnum[i]);
        server_address[i].sin_addr.s_addr = INADDR_ANY; /* coloca IP automaticamente */

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
            if(writeToSocket(clientSocket[i], "Conexao estabelecida com a central!\n") < 0)
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
                    strcpy(temperatura0, buffer[0] + 1);
                }
                else if(id == '1')
                {
                    strcpy(temperatura1, buffer[1] + 1);
                }
                else if(id == '2')
                {
                    strcpy(temperatura2, buffer[2] + 1);
                }
                else if(id == '3')
                {
                    strcpy(economica, buffer[3] + 1);
                }
                else if(id == '4')
                {
                    strcpy(executiva, buffer[4] + 1);
                }
                else if(id == '5')
                {
                    strcpy(primeira, buffer[5] + 1);
                }
                else if(id == '6')
                {
                    strcpy(reqfrente, buffer[6] + 1);
                }
                else if(id == '7')
                {
                    strcpy(reqmeio, buffer[7] + 1);
                }
                else if(id == '8')
                {
                    strcpy(reqtras, buffer[8] + 1);
                }
                else if(id == '9')
                {
                    strcpy(aeromoca, buffer[9] + 1);
                }

                system("clear");

                printf("TEMPERATURA\n");
                tempMedia = temperaturaAviao(atof(temperatura0), atof(temperatura1), atof(temperatura2));
                printf("\tFrente: %.1f ºC\n\tMeio: %.1f ºC\n\tAtra's: %.1f ºC\n", atof(temperatura0), atof(temperatura1), atof(temperatura2));
                printf("\tMe'dia: %.1f ºC (virtual)\n", tempMedia);

                printf("NUMERO DE PESSOAS\n");
                lotacao = numeroPessoas(atoi(economica), atoi(executiva), atoi(primeira));
                printf("\tClasse economica: %s\n\tClasse executiva: %s\n\tPrimeira classe: %s\n", economica, executiva, primeira);
                printf("\tTotal: %d (virtual)\n", lotacao);
            
                printf("STATUS AEROMOÇA\n");
                if(aeromocaOcupada(atoi(reqfrente), atoi(reqmeio), atoi(reqtras))) 
                    printf("\tA aeromoça esta ocupada atendendo alguém %s do aviao.", posicaoAeromoca(atoi(aeromoca)));
                else 
                    printf("\tA aeromoça esta livre!");
                printf(" (virtual)\n");

                printf("SATISFACAO DOS PASSAGEIROS\n");
                if(satisfeitos(tempMedia, lotacao))
                    printf("\tOs passageiros estao satisfeitos com o voo.");
                else
                    printf("\tOS PASSAGEIROS COMECAM A SE SENTIR INSATISFEITOS!");
                printf(" (virtual)\n");

                printf("Pressiona Crtl-C para sair.\n");
            }
        }   
    }

    return 0;
}