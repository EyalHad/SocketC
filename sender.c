#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <netinet/tcp.h>

int sendToServer(int sock, int i)
{
    
    char buffer[256];
    memset(buffer, '0', sizeof(buffer));
    if (i == 0)
    {
        printf("cubic\n");
    }
    else if (i == 5)
    {
        printf("reno mode \n");
    }
    FILE *f;
    f = fopen("1gb.txt", "r");
    if (f == NULL)
    {
        printf("Cant open file");
        return 0;
    }
    else
    {
        printf("the file is now open! \n");
    }

    while (fgets(buffer, 80, f) != NULL)
    {
        write(sock, buffer, sizeof(buffer));
    }
    fclose(f);
    printf("The File finnaly read successfully! \n");
}

int main()
{
    struct sockaddr_in servStr;
    servStr.sin_port = htons(5060);
    servStr.sin_family = AF_INET;
    servStr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (inet_pton(AF_INET, "127.0.0.1", &(servStr.sin_addr)) < 0)
    {
        printf("Error inet \n");
        return 0;
    }
    else
    {
        printf("Connection created  \n");
    }

    for (int i = 0; i < 10; i++)
    {

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (i >= 5)
        {
            char bif[256];
            socklen_t len;
            strcpy(bif, "reno");
            len = strlen(bif);
            if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, bif, len) != 0)
            {
                printf("ERROR RENO \n");
                return 0;
            }
        }
        if (sock < 0)
        {
            printf("Set Socket has Faild \n");
            return 0;
        }

        if (connect(sock, (struct sockaddr *)&servStr, sizeof(servStr)) < 0)
        {
            printf("Bad connect");
            return 0;
        }
        else
        {
            printf("Successfull Connect \n");
        }

        sendToServer(sock, i);
        int OKAY = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &OKAY, sizeof(int));
        close(sock);
    }

    return 0;
}
