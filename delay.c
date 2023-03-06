
#include "delay.h"
#include "typedef.h"
#include "memreg.h"


#define SIMULATOR 1

#if SIMULATOR
#define DELAY_COUNT 100
#else
#define DELAY_COUNT 1000000
#endif

// A value of 41 will delay for ~250ns.
static const u32 DELAY_VALUE = 168/4 - 1;

/**
 * @brief Blocks execution for ~250 nanoseconds.
 */
void delay_250ns(void)
{
    systick_t *systick = (systick_t*)SYSTICK;

    systick->CTRL       = 0;
    systick->LOAD_VALUE = DELAY_VALUE;
    systick->VAL_VALUE  = 0;
    systick->CTRL       = 5;

    // Wait until the countflag is enabled.
    while (systick->CTRL_COUNTFLAG == 0); // NOTE: VERY IMPORTANT THAT THIS IS VOLATILE.
    //while ( *(volatile u32*)(STK_CTRL & 0x00010000) == 0 );

    systick->CTRL = 0;
}


/**
 * @brief Delay the execution of code for a specified amount of mikroseconds.
 * @param us The duration to sleep in mikroseconds.
 */
void delay_mikro(u32 us)
{
#if SIMULATOR
    us /= 1000;
    us++;
#endif
    while (us > 0)
    {
        delay_250ns();
        delay_250ns();
        delay_250ns();
        delay_250ns();

        us--;
    }
}


/**
 * @brief Delay the execution of code for a specified amount of milliseconds.
 * @param ms The duration to sleep in milliseconds.
 */
void delay_milli(u32 ms)
{
#if SIMULATOR
    ms /= 1000;
    ms++;
#endif
    for (u32 i = 0; i < 1000 * ms; i++)
    {
        delay_250ns();
        delay_250ns();
        delay_250ns();
        delay_250ns();
    }
}