#ifndef __INIT_H__
# define __INIT_H__

#include "shared_defines.h"

void initServer(int*, int*, struct sockaddr_in*, struct sockaddr_in*);
void initClient(int*, int*, struct sockaddr_in*);

#endif /* !__INIT_H__ */
