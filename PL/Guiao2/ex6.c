#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define L 10
#define C 1000000

int procuraMatriz (int **matrix, int key)
{
    pid_t pid;
    int created_processes = 0, status, f=0;
    int pids[L];

    for (int i=0 ; i<L ; i++)
    {
        if ((pid=fork())==0) // Child Proccess
        {
            for (int j=0 ; j<C ; j++)
                if (matrix[i][j] == key)
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
                printf("\t[pai] proccess %d exited. key found on row %d\n", child_pid, i);
            }
            else
            {
                printf("\t[pai] proccess %d exited. key not found\n", child_pid);
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
    printf("Executing with %d rows and %d columns\n\n", L, C);
    srand(time(NULL));
    int **m;    
    m = malloc(sizeof(int *) * L);
    for (int i=0 ; i<L ; i++)
    {
        m[i] = malloc(sizeof(int) * C);
        for (int j=0 ; j<C ; j++)
            m[i][j]=rand()%2000000;
    }

    int key;
    if (argc<2)
    {
        printf("Key not given as argument. Using Random key\n\n");
        key = rand()%2000000;
    }
    else
    {
        key = atoi(args[1]);
    }


    // for (int i=0 ; i<L ; i++) m[i][1] = key;

    int row = procuraMatriz(m,key);

    printf("\nSearching with key=%d returned: %d\n", key, row);

    return 0;
}