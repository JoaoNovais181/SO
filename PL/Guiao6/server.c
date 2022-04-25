#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO "clientToServer"
#define LOG "log.txt"
#define BUF_SIZE 1024

int main ()
{
    char buffer[BUF_SIZE];

    if (mkfifo(FIFO, 0640)==-1)
    {
        if (errno!=EEXIST)
        {
            perror("Error creating fifo");
            return 2;
        }
    }

    int fd;
    if ((fd = open(LOG,  O_APPEND |  O_WRONLY | O_TRUNC | O_CREAT, 0640))==-1)
    {
        perror("Error opening log.txt");
        return 2;
    }

    int readingEndFifo = open(FIFO, O_RDWR), r=0;
    while ((r = read(readingEndFifo, buffer, BUF_SIZE))>0)
    {
        if (!strncmp(buffer, "exit", 4)) break;
        write(fd, buffer, r);
        write(fd, "\n", 1);
    }

    close(readingEndFifo);
    close(fd);

    return 0;
}