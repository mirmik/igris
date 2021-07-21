#include <errno.h>
#include <igris/shell/rshell.h>
#include <string.h>

#define SSHELL_ARGCMAX 10

int rshell_execute_v(int argc, char **argv,
                     const struct rshell_command *cmdtable, int *retptr,
                     int dropargs, char *output, int maxsize)
{
    int res;
    const struct rshell_command *it = cmdtable;
    while (it->func != NULL)
    {
        if (!strcmp(argv[0], it->name))
        {
            res = it->func(argc - dropargs, argv + dropargs, output, maxsize);
            if (retptr)
                *retptr = res;
            return SSHELL_OK;
        }
        ++it;
    }

    return ENOENT;
}

int rshell_execute(char *str, const struct rshell_command *cmdtable,
                   int *retptr, int dropargs, char *output, int maxsize)
{
    char *argv[SSHELL_ARGCMAX];
    int argc;

    if (*str == '\0')
    {
        return ENOENT;
    }

    argc = argvc_internal_split(str, argv, SSHELL_ARGCMAX);

    return rshell_execute_v(argc, argv, cmdtable, retptr, dropargs, output,
                            maxsize);
}

int rshell_tables_execute(char *str, const struct rshell_command *const *tables,
                          int *retptr, int dropargs, char *output, int maxsize)
{
    char *argv[SSHELL_ARGCMAX];
    int argc;
    int res;

    if (*str == '\0')
    {
        return ENOENT;
    }

    argc = argvc_internal_split(str, argv, SSHELL_ARGCMAX);

    const struct rshell_command *const *tit = tables;
    while (*tit != NULL)
    {
        const struct rshell_command *it = *tit;
        while (it->func != NULL)
        {
            if (!strcmp(argv[0], it->name))
            {
                res =
                    it->func(argc - dropargs, argv + dropargs, output, maxsize);
                if (retptr)
                    *retptr = res;
                return SSHELL_OK;
            }
            ++it;
        }
        ++tit;
    }

    return ENOENT;
}

void rshell_help(const struct rshell_command *cmdtable,
                 void (*write)(void *, const char *, unsigned int),
                 void *privdata)
{
    const struct rshell_command *it = cmdtable;
    while (it->func != NULL)
    {
        write(privdata, it->name, strlen(it->name));
        if (it->help)
        {
            write(privdata, " - ", 3);
            write(privdata, it->help, strlen(it->help));
        }
        write(privdata, "\r\n", 2);
        ++it;
    }
}

void rshell_tables_help(const struct rshell_command *const *cmdtables,
                        void (*write)(void *, const char *, unsigned int),
                        void *privdata)
{
    const struct rshell_command *const *tit = cmdtables;
    while (*tit != NULL)
    {
        rshell_help(*tit, write, privdata);
        ++tit;
    }
}
