#ifndef __XFUNC_H__
# define __XFUNC_H__

#include "netSharedDefines.h"

int xsocket(int domain, int type, int protocol);
int xbind(int socket, const struct sockaddr *address, socklen_t address_len);
int xlisten(int socket, int backlog);
int xaccept(int socket, struct sockaddr * address, socklen_t * address_len);
int xselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#endif  /* !__XFUNC_H__ */
