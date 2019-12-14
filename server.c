#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WAIT_CLIENT_MAX 5
#define MAX 20

int main()
{
    int socket_id;
    if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket wrong\n");
        exit(0);
    }

    int on = 1;
    setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(55555);

    int bind_flag;
    bind_flag = bind(socket_id, (struct sockaddr *)&server, sizeof(server));
    if (bind_flag < 0)
    {
        printf("bind error\n");
        exit(1);
    }

    int listen_flag = listen(socket_id, WAIT_CLIENT_MAX);
    if (listen_flag < 0)
    {
        printf("listen error\n");
        exit(1);
    }

    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    while (1)
    {
        socklen_t size = sizeof(client);
        int client_id = accept(socket_id,
                               (struct sockaddr *)(&client),
                               &size);
        if (client_id == -1)
        {
            perror("error\n");
            continue;
        }
        else
        {
            printf("client ip : %s, client port : %d\n",
                   inet_ntoa(client.sin_addr),
                   ntohs(client.sin_port));
            while (1)
            {
                char buffer[MAX];
                memset(buffer, 0, MAX);
                int n = recv(client_id, buffer, MAX, 0);
                if (n < 0)
                {
                    printf("recv error\n");
                    exit(1);
                }else if(n == 0){
                    printf("disconnect\n");
                    close(client_id);
                    break;
                }
                printf("%s\n", buffer);
            }
        }

        break;
    }
    close(socket_id);
}
