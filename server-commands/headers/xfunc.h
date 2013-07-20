#ifndef __XFUNC_H__
# define __XFUNC_H__

#include "shared_defines.h"

int xsocket(int domain, int type, int protocol);
int xbind(int socket, const struct sockaddr *address, socklen_t address_len);
int xlisten(int socket, int backlog);
int xaccept(int socket, struct sockaddr * address, socklen_t * address_len);

#endif  /* !__XFUNC_H__ */
