// game.h

#ifndef GAME_H
#define GAME_H
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 50
#define BALL_RADIUS 5

typedef struct {
    int left_paddle_x, left_paddle_y;
    int right_paddle_x, right_paddle_y;
    int ball_x, ball_y;
    int ball_velocity_x, ball_velocity_y;
    int score_left, score_right; 
    int game_speed;
} GameCoordinates;

// Function to initialize the game
GameCoordinates init_game(void);

// Function to start the game loop
void start_game(void);

// Function to handle game input (e.g., keypresses)
void handle_game_input(GameCoordinates* coords);

// Function to update game state (e.g., positions of objects)
void update_game_state(GameCoordinates* coords);

// Function to render the game graphics
void render_game(GameCoordinates);

void draw_paddle(int x, int y, int width, int height, int color);
void draw_ball(int cx, int cy, int radius, int color);
int is_esc_pressed();
void draw_text(int x, int y, const char *text, int color);


//Adjustments for font inclusion

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

typedef struct {
    unsigned char data[256][FONT_HEIGHT];
} Font;

void init_font(Font *font);

#endif // GAME_H
