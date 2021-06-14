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
    local_addr.sin_port = htons(8401);//PORT NUMBER
    local_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //SERVER ADDRESS currunt: LOCAL
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

            if (strcmp(choice, "ls") == 0)
            {
                strcpy(recvbuflen, "ls");
                send(server, recvbuflen, 100, 0);
                recv(server, &size, sizeof(int), 0);
                f = malloc(size);
                recv(server, f, size, 0);
                fprocss = creat("temp.txt", S_IRUSR);
                write(fprocss, f, size, 0);
                close(fprocss);
                printf("The remote directory listing is as follows:\n");
                system("cat temp.txt");
            }

            if (strcmp(choice, "get") == 0)
            {
                printf("Please enter name of file: ");
                scanf("%s", fname);
                strcpy(recvbuflen, "get ");
                strcat(recvbuflen, fname);
                send(server, recvbuflen, 100, 0);
                recv(server, &size, sizeof(int), 0);
                if (!size)
                {
                    printf("404 file %s not found\n\n", fname);
                    break;
                }
                f = malloc(size);
                recv(server, f, size, 0);
                int i = 1;
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
                printf("GET File\n");
                write(fprocss, f, size, 0);
                close(fprocss);
                strcpy(recvbuflen, "cat ");
                strcat(recvbuflen, fname);
                system(recvbuflen);
                printf("\n.\n");
            }
            if (strcmp(choice, "put") == 0)
            {
                printf("Please enter name of file: ");
                scanf("%s", fname);
                fprocss = open(fname, O_RDONLY);
                if (fprocss == -1)
                {
                    printf("404 file %s is not on the server\n\n", fname);
                    break;
                }
                strcpy(recvbuflen, "put ");
                strcat(recvbuflen, fname);
                send(server, recvbuflen, 100, 0);
                stat(fname, &obj);
                size = obj.st_size;
                send(server, &size, sizeof(int), 0);
                sendfile(server, fprocss, NULL, size);
                recv(server, &status, sizeof(int), 0);
                printf(&size);
                if (status)
                {
                    printf("%d  Bytes  %s  file retrieved by server and was saved\n", &size, fname);
                    printf("\n.\n");
                }
                else
                    printf("404 cannot saved on server side.\n\n");
            }

            if (strcmp(choice, "quit") == 0)
            {
                strcpy(recvbuflen, "quit");
                send(server, recvbuflen, 100, 0);
                recv(server, &status, 100, 0);
                if (status)
                {
                    printf("Goodbye!\n");
                    exit(0);
                }
            }
            if (strcmp(choice, "h") == 0)
            {
                printf("\nOptions      Discription\n\n");
                printf("get            Download & view file\n"
                       "put            Upload & view file\n"
                       "ls             List files in directory\n"
                       "del            Delete file 'N/A'\n"
                       "quit           Exit\n");
            }
        }
    }
    else
    {
        printf("400 User not found, Please try with another user.\n");
        exit(1);
    }
}
