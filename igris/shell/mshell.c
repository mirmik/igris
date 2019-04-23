#include <igris/shell/sshell.h>
#include <errno.h>
#include <string.h>

#define SSHELL_ARGCMAX 10

//TODO: заменить SSHELL_ARGCMAX на подсчет аргументов. Использовать динамический масив.
int mshell_execute_unsafe(
	const struct mshell_command* cmd, 
	char* str, 
	int len,
	int* retptr
) {
	char* argv[SSHELL_ARGCMAX];
	int argc;
	int res;

	if (*str == '\0') {
		return ENOENT;
	}

	argc = argvc_internal_split(str, argv, SSHELL_ARGCMAX);

	for(int i = 0; i < cmdlen; ++i) {
		if (!strcmp(argv[0], cmd[i].name)) {
			res = cmd[i].func(argc, argv);
			if (retptr) *retptr = res;
			return SSHELL_OK;
		}
	}

	return ENOENT;
}

/*int mshell_execute(
	const struct mshell_command* cmd, 
	const char* str, 
	int len, 
	int* retptr
) {
	char locstr[strlen(str) + 1];
	strcpy(locstr, str);
	return mshell_execute_unsafe(locstr, cmd, cmdlen, retptr);
}*/