#include "netServer.h"

void *launch_smartserver(void* args)
{
   fd_set readfs;

   init_server(&netInfo);

   send_broadcast_packet();

   uint8_t maxsocket = netInfo.sSocketUDP;
   while (1)
   {
      FD_ZERO(&readfs);
      FD_SET(netInfo.sSocketTCP, &readfs);

      uint8_t i;
      for (i = 0; i < netInfo.nbClients; i++)
         FD_SET(netInfo.clients[i]->socket, &readfs);

      int ret;
      struct timeval timeout = {3, 0};
      ret = xselect(maxsocket + 1, &readfs, NULL, NULL, &timeout);
      if (ret == 0)
      {
         // timeout reached
         send_broadcast_packet();
      }

      if (FD_ISSET(netInfo.sSocketTCP, &readfs))
      {
         uint32_t csock = new_client(&netInfo, &maxsocket);
         FD_SET(csock, &readfs);
      }
      else
      {
         // a client is talking
         for (i = 0; i < netInfo.nbClients; i++)
         {
            // find which one
            if (FD_ISSET(netInfo.clients[i]->socket, &readfs))
            {
               ssize_t r;
               packet_t pkt;
               client_t* client = netInfo.clients[i];

               r = recv(client->socket, &pkt, sizeof(pkt), 0);
               if (r == 0)
               {
                  disconnect_client(client);
                  break;
               }

               if (pkt.opcode >= OPCODE_MAXNUM)
               {
                  RARCH_ERR("Smart-Server: Unknow opcode 0x%02x, skipping...\n", pkt.opcode);
                  break;
               }

               struct s_OpcodeHandler opcode = opcodeTable[pkt.opcode];
               if ((*opcode.handler) == NULL)
               {
                  RARCH_WARN("Smart-Server: No handler defined for opcode %s(0x%02x), skipping...\n", opcode.name, pkt.opcode);
                  break;
               }

               RARCH_LOG("Smart-Server: Receiving opcode %s(0x%02x) from %s\n", opcode.name, pkt.opcode, client->ip);

               // call appropriate handler
               (*opcode.handler)(client, pkt.data);
            }
         }
      }
   }

   return (NULL);
}

void send_broadcast_packet(void)
{
   if (netInfo.nbClients < MAX_CLIENT)
   {
      packet_t pkt = build_packet(SMSG_WHO_IS_HERE);
      sendto(netInfo.sSocketUDP, &pkt, sizeof(pkt), 0, (struct sockaddr *)&netInfo.serverUDP, sizeof(netInfo.serverUDP));
   }
}

void disconnect_client(client_t* client)
{
   RARCH_LOG("Smart-Server: Client %s disconnected.\n", client->ip);
   close(client->socket);
   free(client);
   client = NULL;
   netInfo.nbClients--;
}
