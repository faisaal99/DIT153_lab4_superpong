#ifndef __KEYB_H__
#define __KEYB_H__

#include "typedef.h"


#define BUFFER_SIZE 4

/**
 * @brief A buffer used to send information about which keys are being
 *        pressed at a given moment. The maximum number of keys it can detect
 *        are `BUFFER_SIZE`. Integer values between 0-255;
*/
typedef struct
{
    u8 buffer[BUFFER_SIZE]; // All keystrokes are fed into this buffer.
    u8 n_presses;           // Number of keys that were pressed.
} Input;


/**
 * @brief Activates a specific row.
 * 
 * @param row The row on the keyboard to active.
*/
void activate_row(u32 row);


/**
 * @brief Reads each column and returns the index of the key being pressed,
 *        if that.
*/
int read_column();


/**
 * @brief Return the value of the key being pressed.
 * 
 * @param row The row of the key.
 * @param col The column of the key.
*/
u8 key_value(u32 row, u32 col);


/**
 * @brief Return the value of the key being pressed. This is high-level and 
 *        should be the way of communicating with the keyboard.
*/
u8 *keyb();


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