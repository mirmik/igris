#ifndef GENOS_DATASTRUCT_ARGV_H
#define GENOS_DATASTRUCT_ARGV_H

#ifdef __cplusplus
#include <vector>
#include <stdlib.h>
#include <string.h>

class argvc_t {
	std::vector<const char*> vect;

public:
	argvc_t(){}

	//void internal_split(argvc_t* args, char* str, char dv = " ");
	void internal_split(char* str, char dv = ' ') {
		newarg_search:
		while(*str == dv) ++str;
		if (*str == 0) goto end;

		vect.push_back(str);
		while(*str != dv && *str != 0) ++str;
		if (*str == dv) { *str++ = 0; goto newarg_search; };

		end:
		return;
	};

	int argc() { return vect.size(); }
	const char** argv() { return &vect[0]; }
};

struct str_argvc_t : public argvc_t {
	char* str;
	size_t sz;

	str_argvc_t(char* str) : str(str), sz(strlen(str)) {
		internal_split(str);
	}
};
#endif

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
