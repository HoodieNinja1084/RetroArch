#include <stdarg.h>
#include "shared_defines.h"
#include "netPacketsDef.h"

int pack(char* buffer, int opcode, ...)
{
  va_list ap;
  int packetSize = 0;
  char const* format = opcodeTable[opcode].format;

  va_start(ap, opcode);
  while (*format != '\0')
    {
      switch (*format)
	{
	case 'c':
	  {
	    char value = (char)va_arg(ap, int);
	    *buffer++ = value;
	    printf(">> %c\n", value);
	    break;
	  }
	case 'i':
	  {
	    int value = va_arg(ap, int);
	    *buffer++ = value >> 24;
	    *buffer++ = value >> 16;
	    *buffer++ = value >> 8;
	    *buffer++ = value;
	    printf(">> %i\n", value);
	    break;
	  }
	case 'f':
	  {
	    printf(">> %f\n", (float)va_arg(ap, double));
	    break;
	  }
	case 's':
	  {
	    char* value = va_arg(ap, char*);
	    memcpy(buffer, value, strlen(value) +1);
	    break;
	  }
	}
      format++;
    }
  va_end(ap);

  printf("Packet created with length %i\n", packetSize);

  return (packetSize);
}

int main()
{
  int cSocket;
  int sSocketTCP;
  int sSocketUDP;
  struct sockaddr_in client;
  struct sockaddr_in serverTCP;
  struct sockaddr_in serverUDP;

  char buffer[1024];

  init_server(&sSocketTCP, &sSocketUDP, &serverTCP, &serverUDP);
  printf("Server is listenning on port %d...\n", htons(serverTCP.sin_port));

  while (1)
    {
      pack(buffer, SMSG_INFO, 42, "Super-Mario");
  
      //char pkt[7] = "coucou\n";
      //sendto(sSocketUDP, (const char *)pkt, sizeof(pkt), 0L, (struct sockaddr *)&serverUDP, sizeof(serverUDP));
      sleep(3);
    }


  return (EXIT_SUCCESS);
}
