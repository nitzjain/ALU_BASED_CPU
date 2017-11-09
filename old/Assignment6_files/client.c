/* Assignment 6 Echo Client/Server
Group - SS05*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>

int main(int argc,char **argv)
{
    int sockfd,n;
    char sendline[100];
    char recvline[300];
    struct sockaddr_in servaddr;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(22000);

    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));

    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    while(1)
    {
        bzero( sendline, 100);
        bzero( recvline, 300);
        printf("\n+++++++++++++++++++++++++++++++++++++++++++++++");
        printf("\n Enter new string to be send to server:");
        fgets(sendline,100,stdin); /*stdin = 0 , for standard input */

        printf("\n**********************************************");
        printf("\n Response from Server:");

        printf("\n**********************************************\n");
        write(sockfd,sendline,strlen(sendline)+1);
        read(sockfd,recvline,300);
        printf("%s",recvline);
    }

}