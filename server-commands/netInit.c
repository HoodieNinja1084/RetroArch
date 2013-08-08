#include "netInit.h"

void init_server(network_t* netInfo)
{
  netInfo->serverTCP.sin_addr.s_addr = htonl(INADDR_ANY);
  netInfo->serverTCP.sin_port = htons(TCP_PORT);
  netInfo->serverTCP.sin_family = AF_INET;
 
  netInfo->sSocketTCP = xsocket(AF_INET, SOCK_STREAM, 0);
  xbind(netInfo->sSocketTCP, (const struct sockaddr *)&netInfo->serverTCP, sizeof(netInfo->serverTCP));
  xlisten(netInfo->sSocketTCP, MAX_CLIENT);

  netInfo->sSocketUDP = xsocket(AF_INET, SOCK_DGRAM, 0);
  uint32_t broadcast = 1;
  if (setsockopt(netInfo->sSocketUDP, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast, sizeof(broadcast)) < 0){
    printf("setsockopt error");
    exit(EXIT_FAILURE);
  }

  netInfo->serverUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  netInfo->serverUDP.sin_port = htons(UDP_PORT);
  netInfo->serverUDP.sin_family = AF_INET;

  netInfo->nbClients = 0;
}

client_t* new_client(network_t* netInfo)
{
  struct sockaddr_in csin;
  size_t csinsize = sizeof(csin);

  uint32_t csock = accept(netInfo->sSocketTCP, (struct sockaddr *)&csin, (socklen_t *)&csinsize);

  // Waiting for CMSG_ME from client
  packet_t pkt;
  recv(csock, &pkt, sizeof(pkt), 0);  
  
  // deserialize data from packet
  // int : phone type
  // string : phone name

  // setup new client
  client_t* client = malloc(sizeof(*client));
  client->socket = csock; 
  strcpy(client->name, "TODO");

  return (client);  
}
