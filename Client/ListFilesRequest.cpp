#include "message.h"

using namespace std;

int ListFilesRequest::Write(int sockfd)
{
    size_t size = sizeof(this->type);
    // allocate memory
    char *begin = (char *)malloc(size);
    bzero((void *)begin, size);
    char *ptr = begin;
    // type
    memcpy(ptr, &this->type, sizeof(this->type));
    ptr = ptr + sizeof(this->type);
    // send packet
    ::Write(sockfd, begin, size);

    return 0;
}

int ListFilesRequest::Read(int sockfd)
{
    ::Read(sockfd, &this->type, sizeof(this->type));
    return 0;
}

void ListFilesRequest::print()
{
    printf("type = %d\n", this->type);
    printf("----\n");
}