#include "raylib.h"
#include "nn.h"

#define WIDHT 1920
#define HEIGHT 1080
#define GRAVITY 1.5

typedef struct {
	Vector2			position;
	float			radius;
	Color			color;
	float			fitness;
	float			vy;

	int				alive;
	int				onGround;

	NeuralNetwork	brain;
} Circle;

typedef struct {
	int		size;
	float	startX;
	float	startY;
	Circle	*circles;
	int		generation;
} Population;

void c_jump(Circle *c)
{
	c->vy = -25.0;
}

void c_update(Circle *c, Rectangle floor)
{
	if (!c->onGround)
	{
		c->vy += GRAVITY;
		c->position.y += c->vy;
	}
	if (CheckCollisionCircleRec(c->position, c->radius, floor))
	{
		c->position.y = floor.y - c->radius;  
		c->vy = 0;
		c->onGround = 1;  
	}
	else
	{
		c->onGround = 0; 
	}
}

void p_update(Population *p, Rectangle floor)
{
	for (int i = 0; i < p->size; i++)
	{
		c_update(&p->circles[i], floor);
	}
}

void p_draw(Population *p)
{
	for (int i = 0; i < p->size; i++)
	{
		DrawCircleV(p->circles[i].position, p->circles[i].radius, p->circles[i].color);
	}
}

void p_init(Population *p, int size, float startX, float startY)
{
	p->size		= size;
	p->startX	= startX;
	p->startY	= startY;
	p->circles	= malloc(size * sizeof(Circle));
	
	for (int i = 0; i < size; i++)
	{
		// Initialize circles
		p->circles[i].position	= (Vector2){ startX, startY };
		p->circles[i].radius	= 40.0;
		p->circles[i].color		= BLUE;
		p->circles[i].alive		= 1;
		p->circles[i].fitness	= 0;
		p->circles[i].vy		= 0;
		p->circles[i].onGround	= 1;

		NeuralNetwork nn;
		nn_init(&nn, 4, 6);
		p->circles[i].brain		= nn;
	}

	p->generation = 0;
}

int main(void)
{
    
	Population p;
	// Population, Size, StartX, StartY
	p_init(&p, 50, 80, HEIGHT - 120);

    InitWindow(WIDHT, HEIGHT, "Dinosaur game in C using NN");
	SetTargetFPS(60);

	Rectangle floor = { 0, HEIGHT - 80, WIDHT, 80 };

    while (!WindowShouldClose())
    {
		if (GetRandomValue(0, 100) < 3)
		{
			int idx = GetRandomValue(0, p.size - 1);
			if (p.circles[idx].onGround)
			{
				c_jump(&p.circles[idx]);
				p.circles[idx].onGround = 0;
			}
		}
		p_update(&p, floor);

        BeginDrawing();
			DrawFPS(40, 40);
			
            ClearBackground(RAYWHITE);
			DrawRectangleRec(floor, BLACK);

			p_draw(&p);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
