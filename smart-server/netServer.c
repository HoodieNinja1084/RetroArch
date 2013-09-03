#include "netServer.h"

void launch_smartserver(void)
{
   fd_set readfs;
   network_t netInfo;

   init_server(&netInfo);

   uint8_t max = netInfo.sSocketUDP;
   uint8_t actual = 0;
   while (1)
   {
      FD_ZERO(&readfs);
      FD_SET(netInfo.sSocketTCP, &readfs);

      uint8_t i;
      for (i = 0; i < actual; i++)
         FD_SET(netInfo.clients[i]->socket, &readfs);

      if (netInfo.nbClients < MAX_CLIENT)
      {
         packet_t pkt = build_packet(SMSG_WHO_IS_HERE);
         sendto(netInfo.sSocketUDP, &pkt, sizeof(pkt), 0, (struct sockaddr *)&netInfo.serverUDP, sizeof(netInfo.serverUDP));
      }

      struct timeval timeout = {3, 0};
      xselect(max + 1, &readfs, NULL, NULL, &timeout);

      if (FD_ISSET(netInfo.sSocketTCP, &readfs))
      {
         // new client
         netInfo.clients[actual] = new_client(&netInfo);
         netInfo.nbClients++;

         uint32_t csock = netInfo.clients[actual]->socket;
         FD_SET(csock, &readfs);
         max = csock > max ? csock : max;
         actual++;
      }
      else
      {
         // a client is talking
         for (i = 0; i < actual; i++)
         {
            // find which one
            if (FD_ISSET(netInfo.clients[i]->socket, &readfs))
            {
               client_t* client = netInfo.clients[i];

               packet_t pkt;
               recv(client->socket, &pkt, sizeof(pkt), 0);

               struct s_OpcodeHandler opcode = opcodeTable[pkt.opcode];
               printf("Receiving opcode %s(0x%02x)\n", opcode.name, pkt.opcode);

               // call appropriate handler
               (*opcode.handler)(client, pkt.data);
            }
         }
      }
   }
}
