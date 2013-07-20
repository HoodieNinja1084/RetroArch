#include "netPacketsDef.h"
#include "netCommandsHandler.h"

/*
Format:
  c = char
  i = integer
  f = float
  s = string
 */
struct s_OpcodeHandler opcodeTable[OPCODE_MAXNUM] = {
  {"SMSG_NULL",		"",	NULL},
  {"SMSG_WHO_IS_HERE",	"",	NULL},
  {"CMSG_ME",		"sc",	&HandleMeOpcode},
  {"SMSG_INFO",		"cs",	NULL},
  {"CMSG_INFO",		"c",	&HandleInfoOpcode},
  {"SMSG_LAUNCHING_GAME",	"",	NULL},
  {"CMSG_BUTTON_B",	"c",	&HandleButtonBOpcode},
  {"CMSG_BUTTON_Y",	"c",	&HandleButtonYOpcode},
  {"CMSG_BUTTON_SELECT",	"c",	&HandleButtonSelectOpcode},
  {"CMSG_BUTTON_START",	"c",	&HandleButtonStartOpcode},
  {"CMSG_BUTTON_UP",	"c",	&HandleButtonUpOpcode},
  {"CMSG_BUTTON_DOWN",	"c",	&HandleButtonDownOpcode},
  {"CMSG_BUTTON_LEFT",	"c",	&HandleButtonLeftOpcode},
  {"CMSG_BUTTON_RIGHT", "c",	&HandleButtonRightOpcode},
  {"CMSG_BUTTON_A",	"c",	&HandleButtonAOpcode},
  {"CMSG_BUTTON_X",	"c",	&HandleButtonXOpcode},
  {"CMSG_BUTTON_L",	"c",	&HandleButtonLOpcode},
  {"CMSG_BUTTON_R",	"c",	&HandleButtonROpcode},
  {"CMSG_BUTTON_L2",	"c",	&HandleButtonL2Opcode},
  {"CMSG_BUTTON_R2",	"c",	&HandleButtonR2Opcode},
  {"CMSG_BUTTON_L3",	"c",	&HandleButtonL3Opcode},
  {"CMSG_BUTTON_R3",	"c",	&HandleButtonR3Opcode},
  {"SMSG_QUIT_GAME",	"",	NULL}
};
