#include "readln.h"

#define MAX_BUFFER 4096

int main (int argc, char **argv)
{
    int line_counter = 0;   

    char buffer[MAX_BUFFER];

    int bytes_read = 0;

    int newline = 1;

    while ( (bytes_read = readln(0, buffer, MAX_BUFFER))>0 )
    {
        char line_number[10] = "";

        // Escrever o indice da linha
        if (newline && buffer[0]!='\n')
        {
            snprintf(line_number, 10, "%d:", line_counter);
            write(1, line_number, sizeof(line_number));
            line_counter++;
        }
        write(1, buffer, bytes_read);

        // Nao encontrou o \n no fim da linha por isso continua a ler
        if (buffer[bytes_read - 1] != '\n')
            newline = 0;
        else 
            newline = 1;
    }

    return 0;
}