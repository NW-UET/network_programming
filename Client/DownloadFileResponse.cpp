#include "message.h"

using namespace std;

int DownloadFileResponse::Write(int sockfd)
{
	// size
	size_t size = sizeof(this->type) + sizeof(this->status);
	// allocate memory
	char *begin = (char *)malloc(size);
	bzero((void *)begin, size);
	char *ptr = begin;
	// type
	memcpy(ptr, &this->type, sizeof(this->type));
	ptr = ptr + sizeof(this->type);
	// status
	memcpy(ptr, &this->status, sizeof(this->status));
    ptr = ptr + sizeof(this->status);
	// send packet
	::Write(sockfd, begin, size);

	return 0;
}

int DownloadFileResponse::ReadPayload(int sockfd)
{
	// status
	::Read(sockfd, &this->status, sizeof(this->status));
	
	return 0;
}

int DownloadFileResponse::Read(int sockfd)
{
	// read header
	uint8_t type = ::ReadHeader(sockfd);
	if (this->type == type)
		this->ReadPayload(sockfd);

	return type;
}

void DownloadFileResponse::print()
{
	cout << "type = " << typeString(this->type) << endl;
	printf("status = %d",this->status);
	printf("\n----\n");
}