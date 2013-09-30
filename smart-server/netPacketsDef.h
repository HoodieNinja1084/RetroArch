#ifndef __NETPACKETSDEF_H__
# define __NETPACKETSDEF_H__

#include "netSharedDefines.h"

enum Opcode {
  SMSG_WHO_IS_HERE	= 0x01,
  CMSG_ME		= 0x02,
  SMSG_INFO		= 0x03,
  CMSG_INFO		= 0x04,
  SMSG_LAUNCHING_GAME	= 0x05,
  CMSG_BUTTON_PRESSED = 0x06,
  CMSG_BUTTON_RELEASED = 0x07,
  SMSG_QUIT_GAME	= 0x08,
  CMSG_QUIT_GAME	= 0x09,
  CMSG_DISCONNECT	= 0x0A,
  OPCODE_MAXNUM,
};

struct s_OpcodeHandler
{
  char const* name;
  char const* format;
  void (*handler)(client_t*, unsigned char*);
};

extern struct s_OpcodeHandler opcodeTable[OPCODE_MAXNUM];

struct s_ButtonState
{
  uint32_t id;
  uint8_t pressed;
};

extern struct s_ButtonState buttonTable[16];

#endif /* !__NETPACKETSDEF_H__ */
