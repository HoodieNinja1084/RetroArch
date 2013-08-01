#include "shared_defines.h"
#include "netPackets.h"
#include "netPacketsDef.h"

int main()
{
  //uint32_t cSocket;
  uint32_t sSocketTCP;
  uint32_t sSocketUDP;
  //struct sockaddr_in client;
  struct sockaddr_in serverTCP;
  struct sockaddr_in serverUDP;

  init_server(&sSocketTCP, &sSocketUDP, &serverTCP, &serverUDP);
  printf("Server is listenning on port %d...\n", htons(serverTCP.sin_port));

  while (1)
    {
      packet_t pkt = build_packet(SMSG_INFO, 42, "Super-Mario");
      //char pkt[7] = "coucou\n";
      sendto(sSocketUDP, &pkt, sizeof(pkt), 0,(struct sockaddr *)&serverUDP, sizeof(serverUDP));
      sleep(3);
    }


  return (EXIT_SUCCESS);
}
