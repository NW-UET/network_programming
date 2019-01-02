#include "message.h"

using namespace std;

int ListFilesResponse::Write(int sockfd)
{
    // size
    size_t size = sizeof(this->type) + sizeof(this->n_files);
    vector<Filesize> filesize_list = this->filesize_list;
    for (vector<Filesize>::iterator it = filesize_list.begin(); it != filesize_list.end(); ++it)
    {
        size += sizeof(it->filename_length);
        size += it->filename.size();
        size += sizeof(it->file_size);
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
    // filesize_list
    for (vector<Filesize>::iterator it = filesize_list.begin(); it != filesize_list.end(); ++it)
    {
        // filesize.filename_length
        uint16_t filename_length = htons(it->filename_length);
        memcpy(ptr, &filename_length, sizeof(filename_length));
        ptr = ptr + sizeof(filename_length);
        // filesize.filename
        memcpy(ptr, it->filename.data(), it->filename.size());
        ptr = ptr + it->filename.size();
        // filesize.file_size
        uint64_t file_size = htobe64(it->file_size);
        memcpy(ptr, &file_size, sizeof(file_size));
        ptr = ptr + sizeof(file_size);
    }

    // send packet
    ::Write(sockfd, begin, size);

    return 0;
}

int ListFilesResponse::ReadPayload(int sockfd)
{
    // n_files
    ::Read(sockfd, &this->n_files, sizeof(this->n_files));
    // filesize_list
    for (int k = 0; k < this->n_files; k++)
    {
        // filesize
        Filesize filesize;
        // filesize.filename_length
        uint16_t filename_length;
        ::Read(sockfd, &filename_length, sizeof(filename_length));
        filename_length = ntohs(filename_length);
        filesize.filename_length = filename_length;
        // filesize.filename
        char filenameStr[filename_length + 1];
        ::Read(sockfd, &filenameStr[0], filename_length);
        filenameStr[filename_length] = '\0';
        filesize.filename = filenameStr;
        // filesize.file_size
        uint64_t file_size;
        ::Read(sockfd, &file_size, sizeof(file_size));
        filesize.file_size = be64toh(file_size);
        this->filesize_list.push_back(filesize);
    }

    return 0;
}

int ListFilesResponse::Read(int sockfd)
{
    uint8_t type = ::ReadHeader(sockfd);
	if (this->type == type)
		this->ReadPayload(sockfd);

	return type;
}

void ListFilesResponse::print()
{
    cout << "type = " << typeString(this->type) << endl;
    printf("n_files = %d\n", this->n_files);
    vector<Filesize> filesize_list = this->filesize_list;
    for (vector<Filesize>::iterator it = filesize_list.begin(); it != filesize_list.end(); ++it)
    {
        printf("filename_length = %d\n", it->filename_length);
        string filename = it->filename;
        printf("filename = ");
        cout << filename << endl;
        printf("file_size = %ld\n", it->file_size);
    }
    printf("----\n");
}