#include "netPackets.h"

packet_t build_packet(uint8_t opcode, ...)
{
   packet_t packet;
   va_list ap;

   printf("Sending packet %s(0x%02x)\n", opcodeTable[opcode].name, opcode);

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
            buffer = serialize_uint8(buffer, (uint8_t)va_arg(ap, uint32_t));
            break;
         }
         case 'h':
         {
            buffer = serialize_uint16(buffer, (uint16_t)va_arg(ap, uint32_t));
            break;
         }
         case 'i':
         {
            buffer = serialize_uint32(buffer, va_arg(ap, uint32_t));
            break;
         }
         case 's':
         {
            buffer = serialize_string(buffer, va_arg(ap, char*));
            break;
         }
      }
      format++;
   }
}

unsigned char* serialize_uint8(unsigned char* buffer, uint8_t value)
{
   *buffer++ = value;
   return (buffer);
}

unsigned char* serialize_uint16(unsigned char* buffer, uint16_t value)
{
   *buffer++ = (value >> 8);
   *buffer++ = value;
   return (buffer);
}

unsigned char* serialize_uint32(unsigned char* buffer, uint32_t value)
{
   *buffer++ = (value >> 24);
   *buffer++ = (value >> 14);
   *buffer++ = (value >> 8);
   *buffer++ = value;
   return (buffer);
}

unsigned char* serialize_string(unsigned char* buffer, char* value)
{
   buffer = serialize_uint32(buffer, strlen(value));

   sprintf((char*)buffer, "%s", value);
   buffer += strlen(value);

   return (buffer);
}

unsigned char* deserialize_uint8(unsigned char* data, uint8_t* val)
{
   *val = data[0];
   data += sizeof(uint8_t);

   return (data);
}

unsigned char* deserialize_uint16(unsigned char* data, uint16_t* val)
{
   *val = (data[0] << 8) | data[1];
   data += sizeof(uint16_t);

   return (data);
}

unsigned char* deserialize_uint32(unsigned char* data, uint32_t* val)
{
   *val = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
   data += sizeof(uint32_t);

   return (data);
}

unsigned char* deserialize_string(unsigned char* data, char* str)
{
   uint32_t len;

   data = deserialize_uint32(data, &len);

   strncpy(str, (char*)data, len);
   data += len;

   return (data);
}
