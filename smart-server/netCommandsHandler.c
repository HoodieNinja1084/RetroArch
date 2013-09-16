#include "netCommandsHandler.h"
#include "libretro.h"
#include "driver.h"
#include "command.h"

void handle_me_opcode(client_t* client, unsigned char* data)
{
   data = deserialize_uint8(&data, &client->type); 
   data = deserialize_string(&data, client->name);
   printf("%s set informations: name '%s' type %u\n", client->ip, client->name, client->type);
}

void handle_info_opcode(client_t* client, unsigned char* data)
{
  // nyi
  // We have to find a way
}

// For every handle_button_* we call the associated function of RetroArch
void handle_button_b_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_B);
}

void handle_button_y_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_Y);
}

void handle_button_select_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_SELECT);
}

void handle_button_start_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_START);
}

void handle_button_up_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_UP);
}

void handle_button_down_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_DOWN);
}

void handle_button_left_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_LEFT);
}

void handle_button_right_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_RIGHT);
}

void handle_button_a_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_A);
}

void handle_button_x_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_X);
}

void handle_button_l_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_L);
}

void handle_button_r_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_R);
}

void handle_button_l2_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_L2);
}

void handle_button_r2_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_R2);
}

void handle_button_l3_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_L3);
}

void handle_button_r3_opcode(client_t* client, unsigned char* data)
{
  if (driver.command)
    rarch_cmd_set(driver.command, RETRO_DEVICE_ID_JOYPAD_R3);
}
