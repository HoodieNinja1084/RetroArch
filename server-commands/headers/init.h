#ifndef __INIT_H__
# define __INIT_H__

#include "shared_defines.h"

void init_server(int*, int*, struct sockaddr_in*, struct sockaddr_in*);
void init_client(int*, int*, struct sockaddr_in*);

#endif /* !__INIT_H__ */
