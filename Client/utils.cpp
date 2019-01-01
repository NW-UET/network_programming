#include "message.h"

void Write(int sockfd, const void *buff, size_t n)
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
        }
        if (bytesWritenThisTime == 0)
        {
            printf("socket was closed\n");
            break;
        }
        if (bytesWritenThisTime > 0)
        {
            bytesWriten += bytesWritenThisTime;
        }
    }
}

void Read(int sockfd, void *buff, size_t n)
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
        }
        if (bytesReadThisTime == 0)
        {
            printf("socket was closed\n");
            break;
        }
        if (bytesReadThisTime > 0)
        {
            bytesRead += bytesReadThisTime;
        }
    }
}

uint8_t ReadHeader(int sockfd)
{
    // type
    uint8_t type;
    ::Read(sockfd, &type, sizeof(type));

    return type;
}