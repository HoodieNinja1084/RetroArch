#include "netSharedDefines.h"
#include "netPackets.h"

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
    si_me2.sin_addr.s_addr = inet_addr(inet_ntoa(addr.sin_addr));

    connect(s2, (struct sockaddr*) &si_me2, sizeof(si_me2));

    packet_t cmsgme;
    cmsgme = build_packet(CMSG_ME, 42, "iPhone 4 de Tuxity");
    send(s2, &cmsgme, sizeof(cmsgme), 0);
  }

  getchar();

  return (0);
}
