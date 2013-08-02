#include "init.h"

void init_server(uint32_t* sSocketTCP, uint32_t* sSocketUDP, struct sockaddr_in* serverTCP, struct sockaddr_in* serverUDP)
{
  serverTCP->sin_port = htons(TCP_PORT);
  serverTCP->sin_family = AF_INET;
  serverTCP->sin_addr.s_addr = htonl(INADDR_ANY);

  *sSocketTCP = xsocket(AF_INET, SOCK_STREAM, 0);
  xbind(*sSocketTCP, (const struct sockaddr *)serverTCP, sizeof(*serverTCP));
  xlisten(*sSocketTCP, MAX_CLIENT);

  *sSocketUDP = xsocket(AF_INET, SOCK_DGRAM, 0);
  uint32_t broadcast = 1;
  if (setsockopt(*sSocketUDP, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast, sizeof(broadcast)) < 0){
    printf("setsockopt error");
    exit(EXIT_FAILURE);
  }

  serverUDP->sin_port = htons(UDP_PORT);
  serverUDP->sin_family = AF_INET;
  //inet_pton(AF_INET, "172.255.255.255", &(serverUDP->sin_addr));
  serverUDP->sin_addr.s_addr = htonl(INADDR_BROADCAST);
}

void init_client(uint32_t* cSocket, uint32_t* sSocketTCP, struct sockaddr_in* client)
{
  socklen_t structLen;

  structLen = sizeof(*client);
  *cSocket = xaccept(*sSocketTCP, (struct sockaddr *)client, &structLen);
}
