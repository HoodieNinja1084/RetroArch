#ifndef __INIT_H__
# define __INIT_H__

#include "netSharedDefines.h"
#include "netPackets.h"

void launch_server(void);
void init_server(network_t*);
int32_t find_free_idx(network_t*);
uint32_t new_client(network_t*);
void disconnect_client(network_t*, client_t*, fd_set*);

#endif /* !__INIT_H__ */
