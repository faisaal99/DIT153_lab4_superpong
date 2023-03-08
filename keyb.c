#include "keyb.h"

#include "memreg.h"


static Input input =
{
    { 0, 0, 0, 0 },
    0
};


Input *keyb(void)
{
    input.n_presses = 0;
    u8 n_assigns = BUFFER_SIZE;

    for (u32 row = 1; row <= 4; row++)
    {
        // If the maximum number of keypresses have been achieved, then stop
        // looking for more keys.
        if (n_assigns == 0) break;
        
        activate_row(row);
        u32 column = read_column();
        
        if (column)
        {
            input.buffer[BUFFER_SIZE - n_assigns] = key_value(row, column);
            input.n_presses++;
            n_assigns--;
        }
    }
    
    return &input;
}


void activate_row(u32 row)
{
    volatile gpio_t *gpiod = (gpio_t*)GPIOD;

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


int read_column()
{
    volatile gpio_t *gpiod = (gpio_t*)GPIOD;
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


u8 key_value(u32 row, u32 col)
{
    row--;
    col--;

    return KEYCODE[row][col];
}