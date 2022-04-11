#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_SIZE 1024

int filho2pai ()
{
    int filedes[2];

    if (pipe(filedes)==-1)
    {
        perror("Error Creating PIPE\n");
        return errno;
    }

    static const char *line[] = {"LINHA1", "LINHA2", "LINHA3\n"};
    char buffer[1024];

    pid_t pid=fork();
    int res=0;

    switch (pid)
    {
        case -1:
            perror("Error creating child proccess");
            return errno;
        case 0:
            // fechar descritor de leitura
            close(filedes[0]);
            int i=0;

            while ((res=write(filedes[1], line[i], strlen(line[i])))>0)
            {
                printf("[FILHO] wrote %s to pipe\n", line[i++]);
            }

            write(1, "[FILHO] fora do while\n", 22);
            close(filedes[1]);
            _exit(0);

            break;        
        default:

            close(filedes[1]);

            while ((res=read(filedes[0], buffer, BUF_SIZE))>0)
            {
                printf("[PAI] leu %s do filho\n", buffer);
            }

            write(1, "[PAI] fora do while\n", 20);
            close(filedes[0]);
            wait(NULL);
            break;
    }

    return 0;
}

int pai2filho ()
{
    int filedes[2];

    if (pipe(filedes)==-1)
    {
        perror("Error Creating PIPE\n");
        return errno;
    }

    static const char *line[] = {"LINHA1;", "LINHA2;", "LINHA3;"};
    char buffer[BUF_SIZE];

    pid_t pid=fork();
    int res=0;

    switch (pid)
    {
        case -1:
            perror("Error creating child proccess");
            return errno;
        case 0:
            // fechar descritor de leitura
            close(filedes[1]);

            while ((res=read(filedes[0], buffer, BUF_SIZE))>0)
            {
                printf("[FILHO] leu %s do pai\n", buffer);
            }

            write(1, "[FILHO] fora do while\n", 22);

            // fechar o descritor de escrita
            close(filedes[0]);
            _exit(0);

            break;        
        default:
            close(filedes[0]);

            for (int i=0 ; i<3 ; i++)
            {
                write(filedes[1], line[i], strlen(line[i]));
                printf("[PAI] escreveu %s para o pipe\n", line[i]);
            }

            write(1, "[PAI] fora do while\n", 20);

            close(filedes[1]);
            wait(NULL);
            break;
    }

    return 0;
}

int main ()
{
    // filho2pai();
    pai2filho();
    return 0;
}