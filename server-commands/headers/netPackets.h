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
unsigned char* deserialize_uint8(unsigned char**, uint8_t*);
unsigned char* deserialize_uint32(unsigned char**, uint32_t*);
unsigned char* deserialize_string(unsigned char**, char str[BUFFER_SIZE]);

#endif /* !__NETPACKETS_H__ */
