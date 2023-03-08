#ifndef __ASCII_H__
#define __ASCII_H__

#include "typedef.h"

void ascii_ctrl_bit_set(u8 x);
void ascii_ctrl_bit_clear(u8 x);
void ascii_write_controller(u8 byte);
u8 ascii_read_controller();
void ascii_goto(u32 row, u32 column);
void ascii_write_char(u8 c);
void ascii_init(void);

#endif // __ASCII_H__