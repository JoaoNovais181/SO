#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "readln.h"

#define FIFO "clientToServer"
#define LOG "log.txt"
#define BUF_SIZE 128

int fd;

int parseLine (const char *command, int size, int *exitFlag)
{
    if (!strncmp(command, "exit", 4)) 
    {
        (*exitFlag) = 1;
        return -1;
    }
    printf("%d\n", strncmp(command, "exit", 4));
    write(fd, command, size);
    // write(1, command, size);

    return 0;
}

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

    if ((fd = open(LOG, O_WRONLY | O_TRUNC | O_CREAT, 0640))==-1)
    {
        perror("Error opening log.txt");
        return 2;
    }

    int readingEndFifo = open(FIFO, O_RDWR), r=0, exitFlag = 0;
    while (!exitFlag && (r = readln(readingEndFifo, buffer, BUF_SIZE))>0)
    {
        // int x = parseLine(buffer);
        // if (x==-1)
            // break;
        // if (!strncmp(buffer, "exit", 4)) break;
        // write(fd, buffer, r);
        parseLine(buffer, r, &exitFlag);
        // write(1, buffer, r);
    }

    close(readingEndFifo);
    close(fd);

    return 0;
}