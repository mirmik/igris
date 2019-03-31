#ifndef GXX_UTIL_CRC_H
#define GXX_UTIL_CRC_H

#include <stdint.h>

static inline void strmcrc8(uint8_t* crc, char c) {
	*crc ^= (uint8_t)c;
	for (int i = 0; i < 8; i++)
		*crc = *crc & 0x80 ? ((uint8_t)(((uint8_t)(*crc << 1)) ^ 0x31)) : ((uint8_t)(*crc << 1));
}

#endif