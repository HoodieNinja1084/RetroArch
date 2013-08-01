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

packet_t build_packet(uint32_t, ...);
uint32_t calculate_size(va_list, const char*);
const char* serialize(uint32_t, va_list, const char*);

#endif /* !__NETPACKETS_H__ */
