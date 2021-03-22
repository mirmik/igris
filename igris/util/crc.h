#ifndef GXX_UTIL_CRC_H
#define GXX_UTIL_CRC_H

#include <igris/compiler.h>
#include <stdbool.h>
#include <stdint.h>

static inline void strmcrc8(uint8_t *crc, char c)
{
    *crc ^= (uint8_t)c;
    for (int i = 0; i < 8; i++)
        *crc = *crc & 0x80 ? ((uint8_t)(((uint8_t)(*crc << 1)) ^ 0x31))
                           : ((uint8_t)(*crc << 1));
}

__BEGIN_DECLS

uint8_t crc8(const void *data, uint8_t length, uint8_t crc_init);
uint16_t crc16(const void *data, uint16_t length, uint16_t crc_init);
uint32_t crc32(const void *data, uint32_t length, uint32_t crc_init);

uint8_t mmc_crc7(const uint8_t *message, const uint8_t length);
uint8_t crc8_table(const uint8_t *addr, uint8_t len, uint8_t crc_init);
bool check_crc16(const uint8_t *input, uint16_t len,
                 const uint8_t *inverted_crc, uint16_t crc);

__END_DECLS

#endif
