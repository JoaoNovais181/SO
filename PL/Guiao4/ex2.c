#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int main ()
{
    int i=0;
    char buffer[BUF_SIZE];

    int ifd=open("/etc/passwd", O_RDONLY);
    int ofd=open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    int efd=open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    int fdin=dup(0);
    int fdout=dup(1);
    int fderr=dup(2);

    dup2(ifd, 0);
    dup2(ofd, 1);
    dup2(efd, 2);

    close(ifd);
    close(ofd);
    close(efd);

    pid_t pid;

    if (!(pid=fork()))
    {
        while ((i=read(0,buffer,BUF_SIZE))!=0)
        {
            write(1, buffer, BUF_SIZE);
            write(2, buffer, BUF_SIZE);
        }

        _exit(0);
    }

    wait(NULL);

    close(0);
    close(1);
    close(2);
    dup2(fdin,0);
    dup2(fdout,1);
    dup2(fderr,2);

    printf("Terminei\n");
    // write(1, "Terminei\n", 9);


    return 0;
}