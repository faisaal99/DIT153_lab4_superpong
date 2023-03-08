// =============================================================================
//                         INCLUDES & PRE-PROCESSOR

#include "ascii.h"
#include "ascii_game.h"
#include "graphics.h"
#include "display_driver.h"
#include "delay.h"
#include "memreg.h"
#include "typedef.h"


// =============================================================================
//									STRUCTS

/**
 * @brief Assuming ascii_display has been initiated
*/
void ascii_draw_score(P_Player p)
{
	
	ascii_goto(((p->display_position) + 7) , 2);
	ascii_write_char(((p -> points)+48));
}


void ascii_draw_name(P_Player p)
{	
	const char *s;
	ascii_goto((p->display_position), 1);
	s = &(p->name);
	
	// Print out each character.
	while (*s)
		ascii_write_char(*s++);
}


void ascii_init_game (P_Player p1, P_Player p2)
{
	char *s;
	ascii_draw_name(p1);
	ascii_draw_name(p2);

	const char score[] = "Score: ";	
	ascii_goto((p1 -> display_position), 2);	
	
	s = score;
	while (*s)
		ascii_write_char(*s++);
	
	ascii_goto((p2 -> display_position), 2);	
	
	s = score;
	while (*s)
		ascii_write_char(*s++);
}


void ascii_player_wins(P_Player p)
{
	graphic_clear_screen();
	//ascii_command(0b00000001, delay_milli,  2);
	char wins[] = "wins!";
	char* s;
	ascii_goto(1,1);
	s = (p -> name);
	while (*s)
		ascii_write_char(*s++);
	s = wins;
	while (*s)
		ascii_write_char(*s++);
}


void ascii_start_screen(void)
{
	ascii_command(0b00000001, delay_milli,  2);

	char welcome[] = "Welcome to Superpong!";
	char press[]   = "Press 5 to start.";

	ascii_goto(1, 1);
	
	char *s = welcome;
	while (*s)
		ascii_write_char(*s++);

	ascii_goto(1,2);

	s = press;
	
	while (*s)
		ascii_write_char(*s++);
}