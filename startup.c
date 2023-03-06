__attribute__((naked)) __attribute__((section (".start_section")))
void startup ( void )
{
    __asm__ volatile(" LDR R0,=0x2001C000\n");  /* Set stack    */
    __asm__ volatile(" MOV SP,R0\n");
    __asm__ volatile(" BL main\n");             /* Call main    */
    __asm__ volatile(".L1: B .L1\n");           /* Never return */
}

// =============================================================================
//                         INCLUDES & PRE-PROCESSOR

//#include "typedef.h"
//#include "memreg.h"
//#include "delay.h"
//#include "display_driver.h"
//#include "graphics.h"
//#include "keyb.h"


// =============================================================================
//                                GLOBAL DATA

//static gpio_t    *gpiod   = (gpio_t*)   GPIOD;   // GPIOD register
//static gpio_t    *gpioe   = (gpio_t*)   GPIOE;   // GPIOE register
//static systick_t *systick = (systick_t*)SYSTICK; // SysTick register


// =============================================================================
//                                  SET-UP

void app_init(void)
{
    // Start clocks for port D and port E.
 //   *(ulong*)0x40023830 = 0x18;
    // Starta clocks for SYSCFG
 //   *((ulong*)0x40023844) |= 0x4000;
}


// =============================================================================
//                                 FUNCTIONS

//


// =============================================================================
//                                    MAIN

void main(void)
{
    //
}
