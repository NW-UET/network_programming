#ifndef message_h
#define message_h

#include <cstdint>
#include <string>
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

/* hàm Write viết lại, luôn cố gắng viết được n bytes */
void Write(int sockfd, const void *buff, size_t n);
/* hàm Read viết lại, luôn cố gắng đọc được n bytes */
void Read(int sockfd, void *buff, size_t n);
/* hàm ReadHeader sẽ đọc Header của thông báo và trả về kiểu của thông báo đó */
uint8_t ReadHeader(int sockfd);
/* viết kiểu của thông báo dưới dạng chữ, trả về xâu rỗng nếu ko đúng bất kỳ kiểu nào */
string typeString(uint8_t type);

/* Mỗi một đối tượng thông báo đều có 4 hàm sau đây */

/* Msg.Write(sockfd): trích suất các thông tin có trong Msg rồi viết vào sockfd */
// int Write(int sockfd);

/* Msg.Read(sockfd): đọc header của thông báo, nếu đúng loại thông báo với Msg thì
 * thì tiếp tục đọc payload để lấy thông tin, chỉ nên dúng nếu biết trước loại thông
 * báo tiếp theo
 * trả về type của Header, để dễ dàng chuyển loại đối tượng gói tin nếu cần
 */
// int Read(int sockfd);

/* Msg.ReadPayload(sockfd): chỉ đọc payload của thông báo, chỉ nên dùng khi biết
 * chắc chắn đúng loại thông báo, kết hợp với dùng ReadHeader(sockfd) trước để kiểm tra
 */
// int ReadPayload(int sockfd);

/* Msg.print(): in ra màn hình thông tin của thông báo */
// void print();

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

struct Filesize
{
    uint16_t filename_length;
    string filename;
    uint64_t file_size;
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
    int ReadPayload(int sockfd);
    void print();
};

/* FILE_LIST_UPDATE_RESPONSE == 1 */
struct FileListUpdateResponse
{
    uint8_t type = FILE_LIST_UPDATE_RESPONSE;
    uint8_t n_files;
    vector<Filestatus> filestatus_list;
    int Write(int sockfd);
    int Read(int sockfd);
    int ReadPayload(int sockfd);
    void print();
};

/* LIST_FILES_REQUEST == 2 */
struct ListFilesRequest
{
    uint8_t type = LIST_FILES_REQUEST;
    int Write(int sockfd);
    int Read(int sockfd);
    int ReadPayload(int sockfd);
    void print();
};

/* LIST_FILES_RESPONSE == 3 */
struct ListFilesResponse
{
    uint8_t type = LIST_FILES_RESPONSE;
    uint8_t n_files;
    vector<Filesize> filesize_list;
    int Write(int sockfd);
    int Read(int sockfd);
    int ReadPayload(int sockfd);
    void print();
};

/* LIST_HOSTS_REQUEST == 4 */
struct ListHostsRequest
{
    uint8_t type = LIST_HOSTS_REQUEST;
    Filename filename;
    int Write(int sockfd);
    int Read(int sockfd);
    int ReadPayload(int sockfd);
    void print();
};

/* LIST_HOSTS_RESPONSE == 5 */
struct ListHostsResponse
{
    uint8_t type = LIST_HOSTS_RESPONSE;
    uint8_t n_hosts;
    vector<uint32_t> IP_addr_list;
    int Write(int sockfd);
    int Read(int sockfd);
    int ReadPayload(int sockfd);
    void print();
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
    int ReadPayload(int sockfd);
    void print();
};

/* DOWNLOAD_FILE_RESPONSE == 7 */
struct DownloadFileResponse
{
    uint8_t type = DOWNLOAD_FILE_RESPONSE;
    int Write(int sockfd);
    int Read(int sockfd);
    int ReadPayload(int sockfd);
    void print();
};

#endif