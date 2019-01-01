#include "message.h"

using namespace std;

int FileListUpdateRequest::Write(int sockfd)
{
    // size
    size_t size = sizeof(this->type) + sizeof(this->n_files);
    vector<File> file_list = this->file_list;
    for (vector<File>::iterator it = file_list.begin(); it != file_list.end(); ++it)
    {
        size += sizeof(it->filename_length);
        size += sizeof(it->file_size);
        size += it->filename.size();
        size += sizeof(it->md5);
    }
    // allocate memory
    char *begin = (char *)malloc(size);
    bzero((void *)begin, size);
    char *ptr = begin;
    // type
    memcpy(ptr, &this->type, sizeof(this->type));
    ptr = ptr + sizeof(this->type);
    // n_files
    memcpy(ptr, &this->n_files, sizeof(this->n_files));
    ptr = ptr + sizeof(this->n_files);
    // file_list
    for (vector<File>::iterator it = file_list.begin(); it != file_list.end(); ++it)
    {
        // file.filename_length
        uint16_t filename_length = htons(it->filename_length);
        memcpy(ptr, &filename_length, sizeof(filename_length));
        ptr = ptr + sizeof(filename_length);
        // file.filename
        memcpy(ptr, it->filename.data(), it->filename.size());
        ptr = ptr + it->filename.size();
        // file.file_size
        uint64_t file_size = htobe64(it->file_size);
        memcpy(ptr, &file_size, sizeof(file_size));
        ptr = ptr + sizeof(file_size);
        // file.md5[]
        memcpy(ptr, &it->md5[0], sizeof(it->md5));
        ptr = ptr + sizeof(it->md5);
    }
    
    // send packet
    ::Write(sockfd, begin, size);

    return 0;
}

int FileListUpdateRequest::Read(int sockfd)
{
    // type
    ::Read(sockfd, &this->type, sizeof(this->type));

    // n_files
    ::Read(sockfd, &this->n_files, sizeof(this->n_files));

    // file_list
    for (int k = 0; k < this->n_files; k++)
    {
        // file
        File file;
        // file.file_length
        uint16_t filename_length;
        ::Read(sockfd, &filename_length, sizeof(filename_length));
        filename_length = ntohs(filename_length);
        file.filename_length = filename_length;
        // file.filename
        char filename[filename_length + 1];
        ::Read(sockfd, &filename[0], filename_length);
        filename[filename_length] = '\0';
        file.filename = filename;
        // file.file_size
        uint64_t file_size;
        ::Read(sockfd, &file_size, sizeof(file_size));
        file.file_size = be64toh(file_size);
        // file.md5[16]
        ::Read(sockfd, &file.md5[0], sizeof(file.md5));
        this->file_list.push_back(file);
    }

    return 0;
}