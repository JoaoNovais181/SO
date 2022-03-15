#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define L 10
#define C 1000000

int procuraMatriz (int **matrix, int needle)
{
    pid_t pid;
    int status, f=0;
    int pids[L];

    for (int i=0 ; i<L ; i++)
    {
        if ((pid=fork())==0) // Child Proccess
        {
            for (int j=0 ; j<C ; j++)
                if (matrix[i][j] == needle)
                    _exit(j);
            _exit(-1);
        }
        else
        {
            pids[i] = pid;
        }
    }

    for (int i=0 ; i<L ; i++)
    {
        pid_t child_pid = waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) < 255)  // status é em 4 bits sem sinal por isso -1 é representado como 255
            {
                f=i;
                printf("\t[pai] proccess %d exited. found number on row %d\n", child_pid, i);
            }
            else
            {
                printf("\t[pai] proccess %d exited. number not found\n", child_pid);
            }
        }
        else
        {
            printf("\t[pai] proccess %d exited. something went wrong\n", child_pid);
        }
    }

    return f;
}

int main (int argc, char *args[])
{
    if (argc<2)
    {
        printf("Usage: program <needle>\n");
        exit(-1);
    }

    printf("Executing with %d rows and %d columns\n\n", L, C);
    srand(time(NULL));
    int rand_max = 2000000;
    int **m;    
    m = malloc(sizeof(int *) * L);
    printf("Generating numbers from 0 to %d...", rand_max);
    for (int i=0 ; i<L ; i++)
    {
        m[i] = malloc(sizeof(int) * C);
        for (int j=0 ; j<C ; j++)
            m[i][j]=rand()%rand_max;
    }
    printf("Done\n");

    int needle = atoi(args[1]);

    int row = procuraMatriz(m,needle);
    printf("\nSearching with needle=%d returned: %d\n", needle, row);

    for (int i=0 ; i<L ; i++) free(m[i]);
    free(m);

    return 0;
}