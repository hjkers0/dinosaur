#include "obstacle.h"
#include <stdlib.h>


void	obstacle_manager_reset(ObstacleManager *om)
{
	obstacle_manager_init(om);
}

Obstacle* get_nearest_obstacle(ObstacleManager *om, float circleX)
{
    Obstacle *nearest = NULL;
    float minDist = 999999;
    
    for (int i = 0; i < om->capacity; i++)
    {
        if (om->obstacles[i].active && om->obstacles[i].rect.x > circleX)
        {
            float dist = om->obstacles[i].rect.x - circleX;
            if (dist < minDist)
            {
                minDist = dist;
                nearest = &om->obstacles[i];
            }
        }
    }
    return nearest;
}

void obstacle_init(Obstacle *obs)
{
	obs->rect	= (Rectangle){ OBSTACLE_SPAWN_X, 
					OBSTACLE_SPAWN_Y, 
					OBSTACLE_WIDTH, 
					OBSTACLE_HEIGHT };
	obs->speed  = OBSTACLE_SPEED;
	obs->active	= 1;
	obs->color = YELLOW;
}

void obstacle_update(Obstacle *obs)
{
	obs->rect.x -= obs->speed;
	
	if (obs->rect.x + obs->rect.width < 0)
	{
		obs->active = 0;
	}
}

void obstacle_draw(Obstacle *obs)
{
	DrawRectangleRec(obs->rect, obs->color);
}

void obstacle_manager_init(ObstacleManager *om)
{
    om->capacity = MAX_OBSTACLES;
    om->count = 0;
    om->spawnTimer = 0;
    om->spawnInterval = 2.0f;     
    for (int i = 0; i < om->capacity; i++)
    {
        om->obstacles[i].active = 0;
    }
}

void obstacle_manager_update(ObstacleManager *om, float deltaTime)
{
    for (int i = 0; i < om->capacity; i++)
    {
        if (om->obstacles[i].active)
        {
            om->obstacles[i].rect.x -= om->obstacles[i].speed;
            
            if (om->obstacles[i].rect.x + om->obstacles[i].rect.width < 0)
            {
                om->obstacles[i].active = 0;  
                om->count--;  
            }
        }
    }
    
    om->spawnTimer += deltaTime;  
    
    if (om->spawnTimer >= om->spawnInterval)
    {
        om->spawnTimer = 0;
        
        om->spawnInterval = GetRandomValue(120, 250) / 100.0f; 
        
        
        for (int i = 0; i < om->capacity; i++)
        {
            if (!om->obstacles[i].active)
            {
                
                om->obstacles[i].rect.x = WIDHT + 50;  
                om->obstacles[i].rect.y = HEIGHT - 80 - OBSTACLE_HEIGHT;  
                om->obstacles[i].rect.width = OBSTACLE_WIDTH;
                om->obstacles[i].rect.height = OBSTACLE_HEIGHT; 
                om->obstacles[i].speed = OBSTACLE_SPEED;
                om->obstacles[i].color = RED;
                om->obstacles[i].active = 1;
                
                om->count++;
                break;
            }
        }
    }
}

void	obstacle_manager_draw(ObstacleManager *om)
{
	for (int i = 0; i < om->capacity; i++)
	{
		if (om->obstacles[i].active)
		{
			obstacle_draw(&om->obstacles[i]);
		}
	}
}


