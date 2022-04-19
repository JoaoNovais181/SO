#include "MySystem.h"

int redirStdin (const char *in)
{
    int fdin = open(in, O_RDONLY);
    if (dup2(fdin, 0)==-1) { fprintf(stderr, "Erro %s\n", strerror(errno)); exit(errno);}
    close(fdin);
    return 0;
}

int redirStdout (const char *out, int out_append)
{
    int fdout;
    if (out_append) fdout = open(out, O_CREAT | O_WRONLY | O_APPEND, 0666);
    else fdout = open(out, O_CREAT | O_TRUNC | O_WRONLY , 0666);
    if (dup2(fdout, 1)==-1)  { fprintf(stderr, "Erro %s\n", strerror(errno)); exit(errno);}
    close(fdout);
    return 0;
}

int redirStderr (const char *err, int err_append)
{
    int fderr;
    if (err_append) fderr = open(err, O_CREAT | O_WRONLY | O_APPEND, 0666);
    else fderr = open(err, O_CREAT | O_TRUNC | O_WRONLY , 0666);
    if (dup2(fderr, 2)==-1) { fprintf(stderr, "Erro %s\n", strerror(errno)); exit(errno);}
    close(fderr);
    return 0;
}

int mysystem(char **exec_args, int argc)
{
    char **clean_args = malloc((argc+1)*sizeof(char*));

    char *in = NULL, *out=NULL, *err=NULL;
    int out_append=0, err_append=0; 

    int cleanIndex=0;
    for (int i=0 ; i<argc ; i++)
    {
        if (!strcmp(exec_args[i], "<"))
            in=exec_args[++i];
        else if (strchr(exec_args[i], '>'))
        {
            int l = strlen(exec_args[i]);
            if (l==1) { out = exec_args[++i]; out_append=0; }
            else if (l==2) 
            {
                if (exec_args[i+1][0]=='2') { err = exec_args[++i]; err_append=0; }
                else { out = exec_args[++i]; out_append=1; }
            }
            else { err = exec_args[++i]; err_append=1; } 
        }
        else clean_args[cleanIndex++] = exec_args[i];
    }
    clean_args[cleanIndex] = NULL;

    if (in)
        redirStdin(in);
    if (out)
        redirStdout(out, out_append);
    if (err)
        redirStderr(err, err_append);

    int res;
    pid_t pid=fork();
    switch (pid)
    {
        case -1:
            res = -1;
            break;
        case 0:
            int exec_ret = execvp(clean_args[0], clean_args);
            // for (int i=0 ; i<cleanIndex ; free(clean_args[i++]));
            // for (int i=0 ; i<*argc ; free(exec_args[0][i++]));
            // free(exec_args[0]);
            // free(clean_args);
            // free(exec_args);
            // free(argc);
            // free(tokens);
            _exit(exec_ret);
            // _exit(errno);
            break;
        default:
            int status;
            wait(&status);

            if (WIFEXITED(status) && !WEXITSTATUS(status))
                res = status;
            else 
                res = -1;
            break;
    }

    // free(exec_args[0]);
    free(clean_args);
    free(exec_args);
    // free(argc);
    // free(tokens);
    return res;
}