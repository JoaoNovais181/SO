#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO "clientToServer"
#define BUF_SIZE 1024

int main (int argc, char *args[])
{
    if (argc>1)
    {
        int writingEndFifo = open(FIFO, O_WRONLY);

        for (int i=1 ; i<argc ; i++)
            write(writingEndFifo, args[i], strlen(args[i]));
        close(writingEndFifo);
    }


    return 0;
}