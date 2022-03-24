#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_BUF 1024

int numDigits (int num)
{
    num = num<1 ?-1*num :num;
    if (num<10) return 1;
    if (num<100) return 2;
    if (num<1000) return 3;
    if (num<10000) return 4;
    if (num<100000) return 5;
    if (num<1000000) return 6;
    if (num<10000000) return 7;
    if (num<100000000) return 8;
    return 9;
}

int main (int argc, char *args[])
{
    int bytes_read, created_processes = 0, running=1;
    char buffer[MAX_BUF];
    pid_t pid;

    if (write(1, "t-bash opened\n", 14)==-1)
    {
        perror("Something went wrong!");
        exit(errno);
    }

    write(0, " $ ", 3);

    do
    {
        if ((bytes_read = read(0, buffer, MAX_BUF))<0)
        {
            perror("Someting went wrong!");
            exit(errno);
        }
        else if (bytes_read==0)
        {
            running = 0;
        }
        else if (bytes_read==5 && !strncmp("exit\n", buffer, 5))
        {
            running = 0;
        }
        else 
        {
            if (buffer[bytes_read-2]=='&')
            {
                buffer[bytes_read-2]=0;

                if ((pid=fork())==0)
                {
                    if ((pid=fork())==0)
                    {
                        // int n = 14+numDigits(getpid());
                        // char buf[n];
                        // snprintf(buf, n, "[%d] background\n", getpid());
                        printf("[%d] background\n", getpid());
                        // write(0, buf, n);
                        write(0, " $ ", 3);
                        if (execl("/bin/sh", "sh", "-c", buffer, NULL))
                        {
                            perror("Something went wrong!!");
                            _exit(errno);
                        }
                    }
                    else
                    {
                        pid_t tp = wait(NULL);
                        // int n = 14+numDigits(tp);
                        // char buf[n];
                        // snprintf(buf, n, "[%d] terminated\n", tp);
                        // printf("[%d] background\n", getpid());
                        // write(0, buf, n);
                        printf("\n[%d] terminated\n", tp);
                        write(0,  " $ ", 3);
                    }
                    _exit(0);
                }
                else
                {
                    created_processes++;
                }
            }
            else
            {
                buffer[bytes_read-1]=0;

                if ((pid=fork())==0)
                {
                    if (execl("/bin/sh", "sh", "-c", buffer, NULL))
                    {
                        perror("Something went wrong!!");
                        _exit(errno);
                    }
                    _exit(0);
                }
                else
                {
                    int status;
                    wait(&status);
                    write(0, " $ ", 3);
                }
            }
        }
    } while (running);

    for (int i=0 ; i<created_processes ; i++)
    {
        int status;
        wait(&status);
    }


    return 0;
}