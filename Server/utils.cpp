#include "message.h"

ssize_t Write(int sockfd, const void *buff, size_t n)
{
    size_t bytesWriten = 0;
    size_t bytesToWrite = n;
    while (bytesWriten != bytesToWrite)
    {
        ssize_t bytesWritenThisTime = write(sockfd, (char *)buff + bytesWriten, (bytesToWrite - bytesWriten));
        if (bytesWritenThisTime < 0)
        {
            perror("write");
            exit(1);
            // return bytesWritenThisTime;
        }
        if (bytesWritenThisTime == 0)
        {
            printf("Writing socket was closed\n");
            return 0;
        }
        if (bytesWritenThisTime > 0)
        {
            bytesWriten += bytesWritenThisTime;
        }
    }
    return bytesWriten;
}

ssize_t Read(int sockfd, void *buff, size_t n)
{
    size_t bytesRead = 0;
    size_t bytesToRead = n;
    while (bytesRead != bytesToRead)
    {
        ssize_t bytesReadThisTime = read(sockfd, (char *)buff + bytesRead, (bytesToRead - bytesRead));
        if (bytesReadThisTime < 0)
        {
            perror("read");
            exit(1);
            // return bytesReadThisTime;
        }
        if (bytesReadThisTime == 0)
        {
            printf("Reading socket was closed\n");
            return 0;
        }
        if (bytesReadThisTime > 0)
        {
            bytesRead += bytesReadThisTime;
        }
    }
    return bytesRead;
}

uint8_t ReadHeader(int sockfd)
{
    // type
    uint8_t type;
    ::Read(sockfd, &type, sizeof(type));

    return type;
}

string typeString(uint8_t type)
{
    if (type == FILE_LIST_UPDATE_REQUEST)
        return "FILE_LIST_UPDATE_REQUEST";
    else if (type == FILE_LIST_UPDATE_RESPONSE)
        return "FILE_LIST_UPDATE_RESPONSE";
    else if (type == LIST_FILES_REQUEST)
        return "LIST_FILES_REQUEST";
    else if (type == LIST_FILES_RESPONSE)
        return "LIST_FILES_RESPONSE";
    else if (type == LIST_HOSTS_REQUEST)
        return "LIST_HOSTS_REQUEST";
    else if (type == LIST_HOSTS_RESPONSE)
        return "LIST_HOSTS_RESPONSE";
    else if (type == DOWNLOAD_FILE_REQUEST)
        return "DOWNLOAD_FILE_REQUEST";
    else if (type == DOWNLOAD_FILE_RESPONSE)
        return "DOWNLOAD_FILE_RESPONSE";
    else return "";
}