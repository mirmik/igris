#include "gstuff2.h"
#include "igris_crc.h"

int gstuffing(const char* data, int size, char* outdata)
{
	char* outstrt;
	uint8_t crc;

	crc = 0xFF;
	outstrt = outdata;

	*outdata++ = GSTUFF_START;

	while (size--)
	{
		char c = *data++;
		strmcrc8(&crc, c);

		switch (c)
		{
			case GSTUFF_START:
				*outdata++ = GSTUFF_STUB;
				*outdata++ = GSTUFF_STUB_START;
				break;

			case GSTUFF_STOP:
				*outdata++ = GSTUFF_STUB;
				*outdata++ = GSTUFF_STUB_STOP;
				break;

			case GSTUFF_STUB:
				*outdata++ = GSTUFF_STUB;
				*outdata++ = GSTUFF_STUB_STUB;
				break;
			default:
				*outdata++ = c;
		}
	}

	switch (crc)
	{
		case GSTUFF_START:
			*outdata++ = GSTUFF_STUB;
			*outdata++ = GSTUFF_STUB_START;
			break;

		case GSTUFF_STOP:
			*outdata++ = GSTUFF_STUB;
			*outdata++ = GSTUFF_STUB_STOP;
			break;

		case GSTUFF_STUB:
			*outdata++ = GSTUFF_STUB;
			*outdata++ = GSTUFF_STUB_STUB;
			break;
		default:
			*outdata++ = crc;
	}

	*outdata++ = GSTUFF_STOP;

	return outdata - outstrt;
}

void gstuff_autorecv_reset(struct gstuff_autorecv * autom)
{
	autom->crc = 0xff;
	sline_reset(&autom->line);
}

void gstuff_autorecv_setbuf (
    struct gstuff_autorecv * autom,
    void * buf, int len
)
{
	sline_init(&autom->line, buf, len);
	gstuff_autorecv_reset(autom);
}

int gstuff_autorecv_newchar(struct gstuff_autorecv * autom, char c)
{
	int sts;

	switch (autom->state)
	{
		case 0:
			gstuff_autorecv_reset(autom);

			//goto state 1 imediatly;
			autom->state = 4;
		//fallthrow
		
		case 4:
			// wait start
			switch (c)
			{
				case GSTUFF_START:
					//Приняли стартовый символ. Реинициализация.
					autom->state = 1;
					gstuff_autorecv_reset(autom);
					goto __continue__;

				default:
					goto __garbage__;					
			}

		case 1:
			switch (c)
			{
				case GSTUFF_START:
					//Приняли стартовый символ. Реинициализация.
					gstuff_autorecv_reset(autom);
					goto __force_restart__;
				
				case GSTUFF_STOP:
					// приняли финальный символ
					goto __stop_handler__;

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
			switch (c)
			{
				case GSTUFF_STUB_START: c = GSTUFF_START; break;
				case GSTUFF_STUB_STOP: c = GSTUFF_STOP; break;
				case GSTUFF_STUB_STUB:  c = GSTUFF_STUB;  break;
				case GSTUFF_START:
					gstuff_autorecv_reset(autom);
					goto __force_restart__;
				default:
					// Невалидный пакет.
					sts = GSTUFF_STUFFING_ERROR;
					goto __finish__;
			}

			goto __putchar__;
	}

	// unreacheble_point
	return GSTUFF_ALGORITHM_ERROR;

__stop_handler__:
	if (autom->crc != 0)
	{
		//Принят символ окончания пакета, но crc не пройден.
		sts = GSTUFF_CRC_ERROR;
		goto __finish__;
	}

	else
	{
		//Корректный приём пакета. Удаляем crc символ
		sline_backspace(&autom->line, 1);
		sts = GSTUFF_NEWPACKAGE;
		goto __finish__;
	}


__putchar__:
	if (!sline_putchar(&autom->line, c))
	{
		sts = GSTUFF_OVERFLOW;
		autom->state = 0;
		goto __finish__;
	}

	strmcrc8(&autom->crc, c);
	autom->state = 1;

	//fallthrow
__continue__:
	return GSTUFF_CONTINUE;

__force_restart__:
	autom->state = 1;
	return GSTUFF_FORCE_RESTART;

__garbage__:
	return GSTUFF_GARBAGE;

__finish__:
	autom->state = 0;
	return sts;
}