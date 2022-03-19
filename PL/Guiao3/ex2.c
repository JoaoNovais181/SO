#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main ()
{
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        printf("[filho] pid %d\n", getpid());
        if (execl("/bin/ls", "ls", NULL))
        {
            perror("Algo correu mal");
            exit(errno);
        }
    }

    else 
    {
        int status;
        pid_t tp = wait(&status);
        if (WIFEXITED(status))
        {
            printf("[pai] %d terminado com exit status: %d\n", tp, WEXITSTATUS(status));
        }
    }

    printf("oalalaoaoaolal\n");

    return 0;
}