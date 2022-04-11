#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

/*  MINHA VERSAO

int mysystem(const char *command)
{
    if (command == NULL)
    {
        return 2;
    }

    pid_t pid;

    if ((pid = fork()) == 0)
    {
        int exec_ret = execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        
        _exit(exec_ret);
    }
    else if (pid == -1)
    {
        return -1;
    }
    else
    {
        int status;
        wait(&status);

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 127)
            {
                return 127;
            }
            else
            {
                return WEXITSTATUS(status);
            }
        }
        else 
        {
            return errno;
        }
    }
}
*/
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

    // string=strsep(&dup, " ");

    while ((string = strsep(&dup, " "))!=NULL)
    {
        if (strlen(string)<1) continue;
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
    else if (pid == -1)
    {
        res = -1;
    }
    else
    {
        int status;
        wait(&status);

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status))
            {
                res = status;
            }
            else
            {
                res = -1;
            }
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
    int aux;
    mysystem("wc ");

    return 0;
}