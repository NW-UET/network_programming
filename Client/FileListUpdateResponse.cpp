#include "message.h"

using namespace std;

int FileListUpdateResponse::Write(int sockfd)
{
    // size
    size_t size = sizeof(this->type) + sizeof(this->n_files);
    vector<Filestatus> filestatus_list = this->filestatus_list;
    for (vector<Filestatus>::iterator it = filestatus_list.begin(); it != filestatus_list.end(); ++it)
    {
        size += sizeof(it->filename_length);
        size += it->filename.size();
        size += sizeof(it->status);
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
    // filestatus_list
    for (vector<Filestatus>::iterator it = filestatus_list.begin(); it != filestatus_list.end(); ++it)
    {
        // filestatus.filename_length
        uint16_t filename_length = htons(it->filename_length);
        memcpy(ptr, &filename_length, sizeof(filename_length));
        ptr = ptr + sizeof(filename_length);
        // filestatus.filename
        memcpy(ptr, it->filename.data(), it->filename.size());
        ptr = ptr + it->filename.size();
        // filestatus.status
        memcpy(ptr, &it->status, sizeof(it->status));
        ptr = ptr + sizeof(it->status);
    }

    // send packet
    ::Write(sockfd, begin, size);

    return 0;
}

int FileListUpdateResponse::ReadPayload(int sockfd)
{
    // n_files
    ::Read(sockfd, &this->n_files, sizeof(this->n_files));

    // filestatus_list
    for (int k = 0; k < this->n_files; k++)
    {
        // filestatus
        Filestatus filestatus;
        // filestatus.file_length
        uint16_t filename_length;
        ::Read(sockfd, &filename_length, sizeof(filename_length));
        filename_length = ntohs(filename_length);
        filestatus.filename_length = filename_length;
        // filestatus.filename
        char filename[filename_length + 1];
        ::Read(sockfd, &filename[0], filename_length);
        filename[filename_length] = '\0';
        filestatus.filename = filename;
        // filestatus.status
        ::Read(sockfd, &filestatus.status, sizeof(filestatus.status));
        this->filestatus_list.push_back(filestatus);
    }

    return 0;
}

int FileListUpdateResponse::Read(int sockfd)
{
	// read header
    uint8_t type = ::ReadHeader(sockfd);
	if (this->type == type)
		this->ReadPayload(sockfd);

	return type;
}

void FileListUpdateResponse::print()
{
    cout << "type = " << typeString(this->type) << endl;
    printf("n_files = %d\n", this->n_files);
    vector<Filestatus> filestatus_list = this->filestatus_list;
    for (vector<Filestatus>::iterator it = filestatus_list.begin(); it != filestatus_list.end(); ++it)
    {
        printf("filename_length = %d\n", it->filename_length);
        cout << "filename = " << it->filename << endl;
        printf("status = %d", it->status);
        cout << endl;
    }
    printf("----\n");
}