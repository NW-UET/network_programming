#include "message.h"

using namespace std;

int DownloadFileRequest::Write(int sockfd)
{
	// size
	size_t size = sizeof(this->type) + sizeof(this->filename.filename_length) + this->filename.filename.size();
    // allocate memory
	char *begin = (char *)malloc(size);
	bzero((void *)begin, size);
	char *ptr = begin;
    // type
	memcpy(ptr, &this->type, sizeof(this->type));
	ptr = ptr + sizeof(this->type);
    // filename.filename_length
	uint16_t filename_length = htons(this->filename.filename_length);
	memcpy(ptr, &filename_length, sizeof(filename_length));
	ptr = ptr + sizeof(this->filename.filename_length);
    // filename.filename
	memcpy(ptr, this->filename.filename.data(), this->filename.filename.size());
	ptr = ptr + this->filename.filename.size();
	// offset
	uint32_t offset = htonl(this->offset);
	memcpy(ptr, &offset,sizeof(offset));
	ptr = ptr + sizeof(this->offset);
    // send packet
	::Write(sockfd, begin, size);

	return 0;
}

int DownloadFileRequest::Read(int sockfd)
{
	// type
	::Read(sockfd, &this->type, sizeof(this->type));
    // filename.filename_length
	uint16_t filename_length;
	::Read(sockfd, &filename_length, sizeof(filename_length));
	filename_length = ntohs(filename_length);
	this->filename.filename_length = filename_length;
    // filename.filename
	char filename[filename_length + 1];
	::Read(sockfd, &filename[0], filename_length);
	filename[filename_length] = '\0';
	this->filename.filename = filename;
    // offset
	uint32_t offset;
	::Read(sockfd, &offset, sizeof(offset));
	offset = ntohl(offset);
	this->offset = offset;

	return 0;
}