#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO "fifoex1"
#define BUF_SIZE 1024

int main ()
{
    char buffer[BUF_SIZE];
    int writingEndFifo = open(FIFO, O_WRONLY), r=0;

    while ((r = read(0, buffer, BUF_SIZE))>0)
        write(writingEndFifo, buffer, r);

    close(writingEndFifo);

    return 0;
}