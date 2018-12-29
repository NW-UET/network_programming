#include <stdint.h>

#define FILE_LIST_UPDATE_REQUEST 0
#define FILE_LIST_UPDATE_RESPONSE 1
#define LIST_FILES_REQUEST 2
#define LIST_FILES_RESPONSE 3
#define LIST_HOSTS_REQUEST 4
#define LIST_HOSTS_RESPONSE 5
#define DOWNLOAD_FILE_REQUEST 6
#define DOWNLOAD_FILE_RESPONSE 7

struct filename
{
    uint16_t filename_length;
    char * filename;
};

/* message */
struct message
{
    uint8_t type;
    void * payload;
};

/* client - server*/

/* FILE_LIST_UPDATE_REQUEST == 0 */
struct file_list_update_request
{
    uint8_t n_files;
    struct filename filename;
    uint32_t file_size;
    unsigned char md5;
};

/* FILE_LIST_UPDATE_RESPONSE == 1 */
struct file_list_update_response
{
    uint8_t n_files;
    struct filename filename;
    uint8_t status;
};

/* LIST_FILES_REQUEST == 2 */
struct list_files_request
{
    /*no payload*/    
};

/* LIST_FILES_RESPONSE == 3 */
struct list_files_response
{
    uint8_t n_files;
    struct filename * filename_list;
};

/* LIST_HOSTS_REQUEST == 4 */
struct list_hosts_request
{
    uint16_t filename_length;
    char * filename;
};

/* LIST_HOSTS_RESPONSE == 5 */
struct list_hosts_response
{
    uint8_t n_hosts;
    char ** IP_addr;
};

/* client - client*/

/* DOWNLOAD_FILE_REQUEST == 6 */
struct download_file_request
{
    uint16_t filename_length;
    char * filename;
    uint32_t offset;
};

/* DOWNLOAD_FILE_RESPONSE == 7 */
struct download_file_response
{
    /*no payload have data*/
};