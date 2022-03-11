#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main ()
{
    pid_t pid;
    int filho_num, status;

    for (filho_num=1 ; filho_num<=10 ; filho_num++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("Filho%d : pid %d\n", filho_num, getpid());
            printf("Filho%d : pid pai %d\n", filho_num, getppid());
        
            _exit(filho_num);
        }
        else
        {
            pid_t pid_filho = wait(&status);

            if (WIFEXITED(status))
            {
                printf("Pai: recebi o valor %d para o pid %d\n", WEXITSTATUS(status), pid_filho);
            }
            else
            {
                printf("erro\n");
            }
        }
    }

    return 0;
}