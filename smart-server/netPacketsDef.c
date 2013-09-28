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
   {"CMSG_BUTTON_B",       "c",  &handle_button_b_opcode},
   {"CMSG_BUTTON_Y",       "c",  &handle_button_y_opcode},
   {"CMSG_BUTTON_SELECT",  "c",  &handle_button_select_opcode},
   {"CMSG_BUTTON_START",   "c",  &handle_button_start_opcode},
   {"CMSG_BUTTON_UP",      "c",  &handle_button_up_opcode},
   {"CMSG_BUTTON_DOWN",    "c",  &handle_button_down_opcode},
   {"CMSG_BUTTON_LEFT",    "c",  &handle_button_left_opcode},
   {"CMSG_BUTTON_RIGHT",   "c",  &handle_button_right_opcode},
   {"CMSG_BUTTON_A",       "c",  &handle_button_a_opcode},
   {"CMSG_BUTTON_X",       "c",  &handle_button_x_opcode},
   {"CMSG_BUTTON_L",       "c",  &handle_button_l_opcode},
   {"CMSG_BUTTON_R",       "c",  &handle_button_r_opcode},
   {"CMSG_BUTTON_L2",      "c",  &handle_button_l2_opcode},
   {"CMSG_BUTTON_R2",      "c",  &handle_button_r2_opcode},
   {"CMSG_BUTTON_L3",      "c",  &handle_button_l3_opcode},
   {"CMSG_BUTTON_R3",      "c",  &handle_button_r3_opcode},
   {"SMSG_QUIT_GAME",      "",   NULL},
   {"CMSG_QUIT_GAME",      "",   &handle_quit_game},
   {"CMSG_DISCONNECT",     "",   &handle_disconnect_opcode}
};
