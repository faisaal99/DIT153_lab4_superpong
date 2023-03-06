#ifndef __DELAY_H__
#define __DELAY_H__


/**
 * @brief Block execution of program for 250 nanoseconds.
*/
void delay_250ns(void);

/**
 * @brief Block execution of program a specified number of mikroseconds.
*/
void delay_mikro(u32);

/**
 * @brief Block execution of program a specified number of milliseconds.
*/
void delay_milli(u32);


#endif // __DELAY_H__