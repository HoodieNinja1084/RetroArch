#include "netCommandsHandler.h"

void handle_me_opcode(client_t* client, char* data)
{
  client->type = deserialize_uint8(data);    
  strcpy(client->name, deserialize_string(data));
  printf("New client %s type %u\n", client->name, client->type);
}

void handle_info_opcode(client_t* client, char* data)
{
}

// For every handle_button_* we call the associated function of RetroArch
void handle_button_b_opcode(client_t* client, char* data)
{
}

void handle_button_y_opcode(client_t* client, char* data)
{
}

void handle_button_select_opcode(client_t* client, char* data)
{
}

void handle_button_start_opcode(client_t* client, char* data)
{
}

void handle_button_up_opcode(client_t* client, char* data)
{
}

void handle_button_down_opcode(client_t* client, char* data)
{
}

void handle_button_left_opcode(client_t* client, char* data)
{
}

void handle_button_right_opcode(client_t* client, char* data)
{
}

void handle_button_a_opcode(client_t* client, char* data)
{
}

void handle_button_x_opcode(client_t* client, char* data)
{
}

void handle_button_l_opcode(client_t* client, char* data)
{
}

void handle_button_r_opcode(client_t* client, char* data)
{
}

void handle_button_l2_opcode(client_t* client, char* data)
{
}

void handle_button_r2_opcode(client_t* client, char* data)
{
}

void handle_button_l3_opcode(client_t* client, char* data)
{
}

void handle_button_r3_opcode(client_t* client, char* data)
{
}
