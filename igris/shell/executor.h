#ifndef IGRIS_SHELL_EXECUTOR_H
#define IGRIS_SHELL_EXECUTOR_H

#include <igris/shell/conscmd.h>
#include <igris/datastruct/argvc.h>

#include <unistd.h>

#define EXECUTOR_PROCESS_STARTED 2
#define EXECUTOR_ERROR -1

#define EXECUTOR_TBLFIN nullptr

namespace igris
{

	class executor
	{
	public:
		bool debug_mode = false;
		virtual int execute(
		    char* str, int len, int flags, int* p_ret) = 0;
	};

	class syscmd_executor : public executor
	{
		console_command ** tbl;

	public:
		syscmd_executor(igris::console_command** syscmdtbl)
			: tbl(syscmdtbl) {}

		int execute(char * str, int len, int flags, int * retptr) override
		{
			int flen = 0;
			int argc;
			int res;
			char * argv[10];
			igris::console_command ** it0;
			igris::console_command * it1;

			if (debug_mode)
			{
				dpr("execinput: len: "); dpr(len); dpr("data: ");
				debug_write(str, len); dprln();
				debug_print_dump(str, len);
			}

			if (!(flags & SH_INTERNAL_SPLIT))
			{
				BUG();
			}

			if (len <= 0)
			{
				return 0;
			}

			// Скипаем ведущие пробелы
			while (*str == ' ' || *str == '\n')
			{
				++str;
				--len;
			}

			// Ищем длину первого слова
			while (flen != len && str[flen] != '\0' && str[flen] != ' ' && str[flen] != '\n')
				++flen;

			if (flen <= 0)
			{
				return 0;
			}

			// Встроенная функция help
			if ((str[4] == 0 || str[4] == ' ' || str[4] == '\n') && !strncmp(str, "help", 4))
			{
				for (it0 = tbl; *it0 != nullptr; ++it0)
				{
					for (it1 = *it0; it1->func != NULL; ++it1)
					{
						if (it1->help)
						{
							printf("%s - %s\r\n", it1->name, it1->help);
						}
						else
						{
							printf("%s\r\n", it1->name);
						}
					}
				}
				return 0;
			}

			// Основной цикл
			for (it0 = tbl; *it0 != nullptr; ++it0)
			{
				for (it1 = *it0; it1->func != NULL; ++it1)
				{
					if (strlen(it1->name) == flen && !strncmp(str, it1->name, flen))
					{
						switch (it1->type)
						{
							case CMDFUNC:
								argc = argvc_internal_split(str, argv, 10);
								res = ((syscmd_func_t)(it1->func))(argc, argv);

								if (retptr) *retptr = res;

								return 0;

							case CMDAUTOM:
								BUG();

							case CMDCOOP:
								BUG();
								argc = 0;
								//return mshell_make_process(it1->func, argc, argv);
						}
					}
				}
			}

			printf("Not enough command\r\n");
			return ENOENT;
		}
	};

	class syscmd_executor_onelevel : public executor
	{
		console_command * tbl;

	public:
		syscmd_executor_onelevel(igris::console_command* syscmdtbl)
			: tbl(syscmdtbl) {}

		int execute(char * str, int len, int flags, int * retptr) override
		{
			int flen = 0;
			int argc;
			int res;
			char * argv[10];
			igris::console_command * it;

			if (!(flags & SH_INTERNAL_SPLIT))
			{
				BUG();
			}

			if (len <= 0)
			{
				return 0;
			}

			// Скипаем ведущие пробелы
			while (*str == ' ')
				++str;

			// Ищем длину первого слова
			while (flen != len && str[flen] != '\0' && str[flen] != ' ')
				++flen;

			// Встроенная функция help
			if (flen == 4 && !strncmp(str, "help", 4))
			{
				for (it = tbl; it->name != NULL; ++it)
				{
					if (it->help)
					{
						printf("%s - %s\r\n", it->name, it->help);
					}
					else
					{
						printf("%s\r\n", it->name);
					}
				}
				return 0;
			}

			// Основной цикл
			for (it = tbl; it->name != nullptr; ++it)
			{
				if (!strncmp(str, it->name, flen))
				{
					switch (it->type)
					{
						case CMDFUNC:
							argc = argvc_internal_split(str, argv, 10);
							res = ((syscmd_func_t)(it->func))(argc, argv);

							if (retptr) *retptr = res;

							return 0;

						case CMDAUTOM:
							BUG();

						case CMDCOOP:
							BUG();
							argc = 0;
							//return mshell_make_process(it1->func, argc, argv);
					}
				}
			}


			return ENOENT;
		}
	};
}

#endif