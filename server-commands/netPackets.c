#include "netPackets.h"

packet_t build_packet(uint32_t opcode, ...)
{
  packet_t packetData;
  va_list ap, aq;

  va_start(ap, opcode);
  va_copy(aq, ap);

  const char* format = opcodeTable[opcode].format;
  uint32_t size = calculate_size(aq, format);

  packetData.opcode = opcode;
  packetData.data = serialize(size, ap, format);

  va_end(ap);

  return (packetData);
}

const char* serialize(uint32_t packetSize, va_list ap, const char* format)
{
  char* saveBuffer;
  char* buffer = malloc(packetSize);

  saveBuffer = buffer;
  while (*format != '\0')
    {
      switch (*format)
	{
	case 'c':
	  {
	    uint8_t value = (uint8_t)va_arg(ap, int);
	    *buffer++ = (value) & 0xFF;
	    break;
	  }
	case 'i':
	  {
	    uint32_t value = va_arg(ap, int);
	    *buffer++ = (value >> 24) & 0xFF;
	    *buffer++ = (value >> 14) & 0xFF;
	    *buffer++ = (value >> 8) & 0xFF;
	    *buffer++ = (value) & 0xFF;
	    break;
	  }
	case 's':
	  {
	    char* value = va_arg(ap, char*);
	    memcpy(buffer, value, strlen(value));
	    buffer += strlen(value);
	    break;
	  }
	}
      format++;
    }

  return (buffer);
}

uint32_t calculate_size(va_list aq, const char* format)
{
  uint32_t packetSize = 0; // in bytes

  while (*format != '\0')
    {
      switch (*format)
	{
	case 'c':
	  {
	    va_arg(aq, int); 
	    packetSize += 1;
	    break;
	  }
	case 'i':
	  {
	    va_arg(aq, int);
      	    packetSize += 4;
	    break;
	  }
	case 's':
	  {
	    char* value = va_arg(aq, char*);
	    packetSize += 4 * strlen(value);
	    break;
	  }
	}
      format++;
    }

  va_end(aq);

  printf("Packet created with a length of %u bytes\n", packetSize);

  return (packetSize);
}
