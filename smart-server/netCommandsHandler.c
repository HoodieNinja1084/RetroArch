#include "netCommandsHandler.h"

void handle_me_opcode(client_t* client, unsigned char* data)
{
   data = deserialize_uint8(data, &client->type);
   data = deserialize_string(data, client->name);
   RARCH_LOG("Smart-Server: %s set informations: name '%s' type %u\n", client->ip, client->name, client->type);
}

void handle_button_pressed_opcode(client_t* client, unsigned char* data)
{
   uint8_t key;

   data = deserialize_uint8(data, &key);

   buttonTable[key].pressed = 1;
}

void handle_button_released_opcode(client_t* client, unsigned char* data)
{
   uint8_t key;

   data = deserialize_uint8(data, &key);

   buttonTable[key].pressed = 0;
}
