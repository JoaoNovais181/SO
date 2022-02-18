#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int readC (int fd, char *c)
{
    char buff[1];
    int bytes_read = read(fd, buff, 1);
    *c = buff[0];
    return bytes_read;
}

ssize_t readln(int fd, char *line, size_t size)
{
    int bytes = 0;

    while ( readC(fd, line+bytes) && bytes<size)
        if (line[bytes++] == '\n') break;

    return bytes;
}


int main ()
{
    char buff[2];

    int s = readln(0, buff, 2);
    puts(buff);

    return 0;
}