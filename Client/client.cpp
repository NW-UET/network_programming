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
    for (int i = 0; i < 16; i++)
        file.md5[i] = i + 65;
    message.file_list.push_back(file);

    file.filename_length = 5;
    file.filename = "bc1D6";
    file.file_size = 5672;
    message.file_list.push_back(file);

    message.Write(sockfd);

    ListHostsRequest message2;
    message2.filename.filename_length = 3;
    message2.filename.filename = "ghh";
    message2.Write(sockfd);

    FileListUpdateResponse message3;
    message3.Read(sockfd);
    printf("type = %d\n", message3.type);
    printf("nfiles = %d\n", message3.n_files);
    vector<Filestatus> filestatus_list = message3.filestatus_list;
    for (vector<Filestatus>::iterator it = filestatus_list.begin(); it != filestatus_list.end(); ++it)
    {
        printf("filename_length = %d\n", it->filename_length);
        string filename = it->filename;
        printf("filename = ");
        cout << filename;
        printf("\n");
        printf("status = %d", it->status);
        printf("\n");
    }

    ListFilesResponse message4;
    message4.Read(sockfd);
    printf("type = %d\n", message4.type);
    printf("nfiles = %d\n", message4.n_files);
    vector<Filename> filename_list = message4.filename_list;
    for (vector<Filename>::iterator it = filename_list.begin(); it != filename_list.end(); ++it)
    {
        printf("filename_length = %d\n", it->filename_length);
        string filename = it->filename;
        printf("filename = ");
        cout << filename;
        printf("\n");
    }

    ListHostsResponse message5;
    message5.Read(sockfd);
    printf("type = %d\n", message5.type);
    printf("nhosts = %d\n", message5.n_hosts);
    vector<uint32_t> IP_addr_list = message5.IP_addr_list;
    for (vector<uint32_t>::iterator it = IP_addr_list.begin(); it != IP_addr_list.end(); ++it)
    {
        printf("ip_addr = %u\n",*it);
    }

    /* close the socket */
    close(sockfd);
    return 0;
}
