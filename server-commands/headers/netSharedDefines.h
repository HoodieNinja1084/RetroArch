#ifndef __SHARED_DEFINES_H__
# define __SHARED_DEFINES_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "netXFunc.h"

# define TCP_PORT 3000
# define UDP_PORT 3001
# define MAX_CLIENT 4

# define BUFFER_SIZE 256
typedef struct client
{
  uint32_t socket;
  char name[BUFFER_SIZE];
} client_t;


typedef struct network
{
  uint32_t sSocketTCP;
  uint32_t sSocketUDP;
  struct sockaddr_in serverTCP;
  struct sockaddr_in serverUDP; 
  client_t* clients[MAX_CLIENT];
  uint32_t nbClients;
} network_t;

#endif /* !__SHARED_DEFINES_H__ */
