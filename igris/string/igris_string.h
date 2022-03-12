#ifndef IGRIS_STRING_IGRIS_STRING_H
#define IGRIS_STRING_IGRIS_STRING_H 

#include <igris/compiler.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

__BEGIN_DECLS
	char * igris_strdup(const char *s);
	size_t igris_strlen(const char *str);
__END_DECLS

#endif