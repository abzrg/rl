#include <stdlib.h>
#include <time.h>

#include <raylib.h>

typedef struct {
    unsigned int width;
    unsigned int height;
    const char title[256];
} Window;

typedef struct {
    int x;
    int y;
    unsigned int radius;
    Color color;
} Ball;


int main(void)
{
    srand(time(NULL));

    Window w = {.width = 800, .height = 600, .title = "Moving Ball"};

    Ball ball = {
        .x = w.width / 2,
        .y = w.height / 2,
        .radius = 20,
        .color = WHITE
    };

    Color backgroundColor = {20, 160, 133, 255}; // Green-Cyan-ish
    int stepSize = 3;

    // ---

    //- Create window
    InitWindow(w.width, w.height, w.title);

    // Run the while loop 60 times per second
    // Change to see the difference in speed in which size of the ball changes.
    SetTargetFPS(60);

    //- Game Loop
    while (!WindowShouldClose())
    {
        //- 1. Event handling
        if (IsKeyDown(KEY_RIGHT)) {
            ball.x += stepSize;
        } else if (IsKeyDown(KEY_LEFT)) {
            ball.x -= stepSize;
        } else if (IsKeyDown(KEY_UP)) {
            // Note that Computer Graphics coordinate is different from standard
            // Cartesian coordinate. The origin is in the uppper-left corner and
            // going right and down increases the x and y values, respectively.
            ball.y -= stepSize;
        } else if (IsKeyDown(KEY_DOWN)) {
            ball.y += stepSize;
        }

        //- 2. Updating position
        ball.radius = 20 + rand() % 10;

        //- 3. Drawing
        BeginDrawing();
            ClearBackground(backgroundColor);
            DrawCircle(ball.x, ball.y, ball.radius, ball.color);
        EndDrawing();
    }

    //- Destroy window
    CloseWindow();

    return 0;
}
