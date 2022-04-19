#include "Execute.h"

/**
 * @brief Tokenizes string by using ' ' as a delimiter
 * 
 * @param command String to tokenize
 * @param N Poiter to integer that will be written with the number of tokens in the string (if not NULL)
 * @return List of tokens in string
 */
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

/**
 * @brief Executes the first command on piped command succession
 * 
 * @param filedes Pipe
 * @param command Command to execute
 * @param argc Number of arguments on command
 */
void filho0 (int *filedes, char **command, int argc)
{
    close(filedes[0]);

    dup2(filedes[1], 1);
    close(filedes[1]);

    mysystem(command, argc);
}

/**
 * @brief Executes the last command on piped command succession
 * 
 * @param filedes Pipe
 * @param command Command to execute
 * @param argc Number of arguments on command
 */
void filhoN (int *filedes, char **command, int argc)
{
    close(filedes[1]);

    dup2(filedes[0], 0);
    close(filedes[0]);

    mysystem(command, argc);
}

/**
 * @brief Executes the command on the middle of the piped command succession
 * 
 * @param filedesE Entry pipe
 * @param filedesS Out pipe
 * @param command Command to execute
 * @param argc Number of arguments on command
 */
void filhoi (int *filedesE, int *filedesS, char **command, int argc)
{
    close(filedesE[1]);
    close(filedesS[0]);

    dup2(filedesE[0], 0);
    dup2(filedesS[1], 1);

    close(filedesE[0]);
    close(filedesS[1]);

    mysystem(command, argc);
}

/**
 * @brief Executes the commands given as argument with pipes connecting their input/output
 * 
 * @param commands List of commands to execute
 * @param argCs  Array of integer containing the number of arguments of every command
 * @param N  Number of commands
 * @return 0 if everything went well
 */
int executePipes (char ***commands, int *argCs, int N)
{
    int **filedes = malloc((N-1)*sizeof(int*));

    for (int i=0 ; i<(N-1) ; i++)
    {
        filedes[i] = malloc(2*sizeof(int));
        if (pipe(filedes[i])==-1)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(errno);
        }
    }

    pid_t pid;

    for (int i=0 ; i<N ; i++)
    {
        pid = fork();

        if (pid==-1)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(errno);
        }

        if (!pid)
        {
            if (i==0) 
                filho0(filedes[i], commands[i], argCs[i]);
            else if (i==N-1)
                filhoN(filedes[i-1], commands[i], argCs[i]);
            else
                filhoi(filedes[i-1], filedes[i], commands[i], argCs[i]);
            _exit(errno);
        }
        
        free(commands[i]);

        if (!i)
            close(filedes[i][1]);    // como fica o extremo de escrita do filho aberto, é necessario fechar o extremo de escrita do pai
        else if (i==N-1)
            close(filedes[i-1][0]);  // como fica o extremo de leitura do filho aberto, é necessario fechar o extremo de leitura do pai
        else
        {
            close(filedes[i-1][0]);  // como fica o extremo de leitura do filho aberto, é necessario fechar o extremo de leitura do pai
            close(filedes[i][1]);    // como fica o extremo de escrita do filho aberto, é necessario fechar o extremo de escrita do pai
        }
    }

    for (int i=0 ; i<N ; i++)
    {
        int status;
        wait(&status);
        if (i<N-1) free(filedes[i]);
    }
    free(filedes);
    return 0;
}

/**
 * @brief Function to parse the command into arguments and verify if the execution is pipeles or with pipes
 * 
 * @param command line read from the bash that is parsed
 * @return 0 if everything is OK
 */
int execute (const char *command)
{
    int MaxCommands=3, nCommands=0, nTokens=0, MaxArgs=20, nArg=0;
    char ***commands = malloc(MaxCommands*sizeof(char**));  
    int *argCs = malloc(MaxCommands*sizeof(int*));
    bzero(commands, MaxCommands*sizeof(char**));
    bzero(argCs, MaxCommands*sizeof(int*));
    char *dup = strdup(command);
    char **tokens = tokenize(dup, &nTokens);

    for (int i=0 ; i<nTokens ; i++)
    {
        if (!commands[nCommands])
            commands[nCommands] = malloc(MaxArgs*sizeof(char*));
        
        if (!strcmp(tokens[i], "|"))
        {
            if (nCommands+2 > MaxCommands)
            {
                MaxCommands *= 2;
                commands = realloc(commands, MaxCommands*sizeof(char **));
                argCs = realloc(argCs, MaxCommands*sizeof(int*));
                bzero(commands+(nCommands*sizeof(char**)), (MaxCommands/2)*sizeof(char**));
                bzero(argCs+(nCommands*sizeof(int*)), (MaxCommands/2)*sizeof(int*));
            }
            if (nArg+1 > MaxArgs)
                commands[nCommands] = realloc(commands[nCommands], (MaxArgs+1)*sizeof(char*));
            argCs[nCommands] = nArg;
            commands[nCommands++][nArg] = NULL;
            nArg=0;
            MaxArgs=20;
            continue;
        }
        if (nArg+1 > MaxArgs)
        {
            MaxArgs *= 1.5;
            commands[nCommands] = realloc(commands[nCommands], MaxArgs*sizeof(char*));
        }
        commands[nCommands][nArg++] = tokens[i];
    }

    argCs[nCommands] = nArg;
    commands[nCommands++][nArg] = NULL;

    int res=0;

    if (nCommands>1)
        res = executePipes(commands, argCs, nCommands);
    else
        res = mysystem(commands[0], argCs[0]);

    free(dup);
    free(tokens);
    free(commands);
    free(argCs);

    return res;
}