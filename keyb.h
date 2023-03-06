#ifndef __KEYB_H__
#define __KEYB_H__


void activate_row(u32);      // Activates a specific row.
int read_column();            // Reads each column and returns the index of the
                              // key being pressed, if that.
u8 key_value(u32, u32);  // Return the value of the key being pressed.

/**
 * @brief A matrix with the key-values for the keypad.
 */
static const u8 KEYCODE[4][4] =
{ {  1, 2,  3, 10 }
, {  4, 5,  6, 11 }
, {  7, 8,  9, 12 }
, { 14, 0, 15, 13 }
};


#endif // __KEYB_H__