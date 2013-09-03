#include "netSharedDefines.h"
#include "netPackets.h"

int main(int argc, char** argv)
{
  int s;
  struct sockaddr_in si_me;

  //s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  s = socket(AF_INET, SOCK_STREAM, 0);

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(3000);
  si_me.sin_addr.s_addr = inet_addr("127.0.0.1");;

  //bind(s, &si_me, sizeof(si_me));
  connect(s,(struct sockaddr*) &si_me, sizeof(si_me));

  packet_t buffer;
  //recv(s, &buffer, sizeof(buffer), 0);
  //  char* buf = buffer.data; // format is is
  //  unsigned int nb = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  // buf += 4;

  buffer = build_packet(CMSG_ME, 42, "Je suis un iPhone42s");
  while(1)
    {
      send(s, &buffer, sizeof(buffer), 0);
    }
  //printf(">>> opcode %u\n", buffer.opcode);
  //printf(">>> data : %s<<<<\n", buf);
  return (0);
}
