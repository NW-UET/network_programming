#ifndef message_h
#define message_h

#include <cstdint>
#include <string>
#include <strings.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <endian.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define FILE_LIST_UPDATE_REQUEST 0
#define FILE_LIST_UPDATE_RESPONSE 1
#define LIST_FILES_REQUEST 2
#define LIST_FILES_RESPONSE 3
#define LIST_HOSTS_REQUEST 4
#define LIST_HOSTS_RESPONSE 5
#define DOWNLOAD_FILE_REQUEST 6
#define DOWNLOAD_FILE_RESPONSE 7

using namespace std;

void Write(int sockfd, const void *buff, size_t n);
void Read(int sockfd, void *buff, size_t n);

struct Filename
{
    uint16_t filename_length;
    string filename;
};

struct File
{
    uint16_t filename_length;
    string filename;
    uint64_t file_size;
    unsigned char md5[16];
};

struct Filestatus
{
    uint16_t filename_length;
    string filename;
    uint8_t status;
};

/* client - server*/

/* FILE_LIST_UPDATE_REQUEST == 0 */
struct FileListUpdateRequest
{
    uint8_t type = FILE_LIST_UPDATE_REQUEST;
    uint8_t n_files;
    vector<File> file_list;
    int Write(int sockfd);
    int Read(int sockfd);
};

/* FILE_LIST_UPDATE_RESPONSE == 1 */
struct FileListUpdateResponse
{
    uint8_t type = FILE_LIST_UPDATE_RESPONSE;
    uint8_t n_files;
    vector<Filestatus> filestatus_list;
};

/* LIST_FILES_REQUEST == 2 */
struct ListFilesRequest
{
    uint8_t type = LIST_FILES_REQUEST;
    /*no payload no data*/
    int Write(int sockfd);
    int Read(int sockfd);
};

/* LIST_FILES_RESPONSE == 3 */
struct ListFilesResponse
{
    uint8_t type = LIST_FILES_RESPONSE;
    uint8_t n_files;
    vector<Filename> filename_list;
};

/* LIST_HOSTS_REQUEST == 4 */
struct ListHostsRequest
{
    uint8_t type = LIST_HOSTS_REQUEST;
    Filename filename;
    int Write(int sockfd);
    int Read(int sockfd);
};

/* LIST_HOSTS_RESPONSE == 5 */
struct ListHostsResponse
{
    uint8_t type = LIST_HOSTS_RESPONSE;
    uint8_t n_hosts;
    vector<uint32_t> IP_addr_list;
};

/* client - client*/

/* DOWNLOAD_FILE_REQUEST == 6 */
struct DownloadFileRequest
{
    uint8_t type = DOWNLOAD_FILE_REQUEST;
    Filename filename;
    uint32_t offset;
    int Write(int sockfd);
    int Read(int sockfd);
};

/* DOWNLOAD_FILE_RESPONSE == 7 */
struct DownloadFileResponse
{
    uint8_t type = DOWNLOAD_FILE_RESPONSE;
    /*no payload have data*/
};

#endif