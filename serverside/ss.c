#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in server, remote_addr;
    struct stat obj;
    int srvr, srvr2;
    char recvbuflen[512], command[5], fname[20], choice[20], *f;
    int k, i, size, len, c, fprocss;
    srvr = socket(AF_INET, SOCK_STREAM, 0);
    if (srvr < 0)
    {
        printf("Can't create socket!");
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(8401);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    k = bind(srvr, (struct sockaddr *)&server, sizeof(server));
    if (k < 0)
    {
        printf("Binding error");
        exit(1);
    }
    k = listen(srvr, 1);
    if (k < 0)
    {
        printf("Listen failed");
        exit(1);
    }
    len = sizeof(remote_addr);
    srvr2 = accept(srvr, (struct sockaddr *)&remote_addr, &len);
    i = 1;
    while (1)
    {
        recv(srvr2, recvbuflen, 100, 0);
        sscanf(recvbuflen, "%s", command);
    

        if (strcmp(command, "quit") == 0)
        {
            printf("Goodbye!\n");
            i = 1;
            send(srvr2, &i, sizeof(int), 0);
            exit(0);
        }
    }
    return 0;
}
