#ifndef __NETCOMMANDSHANDLER_H__
# define __NETCOMMANDSHANDLER_H__

#include "netSharedDefines.h"
#include "netPackets.h"

void handle_me_opcode(client_t*, unsigned char*);
void handle_button_pressed_opcode(client_t*, unsigned char*);
void handle_button_released_opcode(client_t*, unsigned char*);

#endif /* !__NETCOMMANDSHANDLER_H__ */
