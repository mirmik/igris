#ifndef IGRIS_SIMPLE_SHELL_H
#define IGRIS_SIMPLE_SHELL_H

#warning "SSHELL is deprecated"

#include <igris/datastruct/argvc.h>
#include <igris/compiler.h>

#define SSHELL_OK 0

//При отсутствии ноды возвращает ENOENT

namespace igris {
	namespace sshell {
		struct command {
			const char* name;
			int (*func) (int, char**);
		};

		int execute_unsafe(char* str, const struct command* cmd, int cmdlen, int* retptr);
		int execute(const char* str, const struct command* cmd, int cmdlen, int* retptr);
	}
}

#endif
