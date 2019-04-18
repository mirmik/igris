#include <igris/shell/sshell.h>
#include <errno.h>
#include <string.h>

#define SSHELL_ARGCMAX 10

//TODO: заменить SSHELL_ARGCMAX на подсчет аргументов. Использовать динамический масив.
int igris::sshell::execute_unsafe(
	char* str, 
	const struct igris::sshell::command* cmd, 
	int cmdlen, 
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

int igris::sshell::execute(
	const char* str, 
	const struct igris::sshell::command* cmd, 
	int cmdlen, 
	int* retptr
) {
	char locstr[strlen(str) + 1];
	strcpy(locstr, str);
	return igris::sshell::execute_unsafe(locstr, cmd, cmdlen, retptr);
}