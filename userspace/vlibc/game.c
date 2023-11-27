// game.c

#include "game.h"
#include "../../kernel/include/drivers/framebuffer/framebuffer.h" // Include your framebuffer and other libraries
#include "syscalls.h"
#include <stdio.h>

extern struct frame_buffer_info_type current_fb;
int left_paddle_x, left_paddle_y;
int right_paddle_x, right_paddle_y;
int ball_x, ball_y;

void init_game(void)
{
    printf("Welcome to init_game\n");
    // Retrieve the framebuffer dimensions and depth using syscalls
    int fb_width = fb_get_width();
    int fb_height = fb_get_height();
    
    if (fb_width < 0 || fb_height < 0)
    {
        printf("Error: Failed to get framebuffer width\n");
        return;
    }
    printf("Framebuffer dimensions (w*h) = %d x %d\n", fb_width, fb_height );
    // Initialize the positions of the paddles and the ball
    left_paddle_x = 30;                              
    left_paddle_y = (fb_height - PADDLE_HEIGHT) / 2; // Centered vertically

    right_paddle_x = fb_width - 30 - PADDLE_WIDTH;
    right_paddle_y = (fb_height - PADDLE_HEIGHT) / 2; // Centered vertically

    ball_x = (fb_width - BALL_SIZE) / 2;
    ball_y = (fb_height - BALL_SIZE) / 2;
    printf("Left Paddle: (%d,%d)\tRight Paddle: (%d,%d)\tBall: (%d,%d)\nReturning from init_game()\n",left_paddle_x,left_paddle_y,right_paddle_x,right_paddle_y,ball_x,ball_y);
}

void start_game(void)
{
    printf("Welcome to start_game\n");
    init_game();
    printf("Entering infinite while of start_game\n");
    while (1)
    {
        // handle_game_input();
        // update_game_state();
        render_game();

        // You might want to implement a delay here for timing control
        // Check for 'ESC' key press to exit the game loop
        if (is_esc_pressed())
        {
            break;
        }
    }
}

void handle_game_input(void)
{
    // Handle inputs (keyboard, GPIO, etc.)
    // This function will depend on your input handling implementation
}

void update_game_state(void)
{
    // Update your game's state (e.g., move objects, check for collisions)
}

void render_game(void)
{
    printf("Welcome to render_game\n");
    // framebuffer_clear_screen(0);    // Clear the screen
    fb_clear_screen(0);

    draw_rectangle(left_paddle_x, left_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xFFFFFF);   // // Draw left paddle White color
    draw_rectangle(right_paddle_x, right_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xFFFFFF); // // Draw right paddle White color
    // draw_circle(ball_x, ball_y, BALL_SIZE / 2, 0xFFFFFF); // Draw ball White color

    syscall_framebuffer_push(); // Update the display
}

// Implement is_esc_pressed() based on your input system
int is_esc_pressed()
{
    // Return 1 if ESC key is pressed
    return 0;
}

void draw_rectangle(int x, int y, int width, int height, int color)
{
    printf("Welcome to draw_rectangle\n");
    fb_hline(color, x, x + width, y);          // Top line
    fb_hline(color, x, x + width, y + height); // Bottom line
    fb_vline(color, y, y + height, x);         // Left line
    fb_vline(color, y, y + height, x + width); // Right line
}

// void plot_circle_points(int cx, int cy, int x, int y, int color) {
//     framebuffer_putpixel(color, cx + x, cy + y);
//     framebuffer_putpixel(color, cx - x, cy + y);
//     framebuffer_putpixel(color, cx + x, cy - y);
//     framebuffer_putpixel(color, cx - x, cy - y);
//     framebuffer_putpixel(color, cx + y, cy + x);
//     framebuffer_putpixel(color, cx - y, cy + x);
//     framebuffer_putpixel(color, cx + y, cy - x);
//     framebuffer_putpixel(color, cx - y, cy - x);
// }

// void draw_circle(int cx, int cy, int radius, int color) {
//     int x = 0, y = radius, p = (5 - radius*4)/4;
//     plot_circle_points(cx, cy, x, y, color);

//     while (x < y) {
//         x++;
//         if (p < 0) {
//             p += 2*x+1;
//         } else {
//             y--;
//             p += 2*(x-y)+1;
//         }
//         plot_circle_points(cx, cy, x, y, color);
//     }
// }

void delay(int milliseconds)
{
    volatile int i, j;
    for (i = 0; i < milliseconds; i++)
        for (j = 0; j < 10000; j++)
        {
        }
}
