#ifndef __NETPACKETSDEF_H__
# define __NETPACKETSDEF_H__

#include "netSharedDefines.h"

enum Opcode {
  SMSG_WHO_IS_HERE	= 0x01,
  CMSG_ME		= 0x02,
  SMSG_INFO		= 0x03,
  CMSG_INFO		= 0x04,
  SMSG_LAUNCHING_GAME	= 0x05,
  CMSG_BUTTON_B		= 0x06,
  CMSG_BUTTON_Y		= 0x07,
  CMSG_BUTTON_SELECT	= 0x08,
  CMSG_BUTTON_START	= 0x09,
  CMSG_BUTTON_UP	= 0x0A,
  CMSG_BUTTON_DOWN	= 0x0B,
  CMSG_BUTTON_LEFT	= 0x0C,
  CMSG_BUTTON_RIGHT	= 0x0D,
  CMSG_BUTTON_A		= 0x0E,
  CMSG_BUTTON_X		= 0x0F,
  CMSG_BUTTON_L		= 0x10,
  CMSG_BUTTON_R		= 0x11,
  CMSG_BUTTON_L2	= 0x12,
  CMSG_BUTTON_R2	= 0x13,
  CMSG_BUTTON_L3	= 0x14,
  CMSG_BUTTON_R3	= 0x15,
  SMSG_QUIT_GAME	= 0x16,
  OPCODE_MAXNUM,
};

struct s_OpcodeHandler
{
  char const* name;
  char const* format;
  void (*handler)();
};

extern struct s_OpcodeHandler opcodeTable[OPCODE_MAXNUM];

#endif /* !__NETPACKETSDEF_H__ */
