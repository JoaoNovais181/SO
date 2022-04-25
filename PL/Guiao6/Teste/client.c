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
    int writingEndFifo = open(FIFO, O_WRONLY);
    // if (argc>1)
    // {
    //     for (int i=1 ; i<argc ; i++)
    //         write(writingEndFifo, args[i], strlen(args[i]));
    // }

    int i=1;
    char *buf = args[i++];
    while(buf!=NULL)
    {
        write(writingEndFifo, buf, strlen(buf));
        write(writingEndFifo, " ", 1);
        buf = args[i++];
    }
    write(writingEndFifo, "\n", 1);

    
    close(writingEndFifo);

    return 0;
}