#include "raylib.h"
#include "nn.h"
#include "obstacle.h"
#include <time.h>

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

int compare_fitness(const void *a, const void *b)
{
    Circle *c1 = (Circle*)a;
    Circle *c2 = (Circle*)b;
    
    if (c2->fitness > c1->fitness) return 1;
    if (c2->fitness < c1->fitness) return -1;
    return 0;
}

float p_get_best_fitness(Population *p)
{
    float best = 0;
    for (int i = 0; i < p->size; i++)
    {
        if (p->circles[i].fitness > best)
        {
            best = p->circles[i].fitness;
        }
    }
    return best;
}

void c_jump(Circle *c)
{
	c->vy		= -25.0;
	c->onGround = 0;
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

int p_all_dead(Population *p)
{
	int found_alive = 1;
	int i = 0;
	while (i < p->size && found_alive)
	{
		if (p->circles[i].alive)
		{
			found_alive = 0;
		}
		i++;
	}

	return found_alive;
}

void p_next_generation(Population *p)
{
	float best		= p_get_best_fitness(p);

	qsort(p->circles, p->size, sizeof(Circle), compare_fitness);

	NeuralNetwork selected[10];
	for (int i = 0; i < 10; i++)
	{
		selected[i] = nn_clone(&p->circles[i].brain);
	}

	for (int i = 0; i < p->size; i++)
	{
		nn_free(&p->circles[i].brain);
	}

    int tmp_iter  = 0;
    
    for (int i = 0; i < 10; i++)
    {
        p->circles[tmp_iter].brain = nn_clone(&selected[i]);
        p->circles[tmp_iter].position = (Vector2){ p->startX, p->startY };
        p->circles[tmp_iter].alive = 1;
        p->circles[tmp_iter].fitness = 0;
        p->circles[tmp_iter].vy = 0;
        p->circles[tmp_iter].onGround = 1;
        p->circles[tmp_iter].color = BLUE;
        tmp_iter++;
    }
    
    while (tmp_iter < p->size)
    {
        int parent = rand() % 10;
        p->circles[tmp_iter].brain = nn_clone(&selected[parent]);
        
        nn_mutate(&p->circles[tmp_iter].brain, 0.1f);
        
        p->circles[tmp_iter].position = (Vector2){ p->startX, p->startY };
        p->circles[tmp_iter].alive = 1;
        p->circles[tmp_iter].fitness = 0;
        p->circles[tmp_iter].vy = 0;
        p->circles[tmp_iter].onGround = 1;
        p->circles[tmp_iter].color = BLUE;
        tmp_iter++;
    }
    
    for (int i = 0; i < 10; i++)
    {
        nn_free(&selected[i]);
    }
    
    p->generation++;
}

void p_check_collisions(Population *p, ObstacleManager *om)
{
    for (int i = 0; i < p->size; i++)
    {
        if (p->circles[i].alive)
        {
            for (int j = 0; j < om->capacity; j++)
            {
                if (om->obstacles[j].active)
                {
                    if (CheckCollisionCircleRec(p->circles[i].position, 
                                               p->circles[i].radius, 
                                               om->obstacles[j].rect))
                    {
                        p->circles[i].alive = 0;
                        p->circles[i].color = RED;
                    }
                }
            }
        }
    }
}

void p_update(Population *p, Rectangle floor, ObstacleManager *om)
{
    for (int i = 0; i < p->size; i++)
    {
        if (p->circles[i].alive)
        {
            Obstacle *nearest = get_nearest_obstacle(om, p->circles[i].position.x);
            if (nearest != NULL)
            {
				float inputs[4] = {
					(nearest->rect.x - p->circles[i].position.x) / WIDHT,
					nearest->rect.height / 100.0f,
					p->circles[i].vy / 25.0f,
					p->circles[i].onGround ? 1.0f : 0.0f
				};

                
                float decision = nn_predict(&p->circles[i].brain, inputs);
                if (decision > 0.0 && p->circles[i].onGround)
                {
                    c_jump(&p->circles[i]);
                }
            }
            
            c_update(&p->circles[i], floor);
            
            p->circles[i].fitness += 0.1f;
        }
    }
}

void p_draw(Population *p)
{
	for (int i = 0; i < p->size; i++)
	{
		if (p->circles[i].alive)
		{
			DrawCircleV(p->circles[i].position, p->circles[i].radius, p->circles[i].color);
		}
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
	srand(time(NULL));
    
	Population p;
	// Population, Size, StartX, StartY
	p_init(&p, 50, 80, HEIGHT - 120);

	ObstacleManager om;
	obstacle_manager_init(&om);

    InitWindow(WIDHT, HEIGHT, "Dinosaur game in C using NN");
	SetTargetFPS(60);


	Rectangle floor = { 0, HEIGHT - 80, WIDHT, 80 };

    while (!WindowShouldClose())
    {
		float deltaTime = GetFrameTime();
		
		p_update(&p, floor, &om);
		obstacle_manager_update(&om, deltaTime);
		
		p_check_collisions(&p, &om); 

		if (p_all_dead(&p))
		{
			p_next_generation(&p);
			obstacle_manager_reset(&om);
		}

        BeginDrawing();
			DrawFPS(40, 40);
			
            ClearBackground(RAYWHITE);
			DrawRectangleRec(floor, BLACK);

			p_draw(&p);
			obstacle_manager_draw(&om);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
