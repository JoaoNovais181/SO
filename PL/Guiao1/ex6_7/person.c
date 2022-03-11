#include "person.h"

int new_person (char *name, int age)
{
    Person p;
    p.age = age;
    if (!strncpy(p.name, name, MAX_NAME_SIZE))
        return -1;

    int fd = open (FILENAME, O_CREAT | O_APPEND | O_WRONLY, 666);
    if (fd < 0)
    {
        printf("Msg: %s, id: %d\n", strerror(errno), errno);
        perror("Erro a abrir ficheiro de pessoas");
        return -1;
    }

    write(fd, &p, sizeof(Person));

    int res;

    res = lseek(fd, -sizeof(Person), SEEK_CUR);

    if (res<0)
    {
        perror("Error lseek");
        return -1;
    }

    close(fd);

    printf("Registo %d\n", res/sizeof(Person));

    return res/sizeof(Person);
}

int person_change_age (char *name, int age)
{
    int fd = open (FILENAME, O_RDWR, 640);
    if (fd < 0)
    {
        printf("Msg: %s, id: %d\n", strerror(errno), errno);
        perror("Erro a abrir ficheiro de pessoas");
        return -1;
    }

    int read_bytes = 0;
    Person p2;

    while ( (read_bytes = read(fd, &p2, sizeof(Person))) > 0)
        if (!strcmp(name, p2.name))
        {
            p2.age = age;
            lseek(fd, -sizeof(Person), SEEK_CUR);
            write(fd, &p2, sizeof(Person));
        }

    close(fd);

    return 0;
}

int person_change_age2 (long offset, int age)
{
    int fd = open (FILENAME, O_RDWR, 640);
    if (fd < 0)
    {
        printf("Msg: %s, id: %d\n", strerror(errno), errno);
        perror("Erro a abrir ficheiro de pessoas");
        return -1;
    }

    int read_bytes = 0;
    Person p;

    lseek(fd, offset*sizeof(Person), SEEK_SET);
    if ( (read_bytes = read(fd, &p, sizeof(Person))) > 0)
    {
        p.age = age;
        lseek(fd, -sizeof(Person), SEEK_CUR);
        write(fd, &p, sizeof(Person));
    }

    close(fd);
    return 0;
}

// Funcao que fiz para experimentar so
int dump_People ()
{
    int fd = open(FILENAME, O_RDONLY, 640);
    if (fd <0)
    {
        printf("Msg: %s, id: %d\n", strerror(errno), errno);
        perror("Erro a abrir ficheiro de pessoas");
        return -1;
    }

    int read_bytes = 0;
    Person p;
    int i=0;

    while ( (read_bytes = read(fd, &p, sizeof(Person))) > 0)
    {
        char buf[MAX_NAME_SIZE + 12];
        int w = snprintf(buf, MAX_NAME_SIZE + 23, "%d: %s %d\n",i++,  p.name, p.age);
        write(1, buf, w);
    }

    close(fd);
    return 0;
}

int person_file_clean ()
{
    int fd = open(FILENAME, O_RDONLY | O_WRONLY | O_TRUNC , 640);
    if (fd<0)
    {
        printf("Msg: %s, id: %d\n", strerror(errno), errno);
        perror("Erro a abrir ficheiro de pessoas");
        return -1;
    }
    close(fd);

    write (1, "Limpeza das pessoas feita\n", 27);
    return 0;
}