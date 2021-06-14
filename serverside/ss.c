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
    server.sin_port = htons(8401);//PORT NUMBER
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //SERVER ADDRESS currunt: LOCAL
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

        if (strcmp(command, "ls") == 0)
        {
            system("ls >temps.txt");
            stat("temps.txt", &obj);
            size = obj.st_size;
            send(srvr2, &size, sizeof(int), 0);
            fprocss = open("temps.txt", O_RDONLY);
            sendfile(srvr2, fprocss, NULL, size);
        }

        else if (strcmp(command, "get") == 0)
        {
            sscanf(recvbuflen, "%s%s", fname, fname);
            stat(fname, &obj);
            fprocss = open(fname, O_RDONLY);
            size = obj.st_size;
            if (fprocss == -1)
                size = 0;
            send(srvr2, &size, sizeof(int), 0);
            if (size)
                sendfile(srvr2, fprocss, NULL, size);
        }
        else if (strcmp(command, "put") == 0)
        {
            int c = 0, len;
            sscanf(recvbuflen + strlen(command), "%s", fname);
            recv(srvr2, &size, sizeof(int), 0);
            if (!size)
            {
                    printf("404 file %s not found\n\n", fname);
                    break;
            }
            i = 1;
            while (1)
            {
                fprocss = open(fname, O_CREAT | O_EXCL | O_WRONLY, 0666);
                if (fprocss == -1)
                {
                    sprintf(fname + strlen(fname), "%d", i);
                }
                else
                    break;
            }
            f = malloc(size);
            recv(srvr2, f, size, 0);
            c = write(fprocss, f, size, 0);
            send(srvr2, &c, sizeof(int), 0);
            printf("PUT File\n");
            close(fprocss);
            strcpy(recvbuflen, "cat ");
            strcat(recvbuflen, fname);
            system(recvbuflen);
            printf("\n .\n");
        }

        else if (strcmp(command, "quit") == 0)
        {
            printf("Goodbye!\n");
            i = 1;
            send(srvr2, &i, sizeof(int), 0);
            exit(0);
        }
    }
    return 0;
}
