#ifndef __NETPACKETS_H__
# define __NETPACKETS_H__

#include <stdarg.h>
#include "shared_defines.h"
#include "netPacketsDef.h"

typedef struct packet
{
  uint32_t opcode;
  const char* data;
} packet_t;

packet_t bluid_packet(uint32_t, ...);
const char* serialize(const char*, ...);

#endif /* !__NETPACKETS_H__ */
