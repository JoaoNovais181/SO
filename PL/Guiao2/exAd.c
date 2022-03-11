#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "file_matrix"

#define ROWS 10
#define COLS 10000

int searchMatrixFile (int key)
{
    pid_t pid;
    int status, f=0;
    int pids[ROWS];
    int aux[COLS];

    int fd = open(FILENAME, O_RDONLY, 640);
    if (fd<0)
    {
        printf("Error: %s. errno: %d\n", strerror(errno), errno);
        perror("Error opening file");
        return errno;
    }

    for (int i=0 ; i<ROWS ; i++)
    {
        if ((pid=fork())==0) // Child Proccess
        {
            int read_bytes = read(fd, aux, COLS * sizeof(int));

            if (read_bytes <= 0) perror("Error reading from file_matrix");
            
            for (int j=0 ; j<COLS ; j++)
                if (aux[j] == key)
                    _exit(j);
            _exit(-1);
        }
        else
        {
            pids[i] = pid;
        }
    }

    for (int i=0 ; i<ROWS ; i++)
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

    close(fd);

    return f;
}

int dumpMatrixFile ()
{
    int fd = open(FILENAME, O_RDONLY, 640);
    if (fd<0)
    {
        printf("Error: %s. errno: %d\n", strerror(errno), errno);
        perror("Error opening file");
        return errno;
    }

    int arr[COLS];
    int bytes_read;
    int j=0;
    while ((bytes_read=read(fd, arr, COLS*sizeof(int)))>0)
    {
        printf("%d: ", j++);
        for (int i=0 ; i<COLS ; i++) printf("%d ", arr[i]);
        printf("\n");
    }

    return 0;
}

int cleanMatrixFile ()
{
    int fd = open(FILENAME, O_WRONLY | O_CREAT, 640);
    if (fd<0)
    {
        printf("Error: %s. errno: %d", strerror(errno), errno);
        perror("Error opening file");
        return -1;
    }
    close(fd);
    return 0;
}

void fillMatrix (int **matrix, int randMax)
{
    for (int i=0 ; i<ROWS ; i++)
    {
        matrix[i] = malloc(sizeof(int) * COLS);
        for (int j=0 ; j<COLS ; j++)
        {
            matrix[i][j] = rand() % randMax;
        }
    }
}

int main (int argc, char *args[])
{

    int **matrix;
    int randMax = 1000000;

    matrix = malloc(sizeof(int *) * ROWS);
    srand(time(NULL));
    fillMatrix(matrix, randMax);

    int fd = open(FILENAME, O_WRONLY | O_CREAT, 640);
    if (fd<0)
    {
        printf("Error: %s. errno: %d", strerror(errno), errno);
        perror("Error opening file");
        exit(errno);
    }

    for (int i=0 ; i<ROWS ; i++)
    {
        int bytes_written = write(fd, matrix[i], COLS * sizeof(int));
        if (bytes_written <= 0)
        {
            perror("Error writing");
        } 
    }

    close(fd);

    
    int key;
    if (argc<2)
    {
        printf("Key not given as argument. Using Random key\n\n");
        key = rand()%randMax;
    }
    else
    {
        key = atoi(args[1]);
    }
    
    // dumpMatrixFile();

    printf("\nSearching with key=%d returned: %d\n", key, searchMatrixFile(key));


    for (int i=0 ; i<ROWS ; i++) free(matrix[i]);
    free(matrix);

    return 0;
}