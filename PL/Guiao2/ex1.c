#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main ()
{
    printf("PID: %d\n", getpid());
    printf("PID pai: %d\n", getppid());

    return 0;
}