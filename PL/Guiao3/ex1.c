#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    execlp("ls", "ls", NULL);
    perror("Algo correu mal");
    
    return 0;
}