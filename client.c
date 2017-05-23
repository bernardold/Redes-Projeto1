/* Programa desenvolvido por Nash Leon 
   Thanks Ramona e unsekurity team.
   http://unsekurity.virtualave.net
*/
/* Headers */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORTA 8081    /* Porta para conectar */
#define MAXDATASIZE 100 /* máximo número de bytes que poderemos enviar
                           por vez */

int main(int argc, char *argv[])
{
    int Meusocket, numbytes;
    char buffer[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in seu_endereco;
    int portnum;

    if (argc != 3) {
        fprintf(stderr,"Uso: cliente hostname porta\n");
        exit(1);
    }

    portnum = atoi(argv[2]);

    if ((he=gethostbyname(argv[1])) == NULL)   /* envia host info */
    {
        herror("gethostbyname");
        exit(1);
    }

    if ((Meusocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }

    seu_endereco.sin_family = AF_INET;
    seu_endereco.sin_port = htons(portnum);
    seu_endereco.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(seu_endereco.sin_zero), 8);

    if (connect(Meusocket,(struct sockaddr *)&seu_endereco, sizeof(struct sockaddr)) ==-1) 
    {
        perror("connect");
        exit(1);
    }

    printf("Please enter the message: ");
    bzero(buffer, MAXDATASIZE);
    fgets(buffer, MAXDATASIZE, stdin);
    //n = write(Meusocket, buffer, strlen(buffer));
    if (write(Meusocket, buffer, strlen(buffer)) < 0) 
    {   
        perror("ERROR writing to socket");
        exit(1);
    }    

    //bzero(buffer, 100);
    if ((numbytes = read(Meusocket, buffer, MAXDATASIZE)) < 0) 
    {
        perror("read");
        exit(1);
    }

    buffer[numbytes] = '\0';
    printf("Recebido: %s",buffer);
    close(Meusocket);

    return 0;
}
