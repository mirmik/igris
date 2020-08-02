#ifndef GSTUFF2_H
#define GSTUFF2_H

#include <stdint.h>
#include <igris/compiler.h>
#include <igris/datastruct/sline.h>

#define GSTUFF_START ((char)0xA8)
#define GSTUFF_STOP ((char)0xB2)
#define GSTUFF_STUB ((char)0xC5)
#define GSTUFF_STUB_START ((char)0x8A)
#define GSTUFF_STUB_STOP ((char)0x2B)
#define GSTUFF_STUB_STUB ((char)0x5C)

#define GSTUFF_CRC_ERROR -1
#define GSTUFF_OVERFLOW -2
#define GSTUFF_STUFFING_ERROR -3
#define GSTUFF_ALGORITHM_ERROR -4

#define GSTUFF_CONTINUE 0
#define GSTUFF_NEWPACKAGE 1
#define GSTUFF_FORCE_RESTART 2
#define GSTUFF_GARBAGE 3

struct gstuff_autorecv {
	struct sline line;
	uint8_t crc;
	uint8_t state;
};

__BEGIN_DECLS

/**
	Собрать пакет gstuff.

	@param data - входной буффер
	@param size - длина входного буффер
	@param outdata - выходной буффер (рекомендованная длина 2*size+2)
	@return результирующая длина пакета.
 */
int gstuffing(const char* data, int size, char* outdata);


void gstuff_autorecv_reset(struct gstuff_autorecv * autom);

void gstuff_autorecv_setbuf(struct gstuff_autorecv * autom, 
	void * buf, int len);

/*void gstuff_autorecv_init(
	struct gstuff_autorecv * autom, 
	void(*callback)(void*, int sts, char* dat, int len), 
	void * arg
);*/

int gstuff_autorecv_newchar(struct gstuff_autorecv * autom, char c);

__END_DECLS

#endif