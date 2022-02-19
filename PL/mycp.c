#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAX_BUF 1024*1024

int main (int agrc, char **args)
{
    
    printf("args[0]: %s\n", args[0]);
    printf("args[1]: %s\n", args[1]);
    printf("args[2]: %s\n", args[2]);

    char buffer[MAX_BUF];

    int fd_origem, fd_destino;

    if ((fd_origem = open(args[1], O_RDONLY))==-1)
    {
        printf("Msg: %s, id: %d\n", strerror(errno), errno);
        perror("Erro a abrir ficheiro de origem");
        return -1;
    }

    if ((fd_destino = open(args[2], O_WRONLY | O_CREAT | O_APPEND, 640)) == -1)
    {
        printf("Msg: %s, id: %d\n", strerror(errno), errno);
        perror("Erro a abrir ficheiro de destino");
        return -1;
    }

    int bytes_read;

    while ((bytes_read = read(fd_origem, buffer, MAX_BUF)) > 0)
    {
        write(fd_destino, buffer, bytes_read);
    }

    close(fd_origem);
    close(fd_destino);

    return 0;
}