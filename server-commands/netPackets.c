#include "netPackets.h"

packet_t bluid_packet(uint32_t opcode, ...)
{
  packet_t packetData;
  va_list args;

  va_start(args, opcode);
  packetData.opcode = opcode;
  packetData.data = serialize(opcodeTable[opcode].format, args);
  va_end(args);

  return (packetData);
}

const char* serialize(const char* format, ...)
{
  va_list ap;
  uint32_t packetSize = 0; // in bytes

  char buffer[1024];

  va_start(ap, format);
  while (*format != '\0')
    {
      switch (*format)
	{
	case 'c':
	  {
	    char value = (char)va_arg(ap, int);
	    *buffer++ = value;
	    packetSize += 1;
	    //printf(">> %i\n", value);
	    break;
	  }
	case 'i':
	  {
	    htonl(va_arg(ap, int));
	    packetSize += 4;
	    buffer += 4;
	    //printf(">> %i\n", value);
	    break;
	  }
	case 's':
	  {
	    char* value = va_arg(ap, char*);
	    memcpy(buffer, value, strlen(value) +1);
	    packetSize += 4 * strlen(value);
	    //buffer += strlen(value);
	    break;
	  }
	}
      format++;
    }
  va_end(ap);

  printf("Packet created with length %i bytes\n", packetSize);

  return (buffer);
}
