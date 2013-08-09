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

void serialize(va_list ap, const char* format, char* buffer)
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
	    uint32_t len = strlen(value);
	    
	    //append string lenght
	    *buffer++ = (len >> 24) & 0xFF;
	    *buffer++ = (len >> 14) & 0xFF;
	    *buffer++ = (len >> 8) & 0xFF;
	    *buffer++ = (len) & 0xFF;

	    // now string
	    sprintf((char*)buffer, "%s", value);
	    buffer += strlen(value);
	    break;
	  }
	}
      format++;
    }
}

uint8_t deserialize_uint8(char* data)
{
  uint8_t val;

  val = data[0];
  data += 1; // shift pointer from 1 bytes (size of int on most of systems)

  return (val);
}

uint32_t deserialize_uint32(char* data)
{
  uint32_t val;

  val = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
  data += 4; // shift pointer from 4 bytes (size of int on most of systems)

  return (val);
}

char* deserialize_string(char* data)
{
  uint32_t len;
  char* str;

  len = deserialize_uint32(data);
  str = malloc(sizeof(*str) * len);
  strncpy(str, data, len);
  data += len;
  
  return (str);
}
