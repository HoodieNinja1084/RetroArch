#include "netCommandsHandler.h"

void handle_me_opcode(client_t* client, unsigned char* data)
{
   data = deserialize_uint8(data, &client->type);
   data = deserialize_string(data, client->name);
   RARCH_LOG("Smart-Server: %s set informations: name '%s' type %u\n", client->ip, client->name, client->type);
}

// For every handle_button_* we call the associated function of RetroArch
void handle_button_b_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_B].pressed = 1;
}

void handle_button_y_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_Y].pressed = 1;
}

void handle_button_select_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_SELECT].pressed = 1;
}

void handle_button_start_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_START].pressed = 1;
}

void handle_button_up_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_UP].pressed = 1;
}

void handle_button_down_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_DOWN].pressed = 1;
}

void handle_button_left_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_LEFT].pressed = 1;
}

void handle_button_right_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_RIGHT].pressed = 1;
}

void handle_button_a_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_A].pressed = 1;
}

void handle_button_x_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_X].pressed = 1;
}

void handle_button_l_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_L].pressed = 1;
}

void handle_button_r_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_R].pressed = 1;
}

void handle_button_l2_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_L2].pressed = 1;
}

void handle_button_r2_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_R2].pressed = 1;
}

void handle_button_l3_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_L3].pressed = 1;
}

void handle_button_r3_opcode(client_t* client, unsigned char* data)
{
   buttonTable[RETRO_DEVICE_ID_JOYPAD_R3].pressed = 1;
}

void handle_quit_game(client_t* client, unsigned char* data)
{
  RARCH_WARN("Smart-Server: This opcode isn't handled yet\n");
}

void handle_disconnect_opcode(client_t* client, unsigned char* data)
{
  RARCH_WARN("Smart-Server: This opcode isn't handled yet\n");
}
