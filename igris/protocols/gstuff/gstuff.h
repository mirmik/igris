#ifndef GSTUFF_BASE_H
#define GSTUFF_BASE_H

#include <sys/cdefs.h>

#define GSTUFF_START ((char)0xAC)
#define GSTUFF_STUB ((char)0xAD)
#define GSTUFF_STUB_START ((char)0xAE)
#define GSTUFF_STUB_STUB ((char)0xAF)

__BEGIN_DECLS

/**
	Собрать пакет gstuff.

	@param data - входной буффер
	@param size - длина входного буффер
	@param outdata - выходной буффер (рекомендованная длина 2*size+2)
	@return результирующая длина пакета.
 */
int gstuffing(char* data, int size, char* outdata);

__END_DECLS

#endif