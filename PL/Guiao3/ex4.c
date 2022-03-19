#include <unistd.h>
#include <errno.h>
#include <stdio.h>
// #include <stdlib.h>
#include <sys/wait.h>

int system(const char *command)
{
    if (command == NULL)
    {
        return 2;
    }

    pid_t pid;

    if ((pid = fork()) == 0)
    {
        if (execl("/bin/sh", "sh", "-c", command, (char *) NULL))
        {
            _exit(127);
        }
        _exit(0);
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

int main (int argc, char *args[])
{
    int aux;
    if ((aux=system("echo \"It Works!!\"")))
    {
        perror("Something went wrong");
        return errno;
    }

    return 0;
}