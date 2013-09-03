#include "netXFunc.h"

int xsocket(int domain, int type, int protocol)
{
   int sSocket;

   if ((sSocket = socket(domain, type, protocol)) == -1)
   {
      printf("Error on socket()\n");
      exit(EXIT_FAILURE);
   }

   return (sSocket);
}

int xbind(int socket, const struct sockaddr *address, socklen_t address_len)
{
   if (bind(socket, address, address_len) == -1)
   {
      printf("Error on bind()\n");
      close(socket);
      exit(EXIT_FAILURE);
   }

   return (0);
}

int xlisten(int socket, int backlog)
{
   if (listen(socket, backlog) == -1)
   {
      printf("Error on listen\n");
      close(socket);
      exit(EXIT_FAILURE);
   }

   return (0);
}

int xaccept(int socket, struct sockaddr * address, socklen_t * address_len)
{
   int cSocket;

   if ((cSocket = accept(socket, address, address_len)) < 0)
   {
      printf("Error on accept()\n");
      exit(EXIT_FAILURE);
   }

   return (cSocket);
}

int xselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
   int ret;

   ret = select(nfds, readfds, writefds, exceptfds, timeout);
   if (ret == -1)
   {
      printf("Error on select()\n");
      exit(EXIT_FAILURE);
   }

   return (ret);
}
