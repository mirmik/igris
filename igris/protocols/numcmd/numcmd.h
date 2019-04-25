#ifndef IGRIS_PROTOCOL_NUMCMD_H
#define IGRIS_PROTOCOL_NUMCMD_H

struct numcmd_record
{
	uint32_t(*func)(uint32_t);
	uint8_t num;
};

#define NUMCMD_TBLFIN { NULL, 0 }

#endif