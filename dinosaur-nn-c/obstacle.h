#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "raylib.h"

#define WIDHT 1920
#define HEIGHT 1080
#define GRAVITY 1.5

#define MAX_OBSTACLES 5
#define OBSTACLE_SPEED 8.0f
#define OBSTACLE_WIDTH 30.0f
#define OBSTACLE_HEIGHT 60.0f
#define OBSTACLE_SPAWN_X (WIDHT + 50) 
#define OBSTACLE_SPAWN_Y (HEIGHT - 80 - OBSTACLE_HEIGHT)  

typedef struct {
	Rectangle	rect;
	float		speed;
	int			active;
	Color		color;
} Obstacle;

typedef struct {
    Obstacle obstacles[MAX_OBSTACLES];
    int capacity;
    int count;  
    
    float spawnTimer;  
    float spawnInterval;
} ObstacleManager;


void	obstacle_init(Obstacle *o);
void	obstacle_update(Obstacle *o);
void	obstacle_draw(Obstacle *o);

void	obstacle_manager_init(ObstacleManager *om);
void	obstacle_manager_update(ObstacleManager *om, float deltaTime);
void	obstacle_manager_draw(ObstacleManager *om);
void	obstacle_manager_reset(ObstacleManager *om);



Obstacle* get_nearest_obstacle(ObstacleManager *om, float circleX);

#endif
