
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
#include "vars.h" 

/// @brief retrive address ip and port
/// @brief retrive address ip and port

int main(int argc, char **argv)
{ 
	setvbuf(stdout, NULL, _IONBF, 0);   

	int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	char				buff[MAXLINE];
	time_t ticks;


	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(8080);

	bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);

	for(;;){
		  len = sizeof(cliaddr);
		
		connfd = accept(listenfd, (SA *) &cliaddr, &len);
		// printf("connection from %s, port %d\n",
		// inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
		// ntohs(cliaddr.sin_port)
		// );
		        char ipbuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cliaddr.sin_addr, ipbuf, sizeof(ipbuf));
        printf("connection from %s, port %d\n",
               ipbuf, ntohs(cliaddr.sin_port)); 
		 ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		write(connfd, buff, strlen(buff));
		close(connfd);
	}
}
