#include "message.h"

using namespace std;

int ListHostsResponse::Write(int sockfd)
{
    // size
    vector<uint32_t> IP_addr_list = this->IP_addr_list;
    size_t size = sizeof(this->type) + sizeof(this->file_size) + sizeof(this->n_hosts) +
                  sizeof(uint32_t) * IP_addr_list.size();
    // allocate memory
    char *begin = (char *)malloc(size);
    bzero((void *)begin, size);
    char *ptr = begin;
    // type
    memcpy(ptr, &this->type, sizeof(this->type));
    ptr = ptr + sizeof(this->type);
    // file_size
    uint64_t file_size = htobe64(this->file_size);
    memcpy(ptr, &file_size, sizeof(file_size));
    ptr = ptr + sizeof(file_size);
    // n_hosts
    memcpy(ptr, &this->n_hosts, sizeof(this->n_hosts));
    ptr = ptr + sizeof(this->n_hosts);
    // IP_addr_list
    for (vector<uint32_t>::iterator it = IP_addr_list.begin(); it != IP_addr_list.end(); ++it)
    {
        // IP_addr
        uint32_t IP_addr = htonl(*it);
        memcpy(ptr, &IP_addr, sizeof(IP_addr));
        ptr = ptr + sizeof(IP_addr);
    }

    // send packet
    ::Write(sockfd, begin, size);

    return 0;
}

int ListHostsResponse::ReadPayload(int sockfd)
{
    // file_size
    uint64_t file_size;
    ::Read(sockfd, &file_size, sizeof(file_size));
    file_size = be64toh(file_size);
    this->file_size = file_size;
    // n_hosts
    ::Read(sockfd, &this->n_hosts, sizeof(this->n_hosts));
    // IP_addr_list
    for (int k = 0; k < this->n_hosts; k++)
    {
        // IP_addr
        uint32_t IP_addr;
        ::Read(sockfd, &IP_addr, sizeof(IP_addr));
        this->IP_addr_list.push_back(ntohl(IP_addr));
    }

    return 0;
}

int ListHostsResponse::Read(int sockfd)
{
	// read header
    uint8_t type = ::ReadHeader(sockfd);
    if (this->type == type)
        this->ReadPayload(sockfd);

    return type;
}

void ListHostsResponse::print()
{
    cout << "type = " << typeString(this->type) << endl;
    cout << "file_size = " << this->file_size << endl;
    printf("n_hosts = %d\n", this->n_hosts);
    vector<uint32_t> IP_addr_list = this->IP_addr_list;
    for (vector<uint32_t>::iterator it = IP_addr_list.begin(); it != IP_addr_list.end(); ++it)
    {
        struct in_addr in_addr;
        in_addr.s_addr = (in_addr_t)(*it);
        printf("ip_addr = %s\n", inet_ntoa(in_addr));
    }
    printf("----\n");
}