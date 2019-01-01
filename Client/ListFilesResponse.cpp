#include "message.h"

using namespace std;

int ListFilesResponse::Write(int sockfd)
{
    // size
    size_t size = sizeof(this->type) + sizeof(this->n_files);
    vector<Filename> filename_list = this->filename_list;
    for (vector<Filename>::iterator it = filename_list.begin(); it != filename_list.end(); ++it)
    {
        size += sizeof(it->filename_length);
        size += it->filename.size();
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
    // filename_list
    for (vector<Filename>::iterator it = filename_list.begin(); it != filename_list.end(); ++it)
    {
        // filename.filename_length
        uint16_t filename_length = htons(it->filename_length);
        memcpy(ptr, &filename_length, sizeof(filename_length));
        ptr = ptr + sizeof(filename_length);
        // filename.filename
        memcpy(ptr, it->filename.data(), it->filename.size());
        ptr = ptr + it->filename.size();
    }

    // send packet
    ::Write(sockfd, begin, size);

    return 0;
}

int ListFilesResponse::ReadPayload(int sockfd)
{
    // n_files
    ::Read(sockfd, &this->n_files, sizeof(this->n_files));
    // filename_list
    for (int k = 0; k < this->n_files; k++)
    {
        // filename
        Filename filename;
        // filename.filename_length
        uint16_t filename_length;
        ::Read(sockfd, &filename_length, sizeof(filename_length));
        filename_length = ntohs(filename_length);
        filename.filename_length = filename_length;
        // filename.filename
        char filenameStr[filename_length + 1];
        ::Read(sockfd, &filenameStr[0], filename_length);
        filenameStr[filename_length] = '\0';
        filename.filename = filenameStr;
        this->filename_list.push_back(filename);
    }

    return 0;
}

int ListFilesResponse::Read(int sockfd)
{
    if (this->type != ::ReadHeader(sockfd))
        return 0;
    this->ReadPayload(sockfd);

    return 0;
}

void ListFilesResponse::print()
{
    printf("type = %d\n", this->type);
    printf("n_files = %d\n", this->n_files);
    vector<Filename> filename_list = this->filename_list;
    for (vector<Filename>::iterator it = filename_list.begin(); it != filename_list.end(); ++it)
    {
        printf("filename_length = %d\n", it->filename_length);
        string filename = it->filename;
        printf("filename = ");
        cout << filename;
        printf("\n");
    }
    printf("----\n");
}