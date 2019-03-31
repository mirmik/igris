#include "autorecv.h"

void gstuff_autorecv_reset(struct gstuff_autorecv * autom) 
{
	autom->crc = 0xff;
	sline_reset(&autom->line);
}

void gstuff_autorecv_setbuf (
		struct gstuff_autorecv * autom, 
		void * buf, int len
) {
	sline_init(&autom->line, buf, len);
	gstuff_autorecv_reset(autom);
}

int gstuff_autorecv_newchar(struct gstuff_autorecv * autom, char c) 
{
	int sts;

	switch (autom->state) {
		case 0:
			gstuff_autorecv_reset(autom);

			//goto state 1 imediatly;
			autom->state = 1;

		//fallthrow
		case 1:
			switch (c) 
			{
				case GSTUFF_START:
					//Приняли стартовый символ.
					if (sline_empty(&autom->line)) //< Повторный стартовый. Ничего не делаем.
						goto __continue__;
					
					if (autom->crc != 0) 
					{
						//Принят символ окончания пакета, но crc не пройден. 
						sts = GSTUFF_CRC_ERROR;
						goto __finish__;
					}

					else {
						//Корректный приём пакета.
						sts = GSTUFF_NEWPACKAGE;
						goto __finish__;
					}
				
				case GSTUFF_STUB:
					//Принят STUFF ждем вторй байт.
					autom->state = 2;
					goto __continue__;
				
				default:
					goto __putchar__;
			}

		//unreachable point

		case 2:
			// На прошлой итерации принят STUFF. 
			// Обрабатываем второй символ.
			switch (c) {
				case GSTUFF_STUB_START: c = GSTUFF_START; break;
				case GSTUFF_STUB_STUB:  c = GSTUFF_STUB;  break;
				default:
					// Невалидный пакет.
					sts = GSTUFF_DATA_ERROR;
					goto __finish__;
			}		
			
			goto __putchar__;
	}

	__putchar__:
		if (!sline_putchar(&autom->line, c)) {
			sts = GSTUFF_OVERFLOW;
			goto __finish__;
		}
		strmcrc8(&autom->crc, c);
		autom->state = 1;
	
	//fallthrow		
	__continue__:
		return GSTUFF_CONTINUE;

	__finish__:
		autom->state = 0;
		return sts;
}