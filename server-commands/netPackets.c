#include "netPackets.h"

packet_t bluid_packet(uint32_t opcode, ...)
{
  packet_t packetData;
  va_list args;

  va_start(args, opcode);

  const char* format = opcodeTable[opcode].format;
  uint32_t size = calculate_size(format, args);

  packetData.opcode = opcode;
  packetData.data = serialize(size, format, args);

  va_end(args);

  return (packetData);
}

uint32_t calculate_size(const char* format, ...)
{
  uint32_t packetSize = 0; // in bytes
  va_list ap;

  va_start(ap, format);
  while (*format != '\0')
    {
      switch (*format)
	{
	case 'c':
	  {
	    packetSize += 1;
	    break;
	  }
	case 'i':
	  {
      	    packetSize += 4;
	    break;
	  }
	case 's':
	  {
	    char* value = va_arg(ap, char*);
	    packetSize += 4 * strlen(value);
	    break;
	  }
	}
      format++;
    }
  va_end(ap);

  printf("Packet created with length %i bytes\n", packetSize);

  return (packetSize);
}

const unsigned char* serialize(uint32_t packetSize, const char* format, ...)
{
  va_list ap;

  unsigned char* buffer = malloc(packetSize);

  va_start(ap, format);
  while (*format != '\0')
    {
      switch (*format)
	{
	case 'c':
	  {
	    unsigned char value = (unsigned char)va_arg(ap, int);
	    *buffer++ = (value) & 0xFF;
	    break;
	  }
	case 'i':
	  {
	    int value = va_arg(ap, int);
	    *buffer++ = (value >> 24) & 0xFF;
	    *buffer++ = (value >> 16) & 0xFF;
	    *buffer++ = (value >> 8) & 0xFF;
	    *buffer++ = (value) & 0xFF;
	    break;
	  }
	case 's':
	  {
	    char* value = va_arg(ap, char*);
	    memcpy(buffer, value, strlen(value));
	    break;
	  }
	}
      format++;
    }
  va_end(ap);

  return (buffer);
}
