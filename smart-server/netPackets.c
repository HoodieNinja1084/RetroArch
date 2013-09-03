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
            uint8_t value = (uint8_t)va_arg(ap, uint32_t);
            *buffer++ =  (value) & 0xFF;
            break;
         }
         case 'i':
         {
            uint32_t value = va_arg(ap, uint32_t);
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

unsigned char* deserialize_uint8(unsigned char** data, uint8_t* val)
{
   *val = *data[0];
   *data += sizeof(uint8_t); // shift pointer from 1 bytes (size of int on most of systems)

   return (*data);
}

unsigned char* deserialize_uint32(unsigned char** data, uint32_t* val)
{
   *val = ((*data)[0] << 24) | ((*data)[1] << 16) | ((*data)[2] << 8) | (*data)[3];
   *data += sizeof(uint32_t); // shift pointer from 4 bytes (size of int on most of systems)

   return (*data);
}

unsigned char* deserialize_string(unsigned char** data, char* str)
{
   uint32_t len;

   *data = deserialize_uint32(data, &len);
   strncpy(str, (char*)*data, len);
   *data += len;
   return (*data);
}
