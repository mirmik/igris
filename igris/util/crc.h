#ifndef GXX_UTIL_CRC_H
#define GXX_UTIL_CRC_H

#include <stdint.h>
#include <sys/cdefs.h>

static inline void strmcrc8(uint8_t* crc, char c) 
{
	*crc ^= (uint8_t)c;
	for (int i = 0; i < 8; i++)
		*crc = *crc & 0x80 ? ((uint8_t)(((uint8_t)(*crc << 1)) ^ 0x31)) : ((uint8_t)(*crc << 1));
}

__BEGIN_DECLS

uint8_t crc8_table(const uint8_t *addr, uint8_t len);
uint8_t crc8(const uint8_t *addr, uint8_t len);
bool check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc);
uint16_t crc16(const uint8_t* input, uint16_t len, uint16_t crc);

__END_DECLS

#endif