
#ifndef __ASCII_GAME_H__
#define __ASCII_GAME_H__

#include "graphics.h"

void ascii_draw_score(P_Player p);
void ascii_update_score(P_Player p);
void ascii_draw_name(P_Player p);
void ascii_init_game (P_Player p1, P_Player p2);
void ascii_player_wins(P_Player p);
void ascii_start_screen(void);
void ascii_init(void);

#endif // __ASCII_GAME_H__