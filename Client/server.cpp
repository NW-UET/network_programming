#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.h"

#define BLOCK_SIZE 2048

int main(int argc, char const *argv[])
{
    /* create socket descriptor 'sockfd' */
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    /* reuse port */
    int opval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opval, sizeof(opval)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    /* bind socket 'sockfd' to socket address */
    struct sockaddr_in servaddr; /* server socket addr */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                     /* use the Internet addr family */
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* use localhost addr*/
    servaddr.sin_port = htons(6789);                   /* bind ‘sockfd’ to port 6789 */
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    /* listen for connections */
    if (listen(sockfd, 5) < 0)
    {
        perror("listen");
        exit(1);
    }
    /* accept client connection */
    struct sockaddr_in cliaddr; /* client socket addr */
    bzero(&cliaddr, sizeof(cliaddr));
    socklen_t cliaddr_len = sizeof(cliaddr); /* length of client socket addr */
    int clisock = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    if (clisock < 0)
    {
        perror("accept");
        exit(1);
    }
    /* print client's IP and port on screen */
    printf("Client's IP: %s\n", inet_ntoa(cliaddr.sin_addr));
    printf("Client's port: %d\n", ntohs(cliaddr.sin_port));
    
    FileListUpdateRequest message;
    printf("Reading..\n");
    message.Read(clisock);
    printf("Read..\n");
    printf("type = %d\n",message.type);
    printf("nfiles = %d\n",message.n_files);
    printf("filename_length = %d\n",message.file_list.back().filename_length);
    printf("filename = %c\n",message.file_list.back().filename.back());
    
    /* close the socket */
    close(clisock);
    close(sockfd);
    return 0;
}