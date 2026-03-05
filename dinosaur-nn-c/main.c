#include "raylib.h"

#define WIDHT 1920
#define HEIGHT 1080

int main(void)
{
    InitWindow(WIDHT, HEIGHT, "Dinosaur game in C using NN");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
