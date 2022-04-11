#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_SIZE 1024

int mysystem(const char *command)
{
    if (command == NULL)
    {
        return 2;
    }

    // Assumindo um maximo de 20 argumentos
    int i=0, max=20;
    char **exec_args = malloc(max * sizeof(char *));
    char *string, *dup = strdup(command);

    string=strsep(&dup, " ");

    while (string!=NULL)
    {
        // printf("string: %s\n", string);
        if (i>=max)
        {
            max += max*0.5;
            exec_args = realloc(exec_args, max * sizeof(char *));
        }
        exec_args[i++] = string;
        string = strsep(&dup, " ");
    }

    exec_args[i] = NULL;

    pid_t pid;

    int res;
    if ((pid = fork()) == 0)
    {
        int exec_ret = execvp(exec_args[0], exec_args);
        _exit(exec_ret);
    }
    
    if (pid == -1)
    {
        res = -1;
    }

    else
    {
        int status;
        wait(&status);

        if (WIFEXITED(status))
        {
            res = WEXITSTATUS(status);
        }
        else 
        {
            res = -1;
        }
    }
    free(exec_args);
    free(dup);
    return res;
}

int main (int argc, char *args[])
{
    int ifd=open("/etc/passwd", O_RDONLY);
    int ofd=open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    int efd=open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);

    dup2(ifd, 0);
    dup2(ofd, 1);
    dup2(efd, 2);

    close(ifd);
    close(ofd);
    close(efd);

    execlp("wc", "wc", NULL);
    
    return 0;
}