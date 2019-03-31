#include <igris/protocols/gstuff/gstuff.h>
#include <igris/util/crc.h>

int gstuffing(char* data, int size, char* outdata) 
{
	char* outstrt;
	uint8_t crc;

	crc = 0xFF;
	outstrt = outdata;

	*outdata++ = GSTUFF_START;
	
	while(size--) 
	{
		char c = *data++;
		strmcrc8(&crc, c);

		switch(c) 
		{
			case GSTUFF_START:
				*outdata++ = GSTUFF_STUB;
				*outdata++ = GSTUFF_STUB_START;
				break;
		
			case GSTUFF_STUB:
				*outdata++ = GSTUFF_STUB;
				*outdata++ = GSTUFF_STUB_STUB;
				break;
	
			default:
				*outdata++ = c;
		}
	}

	*outdata++ = crc;
	*outdata++ = GSTUFF_START;

	return outdata - outstrt;
}