#ifndef IGRIS_STRING2_H
#define IGRIS_STRING2_H

#include <igris/compiler.h>
#include <stddef.h>

__BEGIN_DECLS

void * igris_memmem(const void *l, size_t l_len, const void *s, size_t s_len);

void replace_substrings(
	char * buffer, size_t maxsize, 
	const char * input, size_t inlen,
	const char * sub, size_t sublen,
	const char * rep, size_t replen
);

__END_DECLS


#ifdef __cplusplus

#include <string>

namespace igris 
{
	std::string replace(
		const std::string& src, 
		const std::string& oldsub, 
		const std::string& newsub);
}
#endif

#endif
