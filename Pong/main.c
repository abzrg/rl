#include <raylib.h>

#define WINDOW_BACKGROUND_COLOR BLACK

#define PLAYER_SPEED 7
#define COMPUTER_SPEED 6

#define BALL_SPEED 7
#define BALL_RADIUS 20.0
#define BALL_COLOR WHITE

#define PADDLE_PADDING 10 // Padding for paddles near the edge of the window
#define PADDLE_WIDTH 25
#define PADDLE_HEIGHT 120
#define PADDLE_COLOR WHITE

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "PONG"
#define WINDOW_TITLE_MAX_LENGTH 256

#define GAME_LOOP_FPS 60

typedef struct {
    int x, y;
} PixelPos2;

typedef struct {
    unsigned int width, height;
    const char title[WINDOW_TITLE_MAX_LENGTH];
} Window;

typedef struct {
    int x, y;
    int speedX, speedY;
    float radius;
    Color color;
} Ball;

typedef struct {
    int x, y;
    float w, h;
    int speed;
    Color color;
} Paddle;

void DrawBall(Ball *ball);
void UpdateBall(Ball *ball);

void DrawPaddle(Paddle *paddle);
void UpperBoundHandlerPaddle(Paddle *paddle);
void UpdatePaddle(Paddle *paddle);
void UpdateCpuPaddle(Paddle *paddle, const Ball *ball);

void HandleCollision(const Paddle *paddle, Ball *ball);

// Score
int leftPlayerScore = 0;
int rightPlayerScore = 0;


int main(void)
{
    const Window w = {
        .width = WINDOW_WIDTH, .height = WINDOW_HEIGHT, .title = WINDOW_TITLE
    };

    Ball ball = {
        .x = w.width / 2, .y = w.height / 2,
        .speedX = BALL_SPEED, .speedY = BALL_SPEED,
        .radius = BALL_RADIUS, .color = BALL_COLOR
    };

    Paddle leftPlayer = {
        // x: 10px away from the left wall
        .x = PADDLE_PADDING,
        .y = w.height / 2 - PADDLE_HEIGHT / 2,
        .w = PADDLE_WIDTH, .h = PADDLE_HEIGHT,
        .speed = COMPUTER_SPEED, .color = PADDLE_COLOR,
    };

    Paddle rightPlayer = {
        // x: count for the paddle width and the padding
        .x = w.width - (PADDLE_PADDING + PADDLE_WIDTH),
        .y = w.height / 2 - PADDLE_HEIGHT / 2,
        .w = PADDLE_WIDTH, .h = PADDLE_HEIGHT,
        .speed = PLAYER_SPEED, .color = WHITE,
    };

    //- Create Window
    InitWindow(w.width, w.height, w.title);

    SetTargetFPS(GAME_LOOP_FPS);

    //- Game Loop
    while (!WindowShouldClose())
    {
        //- Draw
        BeginDrawing();

            ClearBackground(WINDOW_BACKGROUND_COLOR);
            DrawLine(w.width / 2.0, 0, w.width / 2.0, w.height, WHITE);

            // Update Ball's and Paddles' location
            UpdateBall(&ball);
            UpdatePaddle(&rightPlayer);
            UpdateCpuPaddle(&leftPlayer, &ball);

            // Handle collision and update the direction of ball movment
            HandleCollision(&rightPlayer, &ball);
            HandleCollision(&leftPlayer, &ball);

            // Draw the ball
            DrawBall(&ball);
            DrawPaddle(&rightPlayer);
            DrawPaddle(&leftPlayer);

            // Draw the scores
            const PixelPos2 leftPlayerScorePos = {
                .x = w.width / 4 - 20,
                .y = 20
            };
            const PixelPos2 rightPlayerScorePos = {
                .x = 3 * w.width / 4 - 20,
                .y = 20
            };
            const Color scoreColor = WHITE;
            const int scoreFontSize = 80;
            DrawText(TextFormat("%i", leftPlayerScore),
                     leftPlayerScorePos.x, leftPlayerScorePos.y,
                     scoreFontSize, scoreColor);
            DrawText(TextFormat("%i", rightPlayerScore),
                     rightPlayerScorePos.x, rightPlayerScorePos.y,
                     scoreFontSize, scoreColor);

        EndDrawing();
    }

    //- Destroy window
    CloseWindow();

    return 0;
}

void DrawBall(Ball *ball)
{
    DrawCircle(ball->x, ball->y, ball->radius, ball->color);
}

void ResetBall(Ball *ball)
{
    // Place in the center of the screen
    ball->x = GetScreenWidth() / 2;
    ball->y = GetScreenHeight() / 2;

    // Give it a random initial speed
    int speed_choice[2] = {-1, 1};
    ball->speedX *= speed_choice[GetRandomValue(0, 1)];
    ball->speedY *= speed_choice[GetRandomValue(0, 1)];
}

void UpdateBall(Ball *ball)
{
    const Window w = {.width = GetScreenWidth(), .height = GetScreenHeight()};

    if ((ball->y >= w.height - ball->radius) || (ball->y <= ball->radius)) {
        ball->speedY *= -1;
    }

    if (ball->x <= 0) {
        rightPlayerScore++;
        ResetBall(ball);
    } else if (ball->x >= (int)w.width) {
        leftPlayerScore++;
        ResetBall(ball);
    }

    ball->x += ball->speedX;
    ball->y += ball->speedY;
}

void DrawPaddle(Paddle *paddle)
{
    DrawRectangle(paddle->x, paddle->y, paddle->w,paddle->h,
                  paddle->color);
}

void UpdatePaddle(Paddle *paddle)
{
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_J)) {
        paddle->y += paddle->speed;
    } else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_K)) {
        paddle->y -= paddle->speed;
    }

    UpperBoundHandlerPaddle(paddle);
}

void UpperBoundHandlerPaddle(Paddle *paddle)
{
    const Window w = {.width = GetScreenWidth(), .height = GetScreenHeight()};

    if (paddle->y <= 0) {
        paddle->y = 0;
    } else if (paddle->y >= w.height - paddle->h) {
        paddle->y = w.height - paddle->h;
    }
}

void UpdateCpuPaddle(Paddle *paddle, const Ball *ball)
{
    if (paddle-> y + paddle->h / 2.0 >= ball->y ) {
        paddle->y -= paddle->speed;
    } else {
        paddle->y += paddle->speed;
    }

    // Randomly change the speed of paddle to make the game more exciting
    paddle->speed = GetRandomValue(3, 7);

    UpperBoundHandlerPaddle(paddle);
}

void HandleCollision(const Paddle *paddle, Ball *ball)
{
    if (CheckCollisionCircleRec((Vector2){ball->x, ball->y}, ball->radius,
                              (Rectangle){paddle->x, paddle->y, paddle->w,
                                          paddle->h})) {
        ball->speedX *= -1;
    }
}

// Reference: https://github.com/Pakz001/Raylib-Examples/blob/master/Functions_-_Clamp.c
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}
