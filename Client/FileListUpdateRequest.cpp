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

int FileListUpdateRequest::Write(int sockfd)
{
    vector<uint8_t> packet;

    packet.push_back(this->type);

    packet.push_back(this->n_files);

    vector<Filename> file_list = this->file_list;
    for (vector<Filename>::iterator it = file_list.begin(); it != file_list.end(); ++it)
    {
        uint16_t value = it->filename_length;
        packet.push_back(value >> 8);
        packet.push_back(value & 0xff);

        for (vector<char>::iterator j = it->filename.begin(); j != it->filename.end(); ++j)
            packet.push_back(*j);
    }
    cout << "packet = ";
    for (vector<uint8_t>::iterator it = packet.begin(); it != packet.end(); ++it)
        printf("%d",*it);
    cout << endl;
    fflush(stdout);
    size_t size = packet.size()*sizeof(uint8_t);
    if (write(sockfd, &size, sizeof(size)) < 0)
    {
        perror("write");
        exit(1);
    }

    if (write(sockfd, packet.data(), size) < 0)
    {
        perror("write");
        exit(1);
    }

    return 0;
}

int FileListUpdateRequest::Read(int sockfd)
{
    size_t size;
    if (read(sockfd, &size, sizeof(size)) < 0)
    {
        perror("read");
        exit(1);
    }

    uint8_t data8;
    if (read(sockfd, &data8, sizeof(data8)) < 0)
    {
        perror("read");
        exit(1);
    }
    this->type = data8;
    size -= sizeof(data8);

    if (read(sockfd, &data8, sizeof(data8)) < 0)
    {
        perror("read");
        exit(1);
    }
    this->n_files = data8;
    size -= sizeof(data8);

    while (size > 0)
    {
        Filename filename;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        uint16_t filename_length = data8;
        filename_length = filename_length << 8;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        filename_length |= data8;
        filename.filename_length = filename_length;
        size -= 2*sizeof(data8);
        while (filename_length > 0)
        {
            if (read(sockfd, &data8, sizeof(data8)) < 0)
            {
                perror("read");
                exit(1);
            }
            filename.filename.push_back(data8);
            filename_length--;
            size -= sizeof(data8);
        }
        this->file_list.push_back(filename);
    }

    return 0;
}