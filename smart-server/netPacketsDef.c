#include "netPacketsDef.h"
#include "netCommandsHandler.h"

/*
Format:
  c = char (int8)
  h = int16
  i = integer32
  s = string
*/
struct s_OpcodeHandler opcodeTable[OPCODE_MAXNUM] = {
   {"SMSG_NULL",           "",   NULL},
   {"SMSG_WHO_IS_HERE",    "",   NULL},
   {"CMSG_ME",             "cs", &handle_me_opcode},
   {"SMSG_INFO",           "",   NULL},
   {"CMSG_INFO",           "",   NULL},
   {"SMSG_LAUNCHING_GAME", "",   NULL},
   {"CMSG_BUTTON_PRESSED", "c",  &handle_button_pressed_opcode},
   {"CMSG_BUTTON_RELEASED", "c",  &handle_button_released_opcode},
   {"SMSG_QUIT_GAME",      "",   NULL},
   {"CMSG_QUIT_GAME",      "",   NULL},
   {"CMSG_DISCONNECT",     "",   NULL}
};

struct s_ButtonState buttonTable[16] = {
   {RETRO_DEVICE_ID_JOYPAD_B, 0},
   {RETRO_DEVICE_ID_JOYPAD_Y, 0},
   {RETRO_DEVICE_ID_JOYPAD_SELECT, 0},
   {RETRO_DEVICE_ID_JOYPAD_START, 0},
   {RETRO_DEVICE_ID_JOYPAD_UP, 0},
   {RETRO_DEVICE_ID_JOYPAD_DOWN, 0},
   {RETRO_DEVICE_ID_JOYPAD_LEFT, 0},
   {RETRO_DEVICE_ID_JOYPAD_RIGHT, 0},
   {RETRO_DEVICE_ID_JOYPAD_A, 0},
   {RETRO_DEVICE_ID_JOYPAD_X, 0},
   {RETRO_DEVICE_ID_JOYPAD_L, 0},
   {RETRO_DEVICE_ID_JOYPAD_R, 0},
   {RETRO_DEVICE_ID_JOYPAD_L2, 0},
   {RETRO_DEVICE_ID_JOYPAD_R2, 0},
   {RETRO_DEVICE_ID_JOYPAD_L3, 0}
};
