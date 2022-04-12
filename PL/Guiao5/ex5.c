#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_MAX 1024

char **tokenize (char *command, int *N)
{
    if (!command) return NULL;
    int max = 20, i=0;
    char *string;
    char **exec_args = malloc(max * sizeof(char *));


    while ((string = strsep(&command, " "))!=NULL)
    {
        if (strlen(string)<1) continue;
        if (i>=max)
        {
            max += max*0.5;
            exec_args = realloc(exec_args, max * sizeof(char *));
        }
        exec_args[i++] = string;
    }

    exec_args[i] = NULL;
    if (N) *N = i;

    return exec_args;
}

void filho0 (int *filedes, char *command)
{
    close(filedes[0]);

    dup2(filedes[1], 1);
    close(filedes[1]);

    char **args = tokenize(command, NULL);
    execvp(args[0], args);
    free(args);
    _exit(0);
}

void filhoN (int *filedes, char *command)
{
    close(filedes[1]);

    dup2(filedes[0], 0);
    close(filedes[0]);

    char **args = tokenize(command, NULL);
    execvp(args[0], args);
    free(args);
    _exit(errno);
}

void filhoi (int *filedesE, int *filedesS, char *command)
{
    close(filedesE[1]);
    close(filedesS[0]);

    dup2(filedesE[0], 0);
    dup2(filedesS[1], 1);

    close(filedesE[0]);
    close(filedesS[1]);

    char **args = tokenize(command, NULL);
    execvp(args[0], args);
    free(args);
    _exit(errno);
}

int main ()
{   
    char commands[][128] = {"grep -v ^# /etc/passwd", "cut -f7 -d:", "uniq", "wc -l"};
    int n=4;
    int filedes[3][2];

    for (int i=0 ; i<n-1 ; i++)
        if (pipe(filedes[i])==-1)
        {
            strerror(errno);
            exit(errno);
        }

    pid_t pid;

    for (int i=0 ; i<n ; i++)
    {
        pid = fork();

        if (pid==-1)
        {
            strerror(errno);
            exit(errno);
        }

        if (!pid)
        {
            if (i==0) 
                filho0(filedes[i], commands[i]);
            else if (i==n-1)
                filhoN(filedes[i-1], commands[i]);
            else
                filhoi(filedes[i-1], filedes[i], commands[i]);
        }

        if (!i)
            close(filedes[i][1]);    // como fica o extremo de escrita do filho aberto, é necessario fechar o extremo de escrita do pai
        else if (i==n-1)
            close(filedes[i-1][0]);  // como fica o extremo de leitura do filho aberto, é necessario fechar o extremo de leitura do pai
        else
        {
            close(filedes[i-1][0]);  // como fica o extremo de leitura do filho aberto, é necessario fechar o extremo de leitura do pai
            close(filedes[i][1]);    // como fica o extremo de escrita do filho aberto, é necessario fechar o extremo de escrita do pai
        }
    }

    for (int i=0 ; i<n ; i++)
        wait(NULL);

    return 0;
}