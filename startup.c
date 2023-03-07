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
}


// =============================================================================
//                                 FUNCTIONS

// Move this later
typedef struct
{
    bool is_colliding;
    i8   which;         // Left = 'l', Right = 'r', Up = 'u' and Down = 'd'
} WallCollision;

typedef struct
{
    const i8 *name;
    u32      points; 
    P_Object paddle;
} Player, *P_Player;

//

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
* @brief Moves the ball one "tick" by updating its coordinates with its speed.
*
* @param ball The ball to be moved
*/
void move_object(P_Object ball) 
{
    // Clear the ball temporarily from the screen
    ball->clear(ball);
    
    // Update the position of the ball
    ball->pos_x += ball->dir_x;
    ball->pos_y += ball->dir_y;
    
    // Draw the ball with its new positions
    draw_object(ball);
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
    i8 ball_min_x = ball->pos_x;
    i8 ball_max_x = ball->pos_x + ball->geo->size_x;
    i8 ball_min_y = ball->pos_y;
    i8 ball_max_y = ball->pos_y + ball->geo->size_y; 
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

// =============================================================================
//                       GLOBAL VARIABLES AND CONSTANTS 

static const char p1_name[] = "P1";
static const char p2_name[] = "P2";

Geometry ball_geometry = 
{
    12,     // numpoints
    4,      // size_x 
    4,      // size_y
    {
        {0,1},{0,2},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},{3,1},{3,2}
    }
};

static Object start_ball = {
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
    31,     // numpoints
    5,      // size_x 
    8,      // size_y
    {
        {0,0},{1,0},{2,0},{3,0},{4,0},                          // Upper wall
        {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},  // Left wall
        {4,0},{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},{4,8},  // Right wall
        {0,8},{1,8},{2,8},{3,8},{4,8},                          // Lower wall
        {2,3},{2,4},{2,5}                                       // Middle section
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
    "Player 1",     // name
    0,              // points
    &left_paddle    // paddle
};

static Player player_2 =
{
    "Player 2",     // name
    0,              // points
    &right_paddle   // paddle
};


// =============================================================================
//                                    MAIN

// move Mmove ball to the middle of the screen /spawning position//ball 
// flickers on apause for a sceecond

void main(void)
{
    // Initialize application
    // ...

    // Initializing the ball and the players
    P_Object ball = &start_ball;
    P_Player p1   = &player_1;
    P_Player p2   = &player_2;

    // Gameplay-loop
    while (true)
    {
        // Read general input
        Input *keyb_input = keyb();
        u8 player_1_dy = 0;
        u8 player_2_dy = 0;
        
        if (keyb_input->n_presses > 0)
        {
            // Read Player 1 input
            for (u8 i = 0; i < BUFFER_SIZE; i++)
            {
                keyb_input->buffer[i]
            }
            
            // Read Player 2 input
        }

        // Move ball / Collision-detection of ball
        ball->move(ball);

        if (colliding_with_paddles(ball, p1->paddle, p2->paddle))         
            ball -> dir_x *= -1;

        // If ball collided with right wall
        // -> Add point to Player 1
        // Else if ball collided with left wall
        // -> Add point to Player 2
        
        // Could perhaps move this to a function (but then players have to be passed as arguments)
        WallCollision wc = check_wall_collision(ball);
        if (wc.is_colliding) 
        {
            switch (wc.which) 
            {
                // Ball hit upper wall
                case 'u':
                    ball-> dir_y *= -1;
                    break;
                // Ball hit lower wall
                case 'd':
                    ball->dir_y *= -1;
                    break;
                // Ball hit left wall
                case 'l':
                    // Add point to Player 1
                    p1->points += 1;
                    break;
                // Ball hit right wall
                case 'r':
                    // Add point to Player 2
                    p2->points += 1;
                    break;
                // This shouldn't be reached
                default:
                    break;
            }
        }
        
        // If any player has reached max-points (3 points perhaps?)
        // Present game-over screen.
        // -> The player who won is presented, and an option to restart
        //    the game is available.
    }
}
