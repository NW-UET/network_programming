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
    // type
    packet.push_back(this->type);
    // n_files
    packet.push_back(this->n_files);
    // file_list
    vector<File> file_list = this->file_list;
    for (vector<File>::iterator it = file_list.begin(); it != file_list.end(); ++it)
    {
        // file.filename_length
        uint16_t filename_length = it->filename_length;
        packet.push_back(filename_length >> 8);
        packet.push_back(filename_length & 0xff);
        // file.filename
        for (vector<char>::iterator j = it->filename.begin(); j != it->filename.end(); ++j)
            packet.push_back(*j);
        // file.file_size
        uint64_t file_size = it->file_size;
        packet.push_back(file_size >> 24);
        packet.push_back((file_size >> 16) & 0xff);
        packet.push_back((file_size >> 8) & 0xff);
        packet.push_back(file_size & 0xff);
        // file.md5[]
        for (int j = 0; j < 16; j++)
            packet.push_back(it->md5[j]);
    }
    // print
    cout << "packet = ";
    for (vector<uint8_t>::iterator it = packet.begin(); it != packet.end(); ++it)
        printf("%d", *it);
    cout << endl;
    fflush(stdout);
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

int FileListUpdateRequest::Read(int sockfd)
{
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

    // n_files
    if (read(sockfd, &data8, sizeof(data8)) < 0)
    {
        perror("read");
        exit(1);
    }
    this->n_files = data8;
    size -= sizeof(data8);

    // file_list
    while (size > 0)
    {
        // file
        File file;
        // file.file_length
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
        file.filename_length = filename_length;
        size -= 2 * sizeof(data8);
        // file.filename
        while (filename_length > 0)
        {
            if (read(sockfd, &data8, sizeof(data8)) < 0)
            {
                perror("read");
                exit(1);
            }
            file.filename.push_back(data8);
            filename_length--;
            size -= sizeof(data8);
        }
        // file.file_size
        uint64_t file_size;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        file_size = data8;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        file_size = file_size << 8;
        file_size |= data8;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        file_size = file_size << 8;
        file_size |= data8;
        if (read(sockfd, &data8, sizeof(data8)) < 0)
        {
            perror("read");
            exit(1);
        }
        file_size = file_size << 8;
        file_size |= data8;
        file.file_size = file_size;
        size -= 4 * sizeof(data8);
        // file.md5[16]
        for (int j = 0; j < 16; j++)
        {
            if (read(sockfd, &file.md5[j], sizeof(data8)) < 0)
            {
                perror("read");
                exit(1);
            }
            size -= sizeof(file.md5[j]);
        }
        this->file_list.push_back(file);
    }

    return 0;
}