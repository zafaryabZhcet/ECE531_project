// game.c

#include "./include/game.h"
#include "../../kernel/include/drivers/framebuffer/framebuffer.h" // to include framebuffer and other libraries
#include "syscalls.h"
#include <stdio.h>
#include <stdint.h>
// #include "../../kernel/drivers/framebuffer/c_font.h"


extern struct frame_buffer_info_type current_fb;

#define PADDLE_SPEED    10
#define BALL_SPEED_X    1
#define BALL_SPEED_Y    1
#define K_UP            0xa2
#define K_DOWN          0xa3

GameCoordinates init_game(void) {
    GameCoordinates coords;

    int fb_width = fb_get_width();
    int fb_height = fb_get_height();
    
    if (fb_width < 0 || fb_height < 0) {
        printf("Error: Failed to get framebuffer width\n");
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

    coords.ball_velocity_x = BALL_SPEED_X;
    coords.ball_velocity_y = BALL_SPEED_Y;

    coords.score_left = 0;
    coords.score_right = 0;
    coords.game_speed = 1;

    printf("Left Paddle: (%d,%d)\tRight Paddle: (%d,%d)\tBall: (%d,%d)\nReturning from init_game()\n", coords.left_paddle_x, coords.left_paddle_y, coords.right_paddle_x, coords.right_paddle_y, coords.ball_x, coords.ball_y);

    return coords;
}


void start_game(void) {
    GameCoordinates gameCoords = init_game();
    sys_set_game_active(1);

    while (1) {
        handle_game_input(&gameCoords);
        update_game_state(&gameCoords);
        render_game(gameCoords);

        nb_delay(20);  //smaller the smoother

        if (kb_esc_pressed()) {
            fb_clear_screen(0);
            syscall_framebuffer_push();
            sys_set_game_active(0);
            break;
        }
    }
}


void update_game_state(GameCoordinates *coords) {
 
    // Move the ball
    coords->ball_x += coords->ball_velocity_x;
    coords->ball_y += coords->ball_velocity_y;

    // Collision with top and bottom
    if (coords->ball_y - BALL_RADIUS <= 0 || coords->ball_y + BALL_RADIUS >= fb_get_height()) {
        coords->ball_velocity_y = -coords->ball_velocity_y;
    }

    // Collision with paddles
    if ((coords->ball_x - BALL_RADIUS <= coords->left_paddle_x + PADDLE_WIDTH && 
         coords->ball_y >= coords->left_paddle_y && 
         coords->ball_y <= coords->left_paddle_y + PADDLE_HEIGHT) ||
        (coords->ball_x + BALL_RADIUS >= coords->right_paddle_x && 
         coords->ball_y >= coords->right_paddle_y && 
         coords->ball_y <= coords->right_paddle_y + PADDLE_HEIGHT)) {
        coords->ball_velocity_x = -coords->ball_velocity_x;
    }

    // Check for scoring
    if (coords->ball_x < 0) {
        coords->score_right++;
        coords->ball_x = fb_get_width() / 2;
        coords->ball_y = fb_get_height() / 2;
    } else if (coords->ball_x > fb_get_width()) {
        coords->score_left++;
        coords->ball_x = fb_get_width() / 2;
        coords->ball_y = fb_get_height() / 2;
    }

    // Increase game speed after every 5 points
    if ((coords->score_left + coords->score_right) % 5 == 0) {
        coords->game_speed++;
    }

    // Adjust ball speed based on game speed
    coords->ball_velocity_x = BALL_SPEED_X * coords->game_speed;
    coords->ball_velocity_y = BALL_SPEED_Y * coords->game_speed;
}


void handle_game_input(GameCoordinates *coords) {
    int left_paddle_up = kb_key_state('w');
    int left_paddle_down = kb_key_state('s');
    int right_paddle_up = kb_key_state(K_UP);      
    int right_paddle_down = kb_key_state(K_DOWN);  
    int screen_height = fb_get_height();

    // printf("kb_key_state checked: %d %d %d %d\n", left_paddle_up, left_paddle_down, right_paddle_up, right_paddle_down);

    // Handle left paddle movement
    if (left_paddle_up && coords->left_paddle_y > 0) {
        coords->left_paddle_y -= PADDLE_SPEED;
    }
    if (left_paddle_down && coords->left_paddle_y < screen_height - PADDLE_HEIGHT) {
        coords->left_paddle_y += PADDLE_SPEED;
    }

    // Handle right paddle movement
    if (right_paddle_up && coords->right_paddle_y > 0) {
        coords->right_paddle_y -= PADDLE_SPEED;
    }
    if (right_paddle_down && coords->right_paddle_y < screen_height - PADDLE_HEIGHT) {
        coords->right_paddle_y += PADDLE_SPEED;
    }
}


void render_game(GameCoordinates coords)
{
    Font myFont;
    init_font(&myFont);
    int r=fb_clear_screen(30);
    if (r)
        printf("return value fb_clear_screen: %d\n", r);

    draw_paddle(coords.left_paddle_x, coords.left_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xFFFFFF);
    draw_paddle(coords.right_paddle_x, coords.right_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xFFFFFF);
    draw_ball(coords.ball_x, coords.ball_y, BALL_RADIUS, 0xFFFFFF); // Draw ball White color

    // Display scores
    char score_text[50];
    sprintf(score_text, "Left: %d Right: %d", coords.score_left, coords.score_right);
    // sprintf(score_text,"A");
    // printf("sctxt: %s\n",score_text);
    draw_text(&myFont, 10, 10, score_text, 0xFFFFFF); 

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

void nb_delay(int millisecs){
    int start_time = get_current_time();
    while ((get_current_time() - start_time) < millisecs){}
}



void draw_text(Font *font, int x, int y, const char* text, int color) {
    int orig_x = x;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            y += 18;  // Move to the next line
            x = orig_x;  // Reset to original X position
            continue;
        }
        uint8_t txt = text[i];
        for (int row = 0; row < 16; row++) {
            // printf("I am in draw_text row loop\n");
            //  unsigned char character_row = 0xAA;     //works fine
            unsigned char character_row = default_font[text[i] * 16 + row];

            // unsigned char character_row = default_font[256*row + text[i]];   //works but is a wrong logic
            // unsigned char character_row = default_font[txt][row];    //gives permission access error
            // printf("CR: %x\n", character_row);
            for (int col = 0; col < 8; col++) {
                if (character_row & (1 << (7 - col))) {
                    // printf("just above draw_text putpixel\n");
                    fb_putpixel(color, x + col, y + row);
                }
            }
        }
        x += 9;  // Move X to the next character position
    }
}

// void draw_text(Font *font, int x, int y, const char* text, int color) {
//     for (int i = 0; text[i] != '\0'; i++) {
//         if (text[i] == '\n') {
//             y += FONT_HEIGHT;
//             x = 0;
//             continue;
//         }

//         for (int row = 0; row < FONT_HEIGHT; row++) {
//             unsigned char character_row = font->data[(unsigned char)text[i]][row];
//             for (int col = 0; col < FONT_WIDTH; col++) {
//                 if (character_row & (1 << (7 - col))) {
//                     fb_putpixel(color, x + col, y + row);
//                 }
//             }
//         }
//         x += FONT_WIDTH;
//     }
// }
