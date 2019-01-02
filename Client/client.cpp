#include "message.h"

static void *requestThread(void *arg);
static void *responseThread(void *arg);

int main(int argc, char const *argv[])
{
    /* create a active thread */
    pthread_t active_id;
    pthread_create(&active_id, NULL, &requestThread, NULL);

    /* create listening socket for downloading */
    /* create socket descriptor 'serverfd' */
    int serverfd;
    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    /* reuse port */
    int opval = 1;
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEPORT, &opval, sizeof(opval)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    /* bind socket 'serverfd' to socket address */
    struct sockaddr_in servaddr; /* server socket addr */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                     /* use the Internet addr family */
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* use localhost addr*/
    servaddr.sin_port = htons(9876);                   /* bind ‘serverfd’ to port 1026 */
    if (bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    /* listen for connections */
    if (listen(serverfd, 5) < 0)
    {
        perror("listen");
        exit(1);
    }
    while (1)
    {
        struct sockaddr_in cliaddr;
        bzero(&cliaddr, sizeof(cliaddr));
        socklen_t cliaddr_len = sizeof(cliaddr);
        int *clisock_ptr = (int *)malloc(cliaddr_len);
        *clisock_ptr = accept(serverfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if ((*clisock_ptr) < 0)
        {
            perror("accept");
            exit(1);
        }
        /* create a passive thread */
        pthread_t tid;
        pthread_create(&tid, NULL, &responseThread, (void *)clisock_ptr);
    }
    /* close the socket */
    close(serverfd);
    return 0;
}

static void *requestThread(void *arg)
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

    while (1)
    {
        // FileListUpdateRequest file_list_update_request;
        // file_list_update_request.n_files = 3;
        // File file;
        // file.filename = "abc";
        // file.file_size = 555;
        // file.filename_length = file.filename.size();
        // for (int i = 0; i < 16; i++)
        //     file.md5[i] = 97 + i;
        // file_list_update_request.file_list.push_back(file);
        // file.filename = "test";
        // file.file_size = 1000;
        // file.filename_length = file.filename.size();
        // for (int i = 0; i < 16; i++)
        //     file.md5[i] = 48 + i;
        // file_list_update_request.file_list.push_back(file);
        // file.filename = "file";
        // file.file_size = 10000;
        // file.filename_length = file.filename.size();
        // file_list_update_request.file_list.push_back(file);
        // file_list_update_request.Write(sockfd);

        // ListFilesRequest list_files_request;
        // list_files_request.Write(sockfd);
        // ListFilesResponse list_files_response;
        // list_files_response.Read(sockfd);
        // list_files_response.print();

        ListHostsRequest list_hosts_request;
        list_hosts_request.filename.filename = "file";
        list_hosts_request.filename.filename_length = 4;
        list_hosts_request.Write(sockfd);
        ListHostsResponse list_hosts_response;
        list_hosts_response.Read(sockfd);
        list_hosts_response.print();

        printf("Enter \"q\" to quit\n");
        if (getchar() == 'q') break;
    }
    /* close the socket */
    close(sockfd);
    return NULL;
}

static void *responseThread(void *arg)
{
    int clisock;
    clisock = *((int *)arg);
    free(arg);
    pthread_detach(pthread_self());
    // send file
    close(clisock);
    return NULL;
}