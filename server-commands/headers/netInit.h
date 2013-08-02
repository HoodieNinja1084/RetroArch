#ifndef __INIT_H__
# define __INIT_H__

#include "netSharedDefines.h"

void init_server(uint32_t*, uint32_t*, struct sockaddr_in*, struct sockaddr_in*);
void init_client(uint32_t*, uint32_t*, struct sockaddr_in*);

#endif /* !__INIT_H__ */
