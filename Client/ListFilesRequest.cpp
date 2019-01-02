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

int ListFilesRequest::ReadPayload(int sockfd)
{
    return 0;
}

int ListFilesRequest::Read(int sockfd)
{
    uint8_t type = ::ReadHeader(sockfd);
	if (this->type == type)
		this->ReadPayload(sockfd);

	return type;
}

void ListFilesRequest::print()
{
    cout << "type = " << typeString(this->type) << endl;
    printf("----\n");
}