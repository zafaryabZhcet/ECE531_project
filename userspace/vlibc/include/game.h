// game.h

#ifndef GAME_H
#define GAME_H
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 50
#define BALL_SIZE 10

// Function to initialize the game
int init_game(void);

// Function to start the game loop
void start_game(void);

// Function to handle game input (e.g., keypresses)
void handle_game_input(void);

// Function to update game state (e.g., positions of objects)
void update_game_state(void);

// Function to render the game graphics
void render_game(void);

void draw_rectangle(int x, int y, int width, int height, int color);
void draw_circle(int cx, int cy, int radius, int color);
int is_esc_pressed();

#endif // GAME_H
