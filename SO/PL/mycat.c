#include <unistd.h>
#include <fcntl.h>

#define MAX_BUF 1024

int main ()
{
    char buff[MAX_BUF];

    int bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buff, MAX_BUF)) >0)
        write(STDOUT_FILENO, buff, bytes_read);

    return 0;
}