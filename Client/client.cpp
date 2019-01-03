#include "message.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <openssl/md5.h>

#define BLOCK_SIZE 2048

static void *requestThread(void *arg);
static void *responseThread(void *arg);
static void *downloadThread(void *arg);
void sendFile(int const clisock, string const filename, uint32_t const offset, uint64_t const size);
void receiveFile(int const sockfd, string const filename, uint32_t const offset, uint64_t const size);

struct FileToDownload
{
    uint32_t sockfd;
    uint32_t offset;
    uint64_t size;
    uint8_t shardId;
    string *filename_ptr;
};

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
        /* print client's IP and port on screen */
        printf("Client's IP: %s\n", inet_ntoa(cliaddr.sin_addr));
        printf("Client's port: %d\n", ntohs(cliaddr.sin_port));
        /* create a passive thread */
        pthread_t tid;
        pthread_create(&tid, NULL, &responseThread, (void *)clisock_ptr);
    }
    /* close the socket */
    close(serverfd);
    return 0;
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int createRequestSocket(const struct sockaddr_in *servaddr)
{
    /* create socket descriptor 'sockfd' */
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    /* receive timeout */
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    /* connect socket 'sockfd' to server */
    if (connect(sockfd, (struct sockaddr *)servaddr, sizeof(*servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    return sockfd;
}

static void *requestThread(void *arg)
{
    char ipaddr[16] = "127.0.0.1";
    struct sockaddr_in servaddr; /* server socket addr */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                /* use the Internet addr family */
    servaddr.sin_addr.s_addr = inet_addr(ipaddr); /* connect to server's IP */
    servaddr.sin_port = htons(6789);              /* use server's port 6789 */
    while (1)
    {
        cout << "Input your action:" << endl;
        cout << "1 = Update file list\t2 = List files\t3 = Download file" << endl;
        cout << "Your action: ";
        char input;
        cin >> input;
        cout << "----" << endl;
        switch (input)
        {
        case '1':
        {
            int sockfd = createRequestSocket(&servaddr);

            FileListUpdateRequest file_list_update_request;
            /*take list_file*/
            DIR *folder;
            string dir = string("Share");
            //open folder Share
            struct dirent *d_file;
            if ((folder = opendir(dir.c_str())) == NULL)
            {
                cout << "Error(" << errno << ") opening " << dir << endl;
            }
            else
            {
                int count = 0; // n_files
                while ((d_file = readdir(folder)) != NULL)
                {
                    File file;
                    //filename
                    file.filename = string(d_file->d_name);
                    //filename_length
                    file.filename_length = file.filename.size();
                    //file_size
                    if (is_regular_file((dir + "/" + file.filename).c_str()))
                    {
                        FILE *inFile = fopen((dir + "/" + file.filename).c_str(), "rb");
                        cout << file.filename << endl;
                        if (inFile == NULL)
                        {
                            printf("%s can't be opened.\n", file.filename.c_str());
                            file.file_size = 0;
                        }
                        else
                        {
                            fseek(inFile, 0, SEEK_END);
                            file.file_size = ftell(inFile);
                            rewind(inFile);
                        }
                        //md5 (file)
                        unsigned char c[MD5_DIGEST_LENGTH];
                        MD5_CTX mdContext;
                        int bytes;
                        unsigned char data[1024];
                        MD5_Init(&mdContext);
                        while ((bytes = fread(data, 1, 1024, inFile)) != 0)
                            MD5_Update(&mdContext, data, bytes);
                        MD5_Final(c, &mdContext);
                        fclose(inFile);
                        for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
                        {
                            file.md5[i] = c[i];
                        }
                        // add file to list
                        file_list_update_request.file_list.push_back(file);
                        count++;
                    }
                }
                closedir(folder);

                file_list_update_request.n_files = count;
                /*send to server*/
                file_list_update_request.Write(sockfd);
                file_list_update_request.print();
            }
            /* close the socket */
            close(sockfd);
            break;
        }
        case '2':
        {
            int sockfd = createRequestSocket(&servaddr);
            ListFilesRequest list_files_request;
            list_files_request.Write(sockfd);
            ListFilesResponse list_files_response;
            list_files_response.Read(sockfd);
            // list_files_response.print();
            cout << "Number of files: ";
            cout << (int)list_files_response.n_files << endl;
            for (uint8_t i = 0; i < list_files_response.n_files; i++)
            {
                Filesize filesize = list_files_response.filesize_list.at(i);
                cout << filesize.filename << "......."
                     << filesize.file_size << "byte(s)" << endl;
            }
            /* close the socket */
            close(sockfd);
            break;
        }
        case '3':
        {
            uint8_t n_hosts = 0;
            string filename;
            uint32_t offset = 0;
            uint64_t size = 0;
            uint64_t file_size = 0;
            uint8_t n_shards = 0;
            cout << "Input your file name: ";
            cin >> filename;
            int sockfd = createRequestSocket(&servaddr);
            // send list hosts request
            ListHostsRequest list_hosts_request;
            list_hosts_request.filename.filename = filename;
            list_hosts_request.filename.filename_length = filename.size();
            list_hosts_request.Write(sockfd);
            // receive list hosts response
            ListHostsResponse list_hosts_response;
            list_hosts_response.Read(sockfd);
            file_size = list_hosts_response.file_size;
            n_hosts = list_hosts_response.n_hosts;
            cout << "File size: " << (long)file_size << endl;
            cout << "Number of hosts: ";
            cout << (int)n_hosts << endl;
            for (uint8_t i = 0; i < n_hosts; i++)
            {
                struct in_addr in_addr;
                in_addr.s_addr = (in_addr_t)(list_hosts_response.IP_addr_list.at(i));
                cout << inet_ntoa(in_addr) << endl;
            }
            /* close the socket */
            close(sockfd);
            // create download client address
            struct sockaddr_in dlhost;
            bzero(&dlhost, sizeof(dlhost));
            dlhost.sin_family = AF_INET;
            dlhost.sin_addr.s_addr = (in_addr_t)(list_hosts_response.IP_addr_list.front());
            dlhost.sin_port = htons(9876);
            // receive file
            offset = 0;
            n_shards = 2;
            for (uint8_t i = 0; i < n_shards; i++)
            {
                // send request
                if (i == n_shards - 1)
                    size = (file_size / n_shards) + (file_size % n_shards) - n_shards + 1;
                else
                    size = file_size / n_shards + 1;
                // create arg
                FileToDownload *ftd = (FileToDownload *)malloc(sizeof(FileToDownload));
                ftd->sockfd = createRequestSocket(&dlhost);
                ftd->offset = offset;
                ftd->filename_ptr = new string(filename);
                ftd->shardId = i;
                ftd->size = size;
                // create a download thread
                pthread_t tid;
                pthread_create(&tid, NULL, &downloadThread, (void *)ftd);
                pthread_join(tid, NULL);
                offset += size;
            }
            // append file
            // open wfile
            char wfilePath[140] = "Download/";
            strcat(wfilePath, filename.c_str());
            FILE *wfile = fopen(wfilePath, "wb");
            char buffer[BLOCK_SIZE + 1];
            if (wfile != NULL)
            {
                for (uint8_t i = 0; i < n_shards; i++)
                {
                    // open file
                    char rfilePath[140] = "Download/";
                    strcat(rfilePath, (filename + to_string(i)).c_str());
                    FILE *rfile = fopen(rfilePath, "rb");
                    // get size
                    if (i == n_shards - 1)
                        size = (file_size / n_shards) + (file_size % n_shards) - n_shards + 1;
                    else
                        size = file_size / n_shards + 1;
                    // append to wfile
                    while (size > 0)
                    {
                        size_t bytesRead = fread(buffer, 1, BLOCK_SIZE, rfile);
                        fwrite(buffer, sizeof(char), bytesRead, wfile);
                        size -= bytesRead;
                    }
                    fclose(rfile);
                    remove(rfilePath);
                }
            }
            fclose(wfile);
            break;
        }
        }
        cout << endl;
    }
    return NULL;
}

static void *responseThread(void *arg)
{
    int clisock = *((int *)arg);
    free(arg);
    pthread_detach(pthread_self());
    DownloadFileRequest download_file_request;
    download_file_request.Read(clisock);
    string filename = download_file_request.filename.filename;
    uint32_t offset = download_file_request.offset;
    uint64_t fsize = download_file_request.size;
    char filePath[140] = "Share/";
    strcat(filePath, filename.c_str());
    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
        return NULL;
    DownloadFileResponse download_file_response;
    download_file_response.Write(clisock);
    sendFile(clisock, filename, offset, fsize);
    close(clisock);
    return NULL;
}

static void *downloadThread(void *arg)
{
    FileToDownload ftd = *((FileToDownload *)arg);
    free(arg);
    pthread_detach(pthread_self());
    DownloadFileRequest download_file_request;
    download_file_request.filename.filename = *ftd.filename_ptr;
    download_file_request.filename.filename_length = (*ftd.filename_ptr).size();
    download_file_request.offset = ftd.offset;
    download_file_request.size = ftd.size;
    download_file_request.Write(ftd.sockfd);
    // set timeout
    // receive response
    DownloadFileResponse download_file_response;
    download_file_response.Read(ftd.sockfd);
    string filename = (*ftd.filename_ptr) + to_string((int)ftd.shardId);
    receiveFile(ftd.sockfd, filename, ftd.offset, ftd.size);
    free(ftd.filename_ptr);
    close(ftd.sockfd);
    return NULL;
}

void sendFile(int const clisock, string const filename, uint32_t const offset, uint64_t const size)
{
    /* open file */
    char filePath[140] = "Share/";
    strcat(filePath, filename.c_str());
    FILE *file = fopen(filePath, "rb");
    /* read file */
    fseek(file, offset, SEEK_SET);
    char buffer[BLOCK_SIZE + 1];
    long sizeToRead = size;
    while (sizeToRead > 0)
    {
        int blockSize;
        if (sizeToRead >= BLOCK_SIZE)
        {
            blockSize = BLOCK_SIZE;
        }
        else
        {
            blockSize = sizeToRead;
        }
        fread(buffer, 1, blockSize, file);
        /* send file to client */
        int nbytes = write(clisock, buffer, blockSize);
        if (nbytes < 0)
        {
            perror("write");
            exit(1);
        }
        if (nbytes > 0)
            sizeToRead -= nbytes;
    }
    printf("Size of file = %ld byte(s)\n\n", size);
    fclose(file);
}

void receiveFile(int const sockfd, string const filename, uint32_t const offset, uint64_t const size)
{
    /* open file */
    char filePath[140] = "Download/";
    strcat(filePath, filename.c_str());
    FILE *file = fopen(filePath, "wb");
    if (file == NULL)
    {
        perror("file");
        exit(1);
    }
    /* receive file from server */
    long sizeToRead = size;
    while (sizeToRead > 0)
    {
        int blockSize;
        if (sizeToRead >= BLOCK_SIZE)
        {
            blockSize = BLOCK_SIZE;
        }
        else
        {
            blockSize = sizeToRead;
        }
        char fileData[BLOCK_SIZE];
        int fileDatalen = read(sockfd, fileData, blockSize);
        if (errno == EWOULDBLOCK)
            return;
        if (fileDatalen < 0)
        {
            perror("read");
            exit(1);
        }
        fwrite(fileData, sizeof(char), fileDatalen, file);
        if (fileDatalen > 0)
            sizeToRead -= fileDatalen;
    }
    fclose(file);
}