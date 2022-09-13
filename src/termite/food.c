/* Food entity */

#ifndef food_h
#define food_h

#include "voxel.c"
#include <stdio.h>

enum food_state
{
    FOOD_STATE_NORMAL,
    FOOD_STATE_TAKEN,
    FOOD_STATE_EATEN
};

typedef struct _food_t
{
    char            id[10];
    enum food_state state;
    float           mass;

    v2_t     pos;
    voxel_t* model;
} food_t;

food_t* food_create(float x, float y, uint32_t color);
void    food_set_state(food_t* food, enum food_state state);
void    food_set_model_pos(food_t* food, float x, float y);

#endif

#if __INCLUDE_LEVEL__ == 0

#include "defaults.c"

void food_dealloc(void* pointer)
{
    food_t* food = pointer;
    REL(food->model);
}

food_t* food_create(float x, float y, uint32_t color)
{
    food_t* food = CAL(sizeof(food_t), food_dealloc, NULL);

    sprintf(food->id, "%i_%i", (int) (y / defaults.grid_size), (int) (x / defaults.grid_size));

    food->pos   = (v2_t){x, y};
    food->mass  = defaults.food_mass;
    food->state = FOOD_STATE_NORMAL;

    voxel_t* model = HEAP(((voxel_t){
	.model = {x, -y, defaults.grid_size / 2.0 + defaults.ant_size, defaults.ant_size, defaults.ant_size, defaults.ant_size},

	.colors = {color, color, color, color, color, color, color, color}}));

    food->model = model;

    return food;
}

void food_set_state(food_t* food, enum food_state state)
{
    char* text = "";

    switch (food->state)
    {

	case FOOD_STATE_NORMAL: text = "normal"; break;
	case FOOD_STATE_TAKEN: text = "taken"; break;
	case FOOD_STATE_EATEN: text = "eaten"; break;
    }

    // printf( "food %zx to %s\n" , (size_t) food , text );

    food->state = state;
}

void food_set_model_pos(food_t* food, float x, float y)
{
    food->model->model.x = x;
    food->model->model.y = y;
}

#endif
