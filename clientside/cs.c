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
    struct sockaddr_in local_addr;
    struct stat obj;
    char recvbuflen[512], command[5], fname[20], choice[20], *f;
    int server, k, size, status, fprocss;
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0)
    {
        printf("Can't create socket!");
        exit(1);
    }
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(8401);
    local_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    k = connect(server, (struct sockaddr *)&local_addr, sizeof(local_addr));
    if (k < 0)
    {
        printf("Connection to server failed, check server connection OR run server side file");
        exit(1);
    }

    char uname[10];
    char upass[10];
    printf("Welcome to Meselhy's file server\n");
    printf("Enter Username & password to continue\n");
    scanf("%s%s", &uname, &upass);
    if (strcmp(uname, "admin") == 0 && strcmp(upass, "admin") == 0)
    {
        
        while (1)
        {
            printf("\nWaiting for command, type h for help.\n");
            scanf("%s", &choice);
            
            if (strcmp(choice, "h") == 0)
            {
                printf("Options                Discription\n\n");
                printf("get                    Download & view file\n"
                       "put		       Upload & view file\n"
                       "ls		       List files in directory 'N/A'\n"
                       "del		       Delete file\n"
                       "quit		       Exit\n");
            }
        }
    }
    else
    {
        printf("400 User not found, Please try with another user.\n");
        exit(1);
    }
}
