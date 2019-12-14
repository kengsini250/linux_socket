#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX 20

int main(int argc, char **argv)
{
    int socket_id;
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id < 0)
    {
        printf("socket_id error\n");
        exit(0);
    }
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(55555);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    int con_flag;
    con_flag = connect(socket_id, (struct sockaddr *)&server, sizeof(server));
    if (con_flag < 0)
    {
        printf("connect error\n");
        exit(1);
    }

    while (1)
    {
        char msg[MAX];
        memset(&msg, 0, MAX);
        printf("send msg : ");
        scanf("%s",&msg[0]);
        if (msg[0] == '!')
            break;
        int send_flag = send(socket_id, msg, MAX, 0);
        if (send_flag < 0)
        {
            perror("send error");
            exit(1);
        }
        else
        {
            printf("send ok\n");
        }
    }
    close(socket_id);
}