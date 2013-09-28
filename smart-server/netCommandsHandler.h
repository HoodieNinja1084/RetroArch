#ifndef __NETCOMMANDSHANDLER_H__
# define __NETCOMMANDSHANDLER_H__

#include "netSharedDefines.h"
#include "netPackets.h"

void handle_me_opcode(client_t*, unsigned char*);
void handle_info_opcode(client_t*, unsigned char*);

void handle_button_b_opcode(client_t*, unsigned char*);
void handle_button_y_opcode(client_t*, unsigned char*);
void handle_button_select_opcode(client_t*, unsigned char*);
void handle_button_start_opcode(client_t*, unsigned char*);
void handle_button_up_opcode(client_t*, unsigned char*);
void handle_button_down_opcode(client_t*, unsigned char*);
void handle_button_left_opcode(client_t*, unsigned char*);
void handle_button_right_opcode(client_t*, unsigned char*);
void handle_button_a_opcode(client_t*, unsigned char*);
void handle_button_x_opcode(client_t*, unsigned char*);
void handle_button_l_opcode(client_t*, unsigned char*);
void handle_button_r_opcode(client_t*, unsigned char*);
void handle_button_l2_opcode(client_t*, unsigned char*);
void handle_button_r2_opcode(client_t*, unsigned char*);
void handle_button_l3_opcode(client_t*, unsigned char*);
void handle_button_r3_opcode(client_t*, unsigned char*);
void handle_quit_game(client_t*, unsigned char*);
void handle_disconnect_opcode(client_t*, unsigned char*);

#endif /* !__NETCOMMANDSHANDLER_H__ */
