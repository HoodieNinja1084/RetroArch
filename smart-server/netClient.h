#ifndef __NETCLIENT_H__
# define __NETCLIENT_H__

#include "netSharedDefines.h"

int32_t find_free_idx(network_t*);
uint32_t new_client(network_t*);
void disconnect_client(network_t*, client_t*, fd_set*);

#endif /* !__NETCLIENT_H__ */
