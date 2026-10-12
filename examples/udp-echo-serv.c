#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "vars.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int i, n;
    char sendline[DGLEN];
    n = 1024 * 1024;

    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &n, sizeof(n));

    memset(sendline, 'a', sizeof(sendline));
    for (i = 0; i < NDG; i++)
    {
        if (sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen) < 0)
        {
            perror("sendto");
            return;
        }
    }
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        printf("usage ");
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1024);

    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));

    close(sockfd);
    exit(0);
}