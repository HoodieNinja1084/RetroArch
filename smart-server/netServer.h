#ifndef __NETSERVER_H__
# define __NETSERVER_H__

#include "netSharedDefines.h"
#include "netInit.h"
#include "netPackets.h"
#include "netPacketsDef.h"

network_t netInfo;

void *launch_smartserver(void*);
uint8_t find_max_socket(network_t*);
void send_broadcast_packet(void);

#endif
