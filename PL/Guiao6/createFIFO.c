#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#define FIFO "fifoex1"

int main ()
{

    if (mkfifo(FIFO, 0640)==-1)
    {
        if (errno!=EEXIST)
        {
            perror("Error creating fifo");
            return 2;
        }   
    }

    return 0;
}