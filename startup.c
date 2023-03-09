// =========================================
//   =====================================
//     ==         Super Pong          ==
//     ==  Written by:     Faisal A.  ==
//     ==                  Eli U.     ==
//     ==                  Valter M.  ==
//   =====================================
// =========================================

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

#include "typedef.h"
#include "memreg.h"
#include "delay.h"
#include "display_driver.h"
#include "graphics.h"
#include "keyb.h"
#include "ascii_game.h"


// =============================================================================
//                                REGISTERS

static gpio_t    *gpiod   = (gpio_t*)   GPIOD;   // GPIOD register
static gpio_t    *gpioe   = (gpio_t*)   GPIOE;   // GPIOE register
static systick_t *systick = (systick_t*)SYSTICK; // SysTick register


// =============================================================================
//                                  SET-UP

void app_init(void)
{
    // Start clocks for port D and port E.
    *(ulong*)0x40023830 = 0x18;
    // Starta clocks for SYSCFG
    *(ulong*)0x40023844 |= 0x4000;

    gpiod->MODER_LOW  =     0x5555;
    gpiod->MODER_HIGH =     0x5500;
    gpiod->OSPEEDR    = 0x55555555;
    gpiod->OTYPER    &=     0x00FF;
    gpiod->PUPDR     &= 0x0000FFFF;
    gpiod->PUPDR     |= 0x00AA0000;


    gpioe->MODER   = 0x00005555;
    gpioe->OSPEEDR = 0x55555555;
}


// =============================================================================
//                                 FUNCTIONS

void wait_for_start_press()
{
    bool start = false;
    while (true)
    {
        Input *keyb_input = keyb();

        for (u8 i = 0; i < keyb_input->n_presses; i++)
        {
            u8 current_input = keyb_input->buffer[i];
            switch (current_input)
            {
            case 5:
                start = true;
            default:
                break; // To handle any eventual behaviour.
            }
        }

        if (start) break;
    }
}

/**
* @brief Checks if the ball collides with the paddle.
*
* @param ball   The ball to detect a collision with.
* @param paddle The Paddle to detect a collision with.
*/
bool colliding_with_paddle(P_Object ball, P_Object paddle)
{
    i8 ball_min_x = ball->pos_x;
    i8 ball_max_x = ball->pos_x + ball->geo->size_x;
    i8 ball_min_y = ball->pos_y;
    i8 ball_max_y = ball->pos_y + ball->geo->size_y;

    i8 paddle_min_x = paddle->pos_x;
    i8 paddle_max_x = paddle->pos_x + paddle->geo->size_x;
    i8 paddle_min_y = paddle->pos_y;
    i8 paddle_max_y = paddle->pos_y + paddle->geo->size_y;

    return
        ball_min_x <= paddle_max_x
        &&
        ball_max_x >= paddle_min_x
        &&
        ball_min_y <= paddle_max_y
        &&
        ball_max_y >= paddle_min_y;
}

/**
* @brief Checks if the ball collides with one of the paddles.
*
* @param ball     The ball to detect a collision with.
* @param l_paddle The left paddle to detect a collision with.
* @param r_paddle The right paddle to detect a collision with.
*/
bool colliding_with_paddles(P_Object ball, P_Object l_paddle, P_Object r_paddle)
{
    bool left_collision = colliding_with_paddle(ball, l_paddle);
    bool right_collision = colliding_with_paddle(ball, r_paddle);
    return left_collision || right_collision;
}

/**
* @brief Moves an object one "tick" by updating its coordinates with its speed.
*
* @param object The object to be moved
*/
void move_object(P_Object object)
{
    // Clear the ball temporarily from the screen
    object->clear(object);

    // Update the position of the ball
    object->pos_x += object->dir_x;
    object->pos_y += object->dir_y;

    // Draw the ball with its new positions
    draw_object(object);
}

/**
* @brief Sets the speed of the given object
*
* @param object The object to set the speed for
* @param speed_x The speed in horizontal direction
* @param speed_y The speed in vertical direction
*/
void set_object_speed(P_Object object, int speed_x, int speed_y)
{
    object->dir_x = speed_x;
    object->dir_y = speed_y;
}

/**
* @brief Checks if the ball collides with a wall. Returns information about
*        the possible collision in the form of a WallCollision struct.
*
* @param ball The ball to check collision with the walls.
*/
WallCollision check_wall_collision(P_Object ball)
{
    i16 ball_min_x = ball->pos_x;
    i16 ball_max_x = ball->pos_x + ball->geo->size_x;
    i16 ball_min_y = ball->pos_y;
    i16 ball_max_y = ball->pos_y + ball->geo->size_y;
    WallCollision result;

    // Check left wall collision
    if (ball_min_x < 1) {
        result = (WallCollision) { .is_colliding = true, .which = 'l'};
        return result;
    }
    // Check right wall collision
    if (ball_max_x > 128) {
        result = (WallCollision) { .is_colliding = true, .which = 'r'};
        return result;
    }
    // Check upper wall collision
    if (ball_min_y < 1) {
        result = (WallCollision) { .is_colliding = true, .which = 'u'};
        return result;
    }
    // Check lower wall collision
    if (ball_max_y > 64) {
        result = (WallCollision) { .is_colliding = true, .which = 'd'};
        return result;
    }
    // When no wall collision occurs
    result = (WallCollision){ false };
    return result;
}

/**
* @brief Resets the ball and the paddle to their initial positions
*
* @param ball         The ball to reset
* @param left_paddle  The left paddle to reset
* @param right_paddle The right paddle to reset
*/
void reset_game_objects(
    P_Object ball,
    P_Object left_paddle,
    P_Object right_paddle
)
{
    // Reset ball
    ball->dir_x =  5;
    ball->dir_y =  0;
    ball->pos_x = 62;
    ball->pos_y = 30;

    // Reset paddles
    left_paddle->dir_x =  0;
    left_paddle->dir_y =  0;
    left_paddle->pos_x = 10;
    left_paddle->pos_y = 30;

    right_paddle->dir_x =   0;
    right_paddle->dir_y =   0;
    right_paddle->pos_x = 110;
    right_paddle->pos_y =  30;
}


void game_over(P_Player p)
{
    ascii_player_wins(p);
    delay_milli(5000);
}


// =============================================================================
//                       GLOBAL VARIABLES AND CONSTANTS

Geometry ball_geometry =
{
    12,     // numpoints
    4,      // size_x
    4,      // size_y
    {
        {0,1},{0,2},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},{3,1},{3,2}
    }
};


static Object ball =
{
    &ball_geometry,
    0,0,            // Initial direction
    1,1,            // Initial startposition
    draw_object,
    clear_object,
    move_object,
    set_object_speed
};


Geometry paddle_geometry =
{
    31, // numpoints
    5,  // size_x
    8,  // size_y
    {
        {0,0},{1,0},{2,0},{3,0},{4,0},                         // Upper wall
        {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8}, // Left wall
        {4,0},{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},{4,8}, // Right wall
        {0,8},{1,8},{2,8},{3,8},{4,8},                         // Lower wall
        {2,3},{2,4},{2,5}                                      // Middle section
    }
};


static Object right_paddle =
{
    &paddle_geometry,
    0,0,                // Initial direction
    110,50,             // Start position
    draw_object,
    clear_object,
    move_object,
    set_object_speed
};


static Object left_paddle =
{
    &paddle_geometry,
    0,0,                // Initial direction
    10,50,              // Start position
    draw_object,
    clear_object,
    move_object,
    set_object_speed
};


static Player player_1 =
{
    "Player 1 ",            // Name
    PLAYER_1_ASCII_POS,
    0,                      // Points
    &left_paddle            // Paddle
};


static Player player_2 =
{
    "Player 2 ",             // Name
    PLAYER_2_ASCII_POS,
    0,                      // Points
    &right_paddle           // Paddle
};


// =============================================================================
//                                 MAIN

#define PLAYER1_UP  1
#define PLAYER1_DW  7
#define PLAYER2_UP  3
#define PLAYER2_DW  9
#define SPEED       2


void main(void)
{
    // Initialize application
    app_init();
    graphic_initialize();
    ascii_init();

    // Initializing the ball and the players

init_game:
    graphic_clear_screen();
    ascii_start_screen();
    wait_for_start_press();
    // Game reset
new_round:
    graphic_clear_screen();
    reset_game_objects(&ball, &left_paddle, &right_paddle);

    // Gameplay-loop
    while (true)
    {
        ascii_init_game(&player_1, &player_2);
        ascii_draw_score(&player_1);
	    ascii_draw_score(&player_2);

        // Read general input
        Input *keyb_input = keyb();
        i8 player_1_dy = 0;
        i8 player_2_dy = 0;

        const u8 MAX_SCORE = 3; // TODO: Used?

        if (keyb_input->n_presses > 0)
        {
            for (u8 i = 0; i < keyb_input->n_presses; i++)
            {
                u8 current_input = keyb_input->buffer[i];
                switch (current_input)
                {
                case PLAYER1_UP:
                    player_1_dy--; break;
                case PLAYER1_DW:
                    player_1_dy++; break;
                case PLAYER2_UP:
                    player_2_dy--; break;
                case PLAYER2_DW:
                    player_2_dy++; break;
                default:
                    break; // To handle any eventual behaviour.
                }
            }

        }

        // Set the speed of the paddles from the input of the keypad
        left_paddle.set_speed(&left_paddle,  0, player_1_dy * SPEED);
        right_paddle.set_speed(&right_paddle, 0, player_2_dy * SPEED);

        // Only move the paddles if they are inside of the screen
        if (3 < left_paddle.pos_y && left_paddle.pos_y < 53)
            left_paddle.move(&left_paddle);
        if (3 < right_paddle.pos_y && right_paddle.pos_y < 53)
            right_paddle.move(&right_paddle);


        // Move ball
        ball.move(&ball);

        //Collision-detection of ball with paddles
        if (colliding_with_paddles(&ball, &left_paddle, &right_paddle))
            ball.dir_x *= -1;


        // Checks for ball collision with walls.
        // Updates the game accordingly with the different wall collisions
        WallCollision wc = check_wall_collision(&ball);
        bool player_scored = false;
        if (wc.is_colliding)
        {
            switch (wc.which)
            {
                // Ball hit upper wall
                case 'u':
                    ball.dir_y *= -1;
                    break;

                // Ball hit lower wall
                case 'd':
                    ball.dir_y *= -1;
                    break;

                // Ball hit left wall
                case 'l':
                    player_2.points += 1;
                    player_scored = true;
                    break;

                // Ball hit right wall
                case 'r':
                    player_1.points += 1;
                    player_scored = true;
                    break;

                // This shouldn't be reached
                default:
                    break;
            }
        }

        if (player_1.points >= MAX_SCORE || player_2.points >= MAX_SCORE)
        {
            if (player_1.points >= MAX_SCORE)
                game_over(&player_1);
            else if (player_2.points >= MAX_SCORE)
                game_over(&player_2);
            player_1.points = 0;
            player_2.points = 0;
            goto init_game;
        }
        else if (player_scored)
            goto new_round;
    }
}
