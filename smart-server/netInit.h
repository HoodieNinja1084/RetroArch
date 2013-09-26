#ifndef __INIT_H__
# define __INIT_H__

#include "netSharedDefines.h"
#include "netPackets.h"

void launch_server(void);
void init_server(network_t*);
uint32_t new_client(network_t*, uint8_t*);

#endif /* !__INIT_H__ */
