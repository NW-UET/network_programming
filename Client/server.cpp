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
    printf("Reading..");
    fflush(stdout);

    message.Read(clisock);
    printf("Complete\nMessage received:\n");
    printf("type = %d\n", message.type);
    printf("nfiles = %d\n", message.n_files);
    vector<File> file_list = message.file_list;
    for (vector<File>::iterator it = file_list.begin(); it != file_list.end(); ++it)
    {
        printf("filename_length = %d\n", it->filename_length);
        string filename = it->filename;
        printf("filename = ");
        cout << filename;
        printf("\n");
        printf("file_size = %ld\n", it->file_size);
        printf("md5 = ");
        for (int i = 0; i < 16; i++)
            printf("%0x",it->md5[i]);
        printf("\n");
    }

    ListHostsRequest message2;
    message2.Read(clisock);
    printf("type = %d\n", message2.type);
    printf("filename_length = %d\n", message2.filename.filename_length);
    cout << message2.filename.filename << endl;

    FileListUpdateResponse message3;
    message3.n_files = 2;
    Filestatus filestatus;
    filestatus.filename_length = 7;
    filestatus.filename = "abcdefg";
    filestatus.status = 0;
    message3.filestatus_list.push_back(filestatus);
    filestatus.filename_length = 2;
    filestatus.filename = "ae";
    filestatus.status = 1;
    message3.filestatus_list.push_back(filestatus);
    message3.Write(clisock);

    ListFilesResponse message4;
    message4.n_files = 2;
    Filename filename;
    filename.filename_length = 3;
    filename.filename = "uio";
    message4.filename_list.push_back(filename);
    filename.filename_length = 6;
    filename.filename = "rtypoi";
    message4.filename_list.push_back(filename);
    message4.Write(clisock);

    ListHostsResponse message5;
    message5.n_hosts = 3;
    message5.IP_addr_list.push_back(2147483648);
    message5.IP_addr_list.push_back(2684354560);
    message5.IP_addr_list.push_back(2684356608);
    message5.Write(clisock);

    /* close the socket */
    close(clisock);
    close(sockfd);
    return 0;
}