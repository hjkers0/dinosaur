#include "raylib.h"
#include "nn.h"
#include <stdio.h>

#define WIDHT 1920
#define HEIGHT 1080

int main(void)
{
	Neuron n;
	n_init(&n, 4);

	for(int i = 0; i < 4; i++)
	{
		printf("Weight%d: %f", i, n.weights[i]);
	}

    InitWindow(WIDHT, HEIGHT, "Dinosaur game in C using NN");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

	n_free(&n);
    return 0;
}
