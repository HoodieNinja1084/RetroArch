#ifndef __NETPACKETS_H__
# define __NETPACKETS_H__

#include <stdarg.h>
#include "shared_defines.h"
#include "netPacketsDef.h"

typedef struct packet
{
  uint32_t opcode;
  const unsigned char* data;
} packet_t;

packet_t bluid_packet(uint32_t, ...);
uint32_t calculate_size(const char*, ...);
const unsigned char* serialize(uint32_t, const char*, ...);

#endif /* !__NETPACKETS_H__ */
