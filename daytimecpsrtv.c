

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <time.h>
#include	<stdarg.h>	
#include "vars.h" 


//The creation of the TCP socket is identical to the client code.
int main(int argc, char **argv)
{
    int					listenfd, n, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE + 1];
	time_t				ticks;
     int counter = 0;       
    listenfd = socket(AF_INET,  SOCK_STREAM, 0);

    memset(&servaddr,0, sizeof(servaddr));

	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(1024);

    bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    for(;;){

      
        connfd = accept(listenfd, (SA*) NULL, NULL);
        ticks = time(NULL);
snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
  	counter++;
        write(connfd, buff, strlen(buff));
            printf("Sent: %s Number %d", buff, counter);    
            fflush(stdout);

        close(connfd);
    }

}