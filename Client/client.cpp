#include "message.h"

int main(int argc, char const *argv[])
{
    /* create socket descriptor 'sockfd' */
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    /* connect socket 'sockfd' to server */
    char ipaddr[16] = "127.0.0.1";
    struct sockaddr_in servaddr; /* server socket addr */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                /* use the Internet addr family */
    servaddr.sin_addr.s_addr = inet_addr(ipaddr); /* connect to server's IP */
    servaddr.sin_port = htons(6789);              /* use server's port 6789 */
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    
    FileListUpdateRequest message;
    message.n_files = 2;
    File file;

    file.filename_length = 2;
    file.filename = "ab";
    file.file_size = 3521;
    for (int i = 0; i < 16; i++) file.md5[i] = i+65;
    message.file_list.push_back(file);

    file.filename_length = 5;
    file.filename = "bc1D6";
    file.file_size = 5672;
    message.file_list.push_back(file);

    message.Write(sockfd);
    
    /* close the socket */
    close(sockfd);
    return 0;
}
