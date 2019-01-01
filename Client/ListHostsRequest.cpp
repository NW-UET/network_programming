#include "message.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int ListHostsRequest::Write(int sockfd){
	vector<uint8_t> packet;
	// type
    packet.push_back(this->type);
	// filename_length
	packet.push_back(this->filename->filename_length);
	// filename download
	packet.push_back(this->filename->filename);
	// send size of packet
    size_t size = packet.size() * sizeof(uint8_t);
    if (write(sockfd, &size, sizeof(size)) < 0)
    {
        perror("write");
        exit(1);
    }
    // send packet
    if (write(sockfd, packet.data(), size) < 0)
    {
        perror("write");
        exit(1);
    }
    return 0;
}

int ListHostsRequest::Read(int sockfd){
	// receive size of packet
    size_t size;
    if (read(sockfd, &size, sizeof(size)) < 0)
    {
        perror("read");
        exit(1);
    }

    // read packet
    uint8_t data8;

    // type
    if (read(sockfd, &data8, sizeof(data8)) < 0)
    {
        perror("read");
        exit(1);
    }
    this->type = data8;
    size -= sizeof(data8);

    if (size > 0)
    {
        // filename_length
        uint16_t filename_length;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        filename_length = data8;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        filename_length = filename_length << 8;
        filename_length |= data8;
        size -= 2 * sizeof(data8);
        // filename
        vector<char> filename;
        while (filename_length > 0)
        {
            if (read(sockfd, &data8, sizeof(data8)) < 0)
            {
                perror("read");
                exit(1);
            }
            filename.push_back(data8);
            filename_length--;
            size -= sizeof(data8);
        }
    }
    printf("Client request file : %s\n", filename);
}