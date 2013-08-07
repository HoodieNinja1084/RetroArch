#ifndef __NETPACKETS_H__
# define __NETPACKETS_H__

#include <stdarg.h>
#include "netSharedDefines.h"
#include "netPacketsDef.h"

#define MAX_BYTE 64

typedef struct packet
{
  uint8_t opcode:5; // 5 bits : 0 - 31
  unsigned char data[MAX_BYTE];
} packet_t;


packet_t build_packet(uint8_t, ...);
void serialize(va_list, const char*, unsigned char*);

#endif /* !__NETPACKETS_H__ */
