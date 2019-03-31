#ifndef GXX_GSTUFF_CAUTOMATE_H
#define GXX_GSTUFF_CAUTOMATE_H

#include <sys/cdefs.h>

#include <igris/protocols/gstuff/gstuff.h>

#include <igris/datastruct/sline.h>
#include <igris/util/crc.h>

#define GSTUFF_CRC_ERROR -1
#define GSTUFF_OVERFLOW -2
#define GSTUFF_DATA_ERROR -3
#define GSTUFF_NEWPACKAGE 1
#define GSTUFF_CONTINUE 0

struct gstuff_autorecv {
	struct sline line;
	uint8_t crc;
	uint8_t state;
	//void(*callback)(void*,int,char*,int);
	//void* callback_argument;
};

__BEGIN_DECLS

void gstuff_autorecv_reset(struct gstuff_autorecv * autom);

void gstuff_autorecv_setbuf(struct gstuff_autorecv * autom, 
	void * buf, int len);

void gstuff_autorecv_init(
	struct gstuff_autorecv * autom, 
	void(*callback)(void*, int sts, char* dat, int len), 
	void * arg
);

int gstuff_autorecv_newchar(struct gstuff_autorecv * autom, char c);

/*static inline void
__gstuff_automate_reset(struct gstuff_automate * autom) {
	autom->crc = 0xff;
	sline_reset(&autom->line);
}

static inline void
gstuff_automate_reset(struct gstuff_automate * autom) {
	__gstuff_automate_reset(autom);
	autom->state = 0;
}

static inline void
gstuff_automate_init(struct gstuff_automate * autom, void * buf, int len, 
	void(*callback)(void*,char*,int), void * arg
) {
	sline_init(&autom->line, buf, len);
	gstuff_automate_reset(autom);
	autom->callback = callback;
	autom->callback_argument = arg;
}

static inline void 
gstuff_automate_newchar(struct gstuff_automate * autom, char c) {
	switch (autom->state) {
		case 0:
			if (c == gxx::gmsg::strt) {
				//Похоже на символ начала пакета.
				__gstuff_automate_reset(autom);
				autom->state = 1;
			}
			break;*/
/*		case 1:
			switch (c) 
			{
				case GSTUFF_START:
					//Приняли стартовый символ.
					if (autom->line.len == 0) //< Повторный стартовый. Ничего не делаем.
						break;
					
					if (autom->crc != 0) 
					{
						//Принят символ окончания пакета, но crc не пройден. 
						//dprln("packager::crc_error:", crc);
					}
					else {
						//gxx::println("callback");
						//invoke_callback();
						autom->callback(autom->callback_argument, 
							autom->line.buf, autom->line.len);
					}
					__gstuff_automate_reset(autom);
					break;
				case GSTUFF_STUB:
					autom->state = 2;
					break;
				default:
					sline_putchar(&autom->line, c);
					strmcrc8(&autom->crc, c);
			}
			break;

		case 2:
			switch (c) {
				case GSTUFF_STUB_START:
					sline_putchar(&autom->line, GSTUFF_START);
					strmcrc8(&autom->crc, GSTUFF_START);
					autom->state = 1;
					break;
				case GSTUFF_STUB_STUB:
					sline_putchar(&autom->line, GSTUFF_STUB);
					strmcrc8(&autom->crc, GSTUFF_STUB);
					autom->state = 1;
					break;
				default:
					//Неконсистентная последовательность.
					__gstuff_automate_reset(autom);
					autom->state = 0;
			}				
			break;

	}
}*/


__END_DECLS

#endif