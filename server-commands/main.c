#include "netSharedDefines.h"
#include "netInit.h"
#include "netPackets.h"
#include "netPacketsDef.h"

void launch_server(void)
{
  fd_set readfs;
  fd_set writefs;
  network_t netInfo;

  init_server(&netInfo);
  printf("Server is listenning on TCP port %d and UDP port %d\n", htons(netInfo.serverTCP.sin_port), htons(netInfo.serverUDP.sin_port));

  uint8_t max = netInfo.sSocketUDP;
  uint8_t actual = 0;
  while (1)
    {
      FD_ZERO(&readfs);
      FD_ZERO(&writefs);
      FD_SET(netInfo.sSocketTCP, &readfs);
      FD_SET(netInfo.sSocketUDP, &writefs);

      uint8_t i;
      for (i = 0; i < actual; i++)
	FD_SET(netInfo.clients[i]->socket, &readfs);

      select(max + 1, &readfs, &writefs, NULL, NULL);

      if (FD_ISSET(netInfo.sSocketUDP, &writefs))
	{
	  if (netInfo.nbClients < MAX_CLIENT)
	    {
	      packet_t pkt = build_packet(SMSG_WHO_IS_HERE);
	      sendto(netInfo.sSocketUDP, &pkt, sizeof(pkt), 0, (struct sockaddr *)&netInfo.serverUDP, sizeof(netInfo.serverUDP));
	      sleep(3);
	    }
	}
      if (FD_ISSET(netInfo.sSocketTCP, &readfs))
	{
	  // new client
	  netInfo.clients[actual] = new_client(&netInfo);
	  netInfo.nbClients++;

	  printf("New client connected.\n");

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

		  (*opcode.handler)(client, pkt.data);
		}
	    }
	}
    }
}

int main()
{
  launch_server();
  return (EXIT_SUCCESS);
}
