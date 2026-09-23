// tcpcli.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "vars.h"

#define MAXLINE 4096
#define SA struct sockaddr
#define SERVER_PORT 80

static ssize_t readn(int fd, void *vptr, size_t n)
{
	size_t nleft = n;
	ssize_t nread;
	char *ptr = vptr;

	while (nleft > 0)
	{
		size_t nleft = n;
		ssize_t nread;
		char *ptr = vptr;

		while (nleft > 0)
		{
			if ((nread = read(fd, ptr, nleft)) < 0)
			{
				if (errno == EINTR)
					nread = 0;
				else
					return -1;
			}
			else if (nread == 0)
			{
				break;
			}
			nleft -= nread;
			ptr += nread;
		}
		return n - nleft;
	}
}

static ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t nleft = n;
	ssize_t nwritten;
	const char *ptr = vptr;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

static void str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE];
	struct args args;
	struct result result;

	while (fgets(sendline, MAXLINE, fp) != NULL)
	{
		if (sscanf(sendline, "%ld%ld", &args.arg1, &args.arg2) != 2)
		{
			printf("invalid input: %s", sendline);
			continue;
		}

		if (writen(sockfd, &args, sizeof(args)) < 0)
		{
			perror("writen");
			return;
		}

		if (readn(sockfd, &result, sizeof(result)) == 0)
		{
			fprintf(stderr, "str_cli: server terminated prematurely\n");
			return;
		}

		printf("%ld\n", result.sum);
	}
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s ", argv[0]);
		return EXIT_FAILURE;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
		fprintf(stderr, "inet_pton: '%s' is not a valid IPv4 address\n", argv[1]);
		close(sockfd);
		return EXIT_FAILURE;
	}
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		fprintf(stderr, "connect to %s:%d: %s\n",
				argv[1], SERVER_PORT, strerror(errno));
		close(sockfd);
		return EXIT_FAILURE;
	}
	str_cli(stdin, sockfd); // ← stdin как FILE*, а не fd

	close(sockfd);
	return EXIT_SUCCESS;
}