#include "message.h"

using namespace std;

int DownloadFileResponse::Write(int sockfd)
{
	// size
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

int DownloadFileResponse::ReadPayload(int sockfd)
{
	return 0;
}

int DownloadFileResponse::Read(int sockfd)
{
	if (this->type != ::ReadHeader(sockfd))
		return 0;
	this->ReadPayload(sockfd);

	return 0;
}

void DownloadFileResponse::print()
{
	printf("type = %d\n", this->type);
	printf("----\n");
}