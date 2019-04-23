#ifndef GENOS_DATASTRUCT_ARGV_H
#define GENOS_DATASTRUCT_ARGV_H

static inline int argvc_length_of_first(const char* str)
{
	const char* strt = str;

	while (*str != ' ' && *str != '\0') 
	{
		++str;
	}

	return str - strt;
}

static inline int argvc_internal_split(char* data, char** argv, int argcmax) {
	int argc = 0;

	newarg_search:
	while(*data == ' ') ++data;
	if (*data == '\0' || argc >= argcmax) return argc;

	argv[argc++] = data;
	while(*data != ' ' && *data != '\0') ++data;
	if (*data == ' ') { 
		*data++ = '\0'; 
		goto newarg_search; 
	};

	return argc;
}

#endif
