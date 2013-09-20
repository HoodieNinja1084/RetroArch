#include "../netSharedDefines.h"
#include "../netPackets.h"

struct actions
{
  char* str;
  uint32_t opcode;
};

static const struct actions map[] = {
  { "up", CMSG_BUTTON_UP},
  { "down", CMSG_BUTTON_DOWN},
  { "left", CMSG_BUTTON_LEFT},
  { "right", CMSG_BUTTON_RIGHT},
  { "x", CMSG_BUTTON_X},
  { NULL, 0},
};


int main(int argc, char** argv)
{
  int s1, s2;
  struct sockaddr_in si_me1, si_me2;

  socklen_t fromlen;
  struct sockaddr_in addr;

  s1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  si_me1.sin_family = AF_INET;
  si_me1.sin_port = htons(UDP_PORT);
  si_me1.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s1, (const struct sockaddr *)&si_me1, sizeof(si_me1));

  packet_t buffer;
  recvfrom(s1, &buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &fromlen);
  printf(">>> receive from %s\n", inet_ntoa(addr.sin_addr));
  printf(">>> opcode %u\n", buffer.opcode);
  printf(">>> data : %s<<<<\n", buffer.data);

  if (buffer.opcode == 1)
  {
    s2 = socket(AF_INET, SOCK_STREAM, 0);

    si_me2.sin_family = AF_INET;
    si_me2.sin_port = htons(TCP_PORT);
    si_me2.sin_addr.s_addr = inet_addr("192.168.1.75");

    connect(s2, (struct sockaddr*) &si_me2, sizeof(si_me2));

    packet_t cmsgme;
    cmsgme = build_packet(CMSG_ME, 42, "iPhone 4 de Tuxity");
    send(s2, &cmsgme, sizeof(cmsgme), 0);
    printf("sent\n");

    while (1)
    {
      size_t len = 0;
      char line[256];
      int n = read(0, &line, sizeof(line));
      line[strlen(line)-1] = '\0';

      int i;
      for (i = 0; map[i].str; i++)
      {
         if (!strcmp(map[i].str, line))
         {
           packet_t pkt;
           pkt = build_packet(map[i].opcode);
           send(s2, &pkt, sizeof(pkt), 0);
         }
      }
    }

  }

  return (0);
}
