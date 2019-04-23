#ifndef IGRIS_SIMPLE_SHELL_H
#define IGRIS_SIMPLE_SHELL_H

#include <igris/datastruct/argvc.h>
#include <sys/cdefs.h>

#define SSHELL_OK 0

//При отсутствии ноды возвращает ENOENT

struct mshell_command
{
	const char* name;
	int (*func) (int, char**);
};

int mshell_execute_unsafe(char* str,
                          const struct mshell_command* cmd,
                          int cmdlen,
                          int* retptr);

int mshell_execute(const char* str,
                   const struct mshell_command* cmd,
                   int cmdlen, int*
                   retptr);

#endif
