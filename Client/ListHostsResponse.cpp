#include "message.h"

using namespace std;

int ListHostsResponse::Write(int sockfd)
{
    // size
    vector<uint32_t> IP_addr_list = this->IP_addr_list;
    size_t size = sizeof(this->type) + sizeof(this->n_hosts) + sizeof(uint32_t)*IP_addr_list.size();
    // allocate memory
    char *begin = (char *)malloc(size);
    bzero((void *)begin, size);
    char *ptr = begin;
    // type
    memcpy(ptr, &this->type, sizeof(this->type));
    ptr = ptr + sizeof(this->type);
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

int ListHostsResponse::Read(int sockfd)
{
    // type
    ::Read(sockfd, &this->type, sizeof(this->type));
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