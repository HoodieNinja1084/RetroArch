#include "netClient.h"

uint32_t new_client(network_t* netInfo)
{
   struct sockaddr_in csin;
   size_t csinsize = sizeof(csin);

   uint32_t csock = accept(netInfo->sSocketTCP, (struct sockaddr *)&csin, (socklen_t *)&csinsize);

   // setup new client
   client_t* client = malloc(sizeof(*client));
   client->isActive = 1;
   client->socket = csock;
   client->type = 0;
   strcpy(client->name, "undefined");
   strcpy(client->ip, inet_ntoa(csin.sin_addr));

   RARCH_LOG("Smart-Server: New client (%s) connected.\n", client->ip);

   int32_t idx = find_free_idx(netInfo);
   if (idx != -1)
   {
      free(netInfo->clients[idx]);
      netInfo->clients[idx] = client;
   }
   else
      netInfo->clients[netInfo->nbActiveClients] = client;
   netInfo->nbActiveClients++;

   return (client->socket);
}

void disconnect_client(network_t* netInfo, client_t* client, fd_set* readfs)
{
   RARCH_LOG("Smart-Server: Client %s disconnected.\n", client->ip);
   FD_CLR(client->socket, readfs);
   close(client->socket);
   client->isActive = 0;
   netInfo->nbActiveClients--;
}

int32_t find_free_idx(network_t* netInfo)
{
   uint8_t i;
   for (i = 0; i < MAX_CLIENT; i++)
   {
      if (netInfo->clients[i] && netInfo->clients[i]->isActive == 0)
         return i;
   }
   return -1;
}
