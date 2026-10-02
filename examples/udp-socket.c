#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // UDP

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (1)
    {
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n > 0)
        {
            buffer[n] = '\0';
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            int client_port = ntohs(client_addr.sin_port);
            printf("Получено от %s:%d: %s\n", client_ip, client_port, buffer);
            const char *reply = "Привет от сервера!";

            sendto(sockfd, reply, strlen(reply), 0, (struct sockaddr *)&client_addr, client_len);
        }
        else if (n == -1)
        {
            perror("recvfrom");
        }
    }
    close(sockfd);
    return 0;
}