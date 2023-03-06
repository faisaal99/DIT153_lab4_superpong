#include "typedef.h"
#include "memreg.h"
#include "keyb.h"

/**
 * @brief Read signals from the keyboard and determine which key is pressed,
 *        if one is pressed. If no key is pressed, return 0xFF.
 */
u8 keyb(void)
{
    for (u32 row = 1; row <= 4; row++)
    {
        activate_row(row);
        u32 column = read_column();
        if (column)
            return key_value(row, column);
    }

    return (u8)0xFF;
}


/**
 * @brief Activates the specific row on the keyboard.
 *
 * @param row The current row to index. Is 1-indexed. 1 <= row <= 4.
 */
void activate_row(u32 row)
{
    gpio_t *gpiod = (gpio_t*)GPIOD;

    switch (row)
    {
    case 1:
        gpiod->ODR_HIGH = 0x10; break;

    case 2:
        gpiod->ODR_HIGH = 0x20; break;

    case 3:
        gpiod->ODR_HIGH = 0x40; break;

    case 4:
        gpiod->ODR_HIGH = 0x80; break;

    default: break;
        // NAUGHTY MOVE
    }
}


/**
 * @brief Read each column from the keypad and return the first column that is
 *        enabled.
 */
int read_column()
{
    gpio_t *gpiod = (gpio_t*)GPIOD;
    u8 c = gpiod->IDR_HIGH;

    if ( c & 0b1000 )
        return 4;

    if ( c & 0b0100 )
        return 3;

    if ( c & 0b0010 )
        return 2;

    if ( c & 0b0001 )
        return 1;

    return 0;
}


/**
 * @brief Get the keyvalue corresponding to the row and column.
 *
 * @param row The row of the key being pressed. 1-indexed.
 * @param col The column of the key being pressed. 1-indexed.
 */
u8 key_value(u32 row, u32 col)
{
    row--;
    col--;

    return KEYCODE[row][col];
}