#ifndef __NETSERVER_H__
# define __NETSERVER_H__

#include "netSharedDefines.h"
#include "netInit.h"
#include "netClient.h"
#include "netPackets.h"
#include "netPacketsDef.h"

network_t netInfo;

void *launch_smartserver(void*);
void send_broadcast_packet(void);
int16_t ss_is_button_pressed(uint8_t, uint8_t);

#endif
