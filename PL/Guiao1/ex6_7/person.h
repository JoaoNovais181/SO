#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "file_pessoas"

#define MAX_NAME_SIZE 200

typedef struct Person
{
    char name[MAX_NAME_SIZE];
    int age;
} Person;


int new_person (char  *name, int age);
int person_change_age (char *name, int age);
int person_change_age2 (long offset, int age);
int dump_People ();
int person_file_clean ();
