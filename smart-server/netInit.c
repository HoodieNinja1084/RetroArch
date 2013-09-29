#include "netInit.h"

void init_server(network_t* netInfo)
{
   uint32_t optval;

   netInfo->serverTCP.sin_addr.s_addr = htonl(INADDR_ANY);
   netInfo->serverTCP.sin_port = htons(TCP_PORT);
   netInfo->serverTCP.sin_family = AF_INET;
 
   netInfo->sSocketTCP = xsocket(AF_INET, SOCK_STREAM, 0);
   xbind(netInfo->sSocketTCP, (const struct sockaddr *)&netInfo->serverTCP, sizeof(netInfo->serverTCP));
   xlisten(netInfo->sSocketTCP, MAX_CLIENT);

   // active keepalive
   optval = 1;
   if (setsockopt(netInfo->sSocketTCP, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0)
      RARCH_ERR("Smart-Server: setsockopt SOL_SOCKET SO_KEEPALIVE error");

   optval = 15;
   if (setsockopt(netInfo->sSocketTCP, SOL_TCP, TCP_KEEPIDLE, &optval, sizeof(optval)) < 0)
      RARCH_ERR("Smart-Server: setsockopt SOL_TCP TCP_KEEPIDLE error");

   optval = 3;
   if (setsockopt(netInfo->sSocketTCP, SOL_TCP, TCP_KEEPCNT, &optval, sizeof(optval)) < 0)
      RARCH_ERR("Smart-Server: setsockopt SOL_TCP TCP_KEEPCNT error");

   optval = 10;
   if (setsockopt(netInfo->sSocketTCP, SOL_TCP, TCP_KEEPINTVL, &optval, sizeof(optval)) < 0)
      RARCH_ERR("Smart-Server: setsockopt SOL_TCP TCP_KEEPINTVL error");

   netInfo->sSocketUDP = xsocket(AF_INET, SOCK_DGRAM, 0);
   optval = 1;
   if (setsockopt(netInfo->sSocketUDP, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof(optval)) < 0)
   {
      RARCH_ERR("Smart-Server: setsockopt SO_BROADCAST error");
      exit(EXIT_FAILURE);
   }

   netInfo->serverUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);
   netInfo->serverUDP.sin_port = htons(UDP_PORT);
   netInfo->serverUDP.sin_family = AF_INET;

   netInfo->nbActiveClients = 0;

   RARCH_LOG("Smart-Server: Server is listenning on TCP port %d and UDP port %d\n", htons(netInfo->serverTCP.sin_port), htons(netInfo->serverUDP.sin_port));
}
