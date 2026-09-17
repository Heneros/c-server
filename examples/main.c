
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


int main(int argc, char **argv)
{
  int sockfd, n;
   	char				recvline[MAXLINE + 1];
  struct sockaddr_in6 servaddr;

  if(argc != 2)
  {
   fprintf(stderr, "Usage: %s <IPaddress>\n", argv[0]);
    return EXIT_FAILURE;
  }
  if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
  {
      perror("socket error");
    return EXIT_FAILURE; 
  }
  memset(&servaddr, 0, sizeof(servaddr));
  	servaddr.sin6_family = AF_INET6;	
  servaddr.sin6_port   = htons(HTTP_PORT);
if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0) {
      fprintf(stderr, "inet_pton error for %s: not a valid IPv4 address\n", argv[1]);
          close(sockfd);
    return EXIT_FAILURE;
}
if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
fprintf(stderr, "connect to %s: %s\n", argv[1], strerror(errno));
close(sockfd);
    return EXIT_FAILURE;
}
printf("Connected to %s\n", argv[1]);   
while((n = read(sockfd, recvline, MAXLINE)) > 0){
  	recvline[n] = 0;
     
    if(fputs(recvline, stdout) == EOF){
          perror("fputs error");
    }
}
  return EXIT_SUCCESS;
}
