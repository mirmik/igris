#include <igris/util/crc.h>

// Dow-CRC using polynomial X^8 + X^5 + X^4 + X^0
// Tiny 2x16 entry CRC table created by Arjen Lentz
// See http://lentz.com.au/blog/calculating-crc-with-a-tiny-32-entry-lookup-table
static const uint8_t dscrc2x16_table[] = {
	0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
	0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
	0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
	0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74
};

// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (Use tiny 2x16 entry CRC table)
uint8_t crc8_table(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		crc = *addr++ ^ crc;  // just re-using crc as intermediate
		crc = dscrc2x16_table[crc & 0x0f] ^ dscrc2x16_table[16 + ((crc >> 4) & 0x0f)];
	}

	return crc;
}

//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but a little smaller, than the lookup table.
//
uint8_t crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
/*
bool check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc)
{
    crc = ~crc16(input, len, crc);
    return (crc & 0xFF) == inverted_crc[0] && (crc >> 8) == inverted_crc[1];
}

uint16_t crc16(const uint8_t* input, uint16_t len, uint16_t crc)
{
    for (uint16_t i = 0 ; i < len ; i++) {
        crc = _crc16_update(crc, input[i]);
    }

    static const uint8_t oddparity[16] =
        { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

    for (uint16_t i = 0 ; i < len ; i++) {
      // Even though we're just copying a byte from the input,
      // we'll be doing 16-bit computation with it.
      uint16_t cdata = input[i];
      cdata = (cdata ^ crc) & 0xff;
      crc >>= 8;

      if (oddparity[cdata & 0x0F] ^ oddparity[cdata >> 4])
          crc ^= 0xC001;

      cdata <<= 6;
      crc ^= cdata;
      cdata <<= 1;
      crc ^= cdata;
    }

    return crc;
}*/