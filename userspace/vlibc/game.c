// game.c

#include "./include/game.h"
#include "../../kernel/include/drivers/framebuffer/framebuffer.h" // Include your framebuffer and other libraries
#include "syscalls.h"
#include <stdio.h>

extern struct frame_buffer_info_type current_fb;

GameCoordinates init_game(void) {
    GameCoordinates coords;

    // printf("Welcome to init_game\n");
    // Retrieve the framebuffer dimensions and depth using syscalls
    int fb_width = fb_get_width();
    int fb_height = fb_get_height();
    
    if (fb_width < 0 || fb_height < 0) {
        printf("Error: Failed to get framebuffer width\n");
        // Handle the error, maybe by returning an empty struct or setting a flag
        return coords;
    }
    printf("Framebuffer dimensions (w*h) = %d x %d\n", fb_width, fb_height);

    // Initialize the positions of the paddles and the ball
    coords.left_paddle_x = 30;                              
    coords.left_paddle_y = (fb_height - PADDLE_HEIGHT) / 2;

    coords.right_paddle_x = fb_width - 30 - PADDLE_WIDTH;
    coords.right_paddle_y = (fb_height - PADDLE_HEIGHT) / 2;

    coords.ball_x = fb_width / 2;
    coords.ball_y = fb_height / 2;

    printf("Left Paddle: (%d,%d)\tRight Paddle: (%d,%d)\tBall: (%d,%d)\nReturning from init_game()\n", coords.left_paddle_x, coords.left_paddle_y, coords.right_paddle_x, coords.right_paddle_y, coords.ball_x, coords.ball_y);

    return coords;
}


void start_game(void)
{
    GameCoordinates gameCoords;
    // printf("Welcome to start_game\n");
    gameCoords = init_game();
    // printf("Entering infinite while of start_game\n");
    while (1)
    {
        
        render_game(gameCoords);
        delay(500);
        // Check for 'ESC' key press to exit the game loop
        if (kb_esc_pressed())
        {
            fb_clear_screen(0);
            syscall_framebuffer_push();
            break;
        }
    }
}

void handle_game_input(void)
{
    // Handle inputs (keyboard, GPIO, etc.)
    
}

void update_game_state(void)
{
    // Update your game's state (e.g., move objects, check for collisions)
}

void render_game(GameCoordinates coords)
{
    // printf("Welcome to render_game\n");
    // framebuffer_clear_screen(0);    // Clear the screen
    int r=fb_clear_screen(30);
    if (r)
        printf("return value fb_clear_screen: %d\n", r);

    draw_paddle(coords.left_paddle_x, coords.left_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xFFFFFF);
    draw_paddle(coords.right_paddle_x, coords.right_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xFFFFFF);
    draw_ball(coords.ball_x, coords.ball_y, BALL_SIZE / 2, 0xFFFFFF); // Draw ball White color

    syscall_framebuffer_push(); // Update the display
}



void draw_paddle(int x, int y, int width, int height, int color)
{
    // Iterate over each row in the rectangle's height
    for (int currentY = y; currentY < y + height; currentY++) {
        // Draw a horizontal line across the width of the rectangle at the current y-coordinate
        fb_hline(color, x, x + width, currentY);
    }
}


void plot_circle_points(int cx, int cy, int x, int y, int color) {
    fb_putpixel(color, cx + x, cy + y);//printf("(%d,%d)\t",cx+x,cy+y);
    fb_putpixel(color, cx - x, cy + y);//printf("(%d,%d)\t",cx-x,cy+y);
    fb_putpixel(color, cx + x, cy - y);//printf("(%d,%d)\t",cx+x,cy-y);
    fb_putpixel(color, cx - x, cy - y);//printf("(%d,%d)\t",cx-x,cy-y);
    fb_putpixel(color, cx + y, cy + x);//printf("(%d,%d)\t",cx+y,cy+x);
    fb_putpixel(color, cx - y, cy + x);//printf("(%d,%d)\t",cx-y,cy+x);
    fb_putpixel(color, cx + y, cy - x);//printf("(%d,%d)\t",cx+y,cy-x);
    fb_putpixel(color, cx - y, cy - x);//printf("(%d,%d)\n",cx-y,cy-x);
}

void plot_circle_lines(int cx, int cy, int x, int y, int color) {
    fb_hline(color, cx - x, cx + x, cy + y); // Draw line in upper hemisphere
    fb_hline(color, cx - x, cx + x, cy - y); // Draw line in lower hemisphere
}



void draw_ball(int cx, int cy, int radius, int color) {
    int x = 0, y = radius, p = (5 - radius*4)/4;
    plot_circle_points(cx, cy, x, y, color);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2*x+1;
        } else {
            y--;
            p += 2*(x-y)+1;
        }
        plot_circle_points(cx, cy, x, y, color);
    }
}

void delay(int milliseconds)
{
    volatile int i, j;
    for (i = 0; i < milliseconds; i++)
        for (j = 0; j < 10000; j++)
        {
        }
}
