#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    if (execlp("ls", "ls", NULL))
    {
        perror("Algo correu mal");
        // exit(errno);
    }
    
    printf("ola\n\n\n");
    
    return 0;
}