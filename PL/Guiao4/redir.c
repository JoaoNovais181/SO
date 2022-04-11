#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h> 
#include <stdlib.h>


int mysystem(const char *command)
{
    if (command == NULL)
        return 2;

    int i=0, max=20;
    char **exec_args = malloc(max * sizeof(char *));
    char *string, *dup = strdup(command);

    while ((string = strsep(&dup, " "))!=NULL)
    {
        if (strlen(string)<1) continue;
         
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
        res = -1;
    else
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
            res = WEXITSTATUS(status);
        else 
            res = -1;
    }
    free(exec_args); free(dup);
    return res;
}

int main (int argc, char *args[])
{
    int fdin, fdout, idx=1;
    int ol = dup(1);

    if (!strcmp("-i",args[idx]))
    {
        fdin = open(args[idx+1], O_RDONLY);
        dup2(fdin,0);
        idx+=2;
    }

    if (!strcmp("-o",args[idx]))
    {
        fdout = open(args[idx+1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
        dup2(fdout,1);
        idx+=2;
    }

    int n=1024;
    char *buff = malloc(n*sizeof(char));
    bzero(buff,1024);

    while (idx<argc)
    {
        strcat(buff, args[idx++]);
        strcat(buff, " ");
    }

    // write(ol, buff, strlen(buff));

    mysystem(buff);

    free(buff);

    return 0;
}