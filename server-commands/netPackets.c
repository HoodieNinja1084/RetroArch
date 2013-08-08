#include <string.h>
#include "netPackets.h"

packet_t build_packet(uint8_t opcode, ...)
{
  packet_t packet;
  va_list ap;

  printf("Sending packet with opcode 0x%02x\n", opcode);

  va_start(ap, opcode);
  packet.opcode = opcode;
  serialize(ap, opcodeTable[opcode].format, packet.data);
  va_end(ap);
  
  return (packet);
}

void serialize(va_list ap, const char* format, unsigned char* buffer)
{
  memset(buffer, '\0', MAX_BYTE);

  while (*format != '\0')
    {
      switch (*format)
	{
	case 'c':
	  {
	    uint8_t value = (uint8_t)va_arg(ap, int);
	    *buffer++ =  (value) & 0xFF;
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
	    //sprintf((char*)buffer, "%lu%s", strlen(value), value); // Do you really need to write/append str size before ?
	    sprintf((char*)buffer, "%s", value);
	    buffer += strlen(value);
	    break;
	  }
	}
      format++;
    }
}
