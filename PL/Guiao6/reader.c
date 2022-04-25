#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO "fifoex1"
#define BUF_SIZE 1024

int main ()
{
    char buffer[BUF_SIZE];
    int readingEndFifo = open(FIFO, O_RDONLY), r=0;

    while ((r = read(readingEndFifo, buffer, BUF_SIZE))>0)
    {
        if (!strncmp(buffer, "exit", 4)) break;
        write(1, buffer, r);
    }

    close(readingEndFifo);

    return 0;
}