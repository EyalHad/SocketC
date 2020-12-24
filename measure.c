#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <arpa/inet.h>
#include <stdio.h>

#include <time.h>
#include <netinet/tcp.h>


void getfile(int acceptCon, int i)
{
    char buff[256];

    if (i == 0)
    {
        printf("Cubic \n");
    }
    else if (i == 5)
    {
        printf("Reno \n");
    }
    int counter = 0;
    while (read(acceptCon, buff, 80) > 0)
    {
        counter++;
    }
}

int main()
{
    struct sockaddr_in servAddr, ClinetAddr;
    int sock; 
    
    sock = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock < 0)// -1 if there was an ERROR
    {
        printf("Error IN Opening File .. \n");
        return 0;
    }
    else
    {
        printf("The Socket has been created..\n");
    }

    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5060);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int binds = bind(sock, (struct sockaddr *)&servAddr, sizeof(servAddr));  
    if (binds < 0)// -1 if there was an ERROR
    {
        printf("Socket bind failed...\n");
        return 0;
    }
    else
    {
        printf("Bind Successfull ..\n");
    }

    if ((listen(sock, 5)) < 0) // -1 if there was an ERROR
    {
        printf("Listen hads failed...\n");
        return 0;
    }
    else
    {
        printf("Server is now listening..\n");
    }
    int len = sizeof(ClinetAddr);

    time_t time1;
    time_t time2;
    time_t time3;
    time(&time1);

    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
        {
            time(&time2);
            time_t time = time2 - time1;
            printf("ans cubic = %ld\n", time);
            printf("avg cubic = %ld\n", time / 5);
        }
        if (i >= 5)
        {
            char bif[256];
            socklen_t len;
            strcpy(bif, "reno");
            len = strlen(bif);
            if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, bif, len) < 0)  // -1 if there was an ERROR
            {
                printf("ERROR RENO \n");
                return 0;
            }
        }
        int acceptCon;
        acceptCon = accept(sock, (struct sockaddr *)&ClinetAddr, &len);
        if (acceptCon < 0)  // -1 if there was an ERROR
        {
            printf("accpet failed");
        }else
        {
           printf("The Connection has accepted \n");
        }
        getfile(acceptCon, i);
        if (i == 9)
        {
            time(&time3);
            time_t time4 = time3 - time2;
            printf("ans reno time = %ld\n", time4);
            printf("avgarge reno time = %ld\n", time4 / 5);
        }

        close(acceptCon);
    }
    int OKAY = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &OKAY, sizeof(int)); // Release PORT
    close(sock);

    return 0;
}
