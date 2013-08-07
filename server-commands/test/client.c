#include "netSharedDefines.h"
#include "netPackets.h"

int main(int argc, char** argv)
{
  int s;
  struct sockaddr_in si_me;

  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(3001);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s, &si_me, sizeof(si_me));

  packet_t buffer;
  recv(s, &buffer, sizeof(buffer), 0);

  char* buf = buffer.data; // format is is
  unsigned int nb = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];

  buf += 4;

  printf(">>> nb %u\n", nb);
  printf(">>> data : %s<<<<\n", buf);
  return (0);
}
