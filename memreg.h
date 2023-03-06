#ifndef _KEYB_H_
#define _KEYB_H_


#include "typedef.h"


/* NVIC */

#define NVIC ((volatile u32*)0xE000E100)


/* GPIOD */

#define GPIOD 0x40020C00


/* GPIOE */

#define GPIOE 0x40021000


/* GPIO REGISTER */

/**
 * @brief A struct defining the layout of a GPIO register. Do NOT use the
 *        fields beginning with an underscore, as they represent reserved
 *        memory.
 */
typedef volatile struct
{
    union
    {
        u32 MODER;
        struct
        {
            u16 MODER_LOW;
            u16 MODER_HIGH;
        };
    };
    u16 OTYPER;
    u16 _otyper_reserved;
    u32 OSPEEDR;
    u32 PUPDR;
    union
    {
        u16 IDR;
        struct
        {
            u8 IDR_LOW;
            u8 IDR_HIGH;
        };
    };
    u16 _idr_reserved;
    union
    {
        u16 ODR;
        struct
        {
            u8 ODR_LOW;
            u8 ODR_HIGH;
        };
    };
    u16 _odr_reserved;
    u32 BSRR;
} gpio_t;


/* SysTick */

#define SYSTICK 0xE000E010


typedef volatile struct
{
    union
    {
        u32 CTRL;
        struct
        {
            u32 CTRL_ENABLE    :  1;
            u32 CTRL_TICKINT   :  1;
            u32 CTRL_CLKSOURCE :  1;
            u32                : 13;
            u32 CTRL_COUNTFLAG :  1;
        };
    };
    struct
    {
        u32 LOAD_VALUE : 24;
        u32            :  8;
    };
    struct
    {
        u32 VAL_VALUE : 24;
        u32           :  8;
    };
    u32 CALIB;
} systick_t;


/* SYSCFG */

typedef volatile struct
{
    u32 MEMRMP;              // Memory remap register

    union
    {
        u32 PMC;             // Peripheral mode configuration register
        struct
        {
            u32         : 16;
            u32 ADCxDC2 :  3;
            u32         :  4;
            u32 R_SEL   :  1;
        } PMC_B;
    };

    union
    {
        u16 EXTICR1;         // External interrupt configuration register 1
        struct
        {
            u16 EXTI0 : 4;
            u16 EXTI1 : 4;
            u16 EXTI2 : 4;
            u16 EXTI3 : 4;
        };
    };

    u16 : 16;

    union
    {
        u16 EXTICR2;         // External interrupt configuration register 2
        struct
        {
            u16 EXTI4 : 4;
            u16 EXTI5 : 4;
            u16 EXTI6 : 4;
            u16 EXTI7 : 4;
        };
    };

    u16 : 16;

    union
    {
        u16 EXTICR3;         // External interrupt configuration register 3
        struct
        {
            u16 EXTI8  : 4;
            u16 EXTI9  : 4;
            u16 EXTI10 : 4;
            u16 EXTI11 : 4;
        };
    };

    u16 : 16;

    union
    {
        u16 EXTICR4;         // External interrupt configuration register 4
        struct
        {
            u16 EXTI12 : 4;
            u16 EXTI13 : 4;
            u16 EXTI14 : 4;
            u16 EXTI15 : 4;
        };
    };
    u16 : 16;
} syscfg_t;

#define SYSCFG_CMPCR 0x40013820 // Compensation cell control register


/* CONTROL REGISTER BITS */

#define B_E      0x40       /* Enable-signal          */
#define B_SELECT 0x04       /* Choose ASCII-display   */
#define B_RW     0x02       /* 1 = Write, 0 = Read    */
#define B_RS     0x01       /* 1 = Data,  0 = Control */


/* SCB */ 

#define BIT_ENABLE(In, Bit)  (*In) |= (Bit)
#define BIT_DISABLE(In, Bit) (*In) &= (~Bit)

#define SCB_VTOR       ((volatile u32*)0xE000ED08)
#define SCB_RELOC_ADDR 0x2001C000

// SHPR - System Handler Priority Register
#define SCB_SHPR1 ((volatile u32*)0xE000ED18)
#define SCB_SHPR2 ((volatile u32*)0xE000ED1C)
#define SCB_SHPR3 ((volatile u32*)0xE000ED20)

// ICSR - Interrupt Control and State Register
#define SCB_ICSR ((volatile u32*)0xE000ED04)
#define BIT_NMI_PEND_SET (1<<32)

// CFSR - Configurable Fault Status Register
#define SCB_CFSR  ((volatile u32*)0xE000ED28)
#define SCB_UFSR  ((volatile u16*)0xE000ED2A)
#define SCB_BFSR  ((volatile u16*)0xE000ED29)
#define SCB_MMFSR ((volatile u16*)0xE000ED28)


/* TIMER-register */

typedef volatile struct
{
    union // Offset 0x00
    {
        u32 CR1;
        struct
        {
            u32 CEN  : 1; // Counter Enable
            u32 UDIS : 1; // Update disable
            u32 URS  : 1; // Update request source
            u32 OPM  : 1; // One-pulse mode
            u32      : 3;
            u32 ARPE : 1; // Auto-reload perload enable
        } CR1_B;
    };

    struct // Offset 0x04
    {
        u32     : 4;
        u32 MMS : 3; // Master mode selection
    } CR2;

    u32 : 32;
    
    union // Offset: 0x0C
    {
        u32 DIER;
        struct
        {
            u32 UIE : 1; // Update interrupt enable
            u32     : 7;
            u32 UDE : 1; // Update DMA request enable
        } DIER_B;
    };

    struct // Offset: 0x10
    {
        u32 UIF : 1; // Update Interrupt Flag
    } SR;

    struct // Offset: 0x14
    {
        u32 UG : 1; // Update generation
    } EGR;

    u32 : 32;
    u32 : 32;
    u32 : 32;

    u16 CNT; // Offset: 0x24
    u16 : 16;

    u16 PSC; // Offset: 0x28
    u16 : 16;

    u16 ARR; // Offset: 0x2C
    u16 : 16;
} tim_t;


/* TIMER 6 */

#define TIM6 0x40001000

#define TIM6_CR1 ((volatile u16*)0x40001000)
#define TIM6_SR  ((volatile u16*)0x40001010)
#define TIM6_CNT ((volatile u16*)0x40001024)
#define TIM6_ARR ((volatile u16*)0x4000102C)

#define NVIC_TIM6_IRQ_BPOS (1<<22)

#define NVIC_TIM6_ISER ((volatile u32*)0xE000E104)
#define NVIC_TIM6_ICER ((volatile u32*)0xE000E184)
#define NVIC_TIM6_ISPR ((volatile u32*)0xE000E204)
#define NVIC_TIM6_IABR ((volatile u32*)0xE000E304)

#define VTOR_TIM6_IRQ ((void(**)(void))(SCB_RELOC_ADDR + 0x118))


/* SHCSR - System Handler Control and State Register */

#define SHCSR 0xE000ED24


/* CFSR - Configurable Fault Status Register */

#define CFSR 0xE000ED28


/* EXTI - External Interrupt */

#define EXTI 0x40013C00


/* SYSCFG */

#define SYSCFG 0x40013800


/* EXTI */

typedef volatile struct
{
    struct             // Interrupt Mask Register
    {
        u32 IMR : 23;
        u32     :  9;
    };

    struct             // Event Mask Register
    {
        u32 EMR : 23;
        u32     :  9;
    };

    struct             // Rising Trigger Selection Register
    {
        u32 RTSR : 23;
        u32      :  9;
    };

    struct             // Failing Trigger Selection Register
    {
        u32 FTSR : 23;
        u32      :  9;
    };

    struct             // Software Interrupt Event Register
    {
        u32 SWIER : 23;
        u32       :  9;
    };

    struct             // Pending Register
    {
        u32 PR : 23;
        u32    :  9;
    };
} exti_t;

#define IRQ_EXTI0  ((void(**)(void))(SCB_RELOC_ADDR + 0x58))
#define IRQ_EXTI1  ((void(**)(void))(SCB_RELOC_ADDR + 0x5C))
#define IRQ_EXTI2  ((void(**)(void))(SCB_RELOC_ADDR + 0x60))
#define IRQ_EXTI3  ((void(**)(void))(SCB_RELOC_ADDR + 0x64))


#endif // _KEYB_H_