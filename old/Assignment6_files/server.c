/* Assignment 6 Echo Client/Server
Group - SS05*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int i = 0;
int space = 0;
int len = 0;

char *strrev(char *str)
{
      char *a, *b;

      if (! str || ! *str)
            return str;
      for (a = str, b = str + strlen(str) - 1; b > a; ++a, --b)
      {
            *a ^= *b;
            *b ^= *a;
            *a ^= *b;
      }
      return str;
}

int main()
{

    char str[100];
    char temp[300];
    char date[100];
    char weekday[10]="Week day";
    char weekend[10]="Week end";
    int listen_fd, comm_fd;

    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero( &servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22000);

    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    while(1)
    {
        printf("\n****************************************************");
        printf("\nServer is ready to handle request.\n");

        bzero( str, 100);

        space = 0;

        read(comm_fd,str,100);

        printf("Client request received at ");

        time_t t = time(NULL);

        struct tm tm = *localtime(&t);

        printf("%d/%d/%d %d:%d:%d. \n", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

        if(tm.tm_wday==0||tm.tm_wday==6)
        {
                sprintf(date,"%d/%d/%d %d:%d:%d %s", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,weekend);
        }
        else
        {
                sprintf(date,"%d/%d/%d %d:%d:%d %s", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,weekday);
        }

        for (i = 0; i < strlen(str); i++)
        {
                if(str[i] == ' ')
                {
                    space++;
                }
        }

        sprintf(temp, "Current date and time: %s.\nLetters count: %d.\nNumber of words: %d.\nAnd Reversed string is: %s", date,strlen(str)-1-space,space+1,strrev(str));

        printf("Sending data to client.\n");

        write(comm_fd, temp, strlen(temp)+1);

    }
}
