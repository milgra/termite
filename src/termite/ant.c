
#ifndef ant_h
#define ant_h

#include "food.c"
#include "grid.c"

enum ant_state
{

    ST_SEARCH,
    ST_WALKPATH,
    ST_TOENEMY,
    ST_TOFOOD,
    ST_TOQUEEN,
    ST_FIGHT,
    ST_DEAD,
    ST_WAIT,
    ST_QUEEN

};

typedef struct _ant_t
{
    char     id[10];
    uint32_t color;

    v2_t pos; // position
    v2_t piv; // pivot point
    v2_t trg; // target point

    float angle;
    float energy;

    char selected;
    char blinking;
    char sprinkle;

    char target_lost;

    food_t*  food_wanted;
    food_t*  food_taken;
    vec_t*   path;
    block_t* block;
    voxel_t* model;

    struct _ant_t* enemy;
    struct _ant_t* queen;

    enum ant_state state;

    // queen

    char has_child;

    float mass;
    float massperchild;
} ant_t;

void   ant_step(ant_t* ant, float distance);
void   ant_turn(ant_t* ant, uint32_t ticks);
void   ant_select(ant_t* ant);
void   ant_target(ant_t* ant, float x, float y);
ant_t* ant_create(ant_t* queen, float x, float y, uint32_t color);
void   ant_cleanup(ant_t* ant);

#endif

#if __INCLUDE_LEVEL__ == 0

#include "defaults.c"
#include <stdio.h>

uint32_t col_yellow[8] =
    {
	0xFFFF00FF,
	0xFFFF00FF,
	0xFFFF00FF,
	0xFFFF00FF,
	0xFFFF00FF,
	0xFFFF00FF,
	0xFFFF00FF,
	0xFFFF00FF

};

void ant_free(void* pointer)
{
    ant_t* ant = pointer;

    if (ant->food_wanted) REL(ant->food_wanted);
    if (ant->food_taken) REL(ant->food_taken);
    if (ant->path) REL(ant->path);
    if (ant->block) REL(ant->block);
    if (ant->model) REL(ant->model);
    if (ant->enemy) REL(ant->enemy);
    if (ant->queen) REL(ant->queen);

    ant->food_wanted = NULL;
    ant->food_taken  = NULL;
    ant->path        = NULL;
    ant->block       = NULL;
    ant->model       = NULL;
    ant->enemy       = NULL;
    ant->queen       = NULL;
}

void ant_cleanup(ant_t* ant)
{
    if (ant->food_wanted) REL(ant->food_wanted);
    if (ant->food_taken) REL(ant->food_taken);
    if (ant->path) REL(ant->path);
    if (ant->block) REL(ant->block);
    if (ant->model) REL(ant->model);
    if (ant->enemy) REL(ant->enemy);
    if (ant->queen) REL(ant->queen);

    ant->food_wanted = NULL;
    ant->food_taken  = NULL;
    ant->path        = NULL;
    ant->block       = NULL;
    ant->model       = NULL;
    ant->enemy       = NULL;
    ant->queen       = NULL;
}

void ant_set_state(ant_t* ant, enum ant_state state)
{
    char* text = "";

    switch (state)
    {
	case ST_SEARCH: text = "search"; break;
	case ST_WALKPATH: text = "walkpath"; break;
	case ST_TOENEMY: text = "toenemy"; break;
	case ST_TOFOOD: text = "tofood"; break;
	case ST_TOQUEEN: text = "toqueen"; break;
	case ST_FIGHT: text = "fight"; break;
	case ST_DEAD: text = "dead"; break;
	case ST_WAIT: text = "wait"; break;
	case ST_QUEEN: text = "queen"; break;
    }

    // printf( "ant %s %zx state to %s\n" , ant->id , ( size_t ) ant , text );
    ant->state = state;
}

ant_t* ant_create(ant_t* queen, float x, float y, uint32_t color)
{
    ant_t* ant = CAL(sizeof(ant_t), ant_free, NULL);

    ant->color = color;

    ant->pos = (v2_t){x, y};
    ant->piv = (v2_t){x, y};
    ant->trg = (v2_t){x, y};

    ant->angle  = (float) (rand() % 360) * M_PI / 180.0;
    ant->energy = queen == NULL ? defaults.queen_energy : defaults.ant_energy;

    ant_set_state(ant, queen == NULL ? ST_QUEEN : ST_SEARCH);

    ant->mass         = defaults.queen_mass;
    ant->massperchild = defaults.queen_massperchild;

    sprintf(ant->id, "%i_%i", (int) (x / defaults.grid_size), (int) (y / defaults.grid_size));

    // init heap

    uint32_t c1   = color;
    uint32_t c2   = color & 0x444444FF;
    float    size = queen == NULL ? defaults.queen_size : defaults.ant_size;

    voxel_t* model = HEAP(((voxel_t){

	.model  = {x - size / 2.0, -y + size / 2.0, defaults.grid_size / 2.0 + size, size, size, size},
	.colors = {c1, c1, c1, c1, c2, c2, c2, c2}

    }));

    ant->path  = VNEW();
    ant->model = model;

    return ant;
}

void ant_dropfood(ant_t* ant)
{

    // TODO block kezeles scenebol kene tortenjen

    food_set_state(ant->food_taken, FOOD_STATE_NORMAL);

    grid_removefood(ant->food_taken);

    ant->food_taken->pos = ant->pos;

    grid_addfood(ant->food_taken);

    // release ownership

    REL(ant->food_taken);
    ant->food_taken = NULL;
}

void ant_takehit(ant_t* ant, ant_t* attacker, float hit)
{

    if (ant->state == ST_DEAD) return;

    ant->energy -= hit;
    if (hit > defaults.max_hit - 2.0) ant->sprinkle = 1;

    if (ant->state != ST_QUEEN)
    {
	// set update function to fighting if not already fighting

	if (ant->enemy == NULL)
	{
	    ant->enemy = RET(attacker);

	    ant->piv = ant->enemy->pos;
	    ant_set_state(ant, ST_FIGHT);
	}

	// drop food if we have any

	if (ant->food_taken != NULL)
	{
	    ant_dropfood(ant);
	}
    }

    // die if fate is cruel

    if (ant->energy <= 0.0)
    {
	if (ant->state == ST_QUEEN)
	{
	    // transform to enemy queen

	    ant->energy = 100.0;
	    ant->color  = attacker->color;

	    uint32_t c1 = ant->color;
	    uint32_t c2 = c1 & 0xFFFFFF44;

	    uint32_t colors[8] = {c1, c1, c1, c1, c2, c2, c2, c2};

	    memcpy(ant->model->colors, colors, sizeof(colors));
	}
	else
	{
	    ant_set_state(ant, ST_DEAD);

	    if (ant->enemy != NULL) REL(ant->enemy);
	    ant->enemy = NULL;

	    // TODO maybe move to grid/scene?

	    if (ant->block != NULL) VREM(ant->block->ants, ant);
	}
    }
}

void ant_update_queen(ant_t* ant)
{

    if (ant->mass >= ant->massperchild)
    {

	ant->mass -= ant->massperchild;
	ant->has_child = 1;
    }
}

char ant_followtarget(ant_t* ant, v2_t pos, float distance)
{

    float dx = (pos.x - ant->pos.x);
    float dy = (pos.y - ant->pos.y);

    if (fabs(dx) < defaults.dist_dec && fabs(dy) < defaults.dist_dec) return 1; // reached target

    float step = -0.4 + (float) ((float) rand() / (float) RAND_MAX) * 0.8;

    ant->angle = atan2f(dy, dx) + step;

    ant->pos.x += cosf(ant->angle) * distance;
    ant->pos.y += sinf(ant->angle) * distance;

    block_t* block = grid_getblock(ant->pos);

    if (block == NULL || block->wall == 1)
    {
	ant->pos.x -= cosf(ant->angle) * distance;
	ant->pos.y -= sinf(ant->angle) * distance;

	ant->angle += M_PI / 4;

	if (ant->food_taken != NULL)
	{
	    ant_dropfood(ant);
	}
	if (ant->enemy != NULL)
	{
	    REL(ant->enemy);
	    ant->enemy       = NULL;
	    ant->target_lost = 1;
	}

	ant_set_state(ant, ST_SEARCH);
    }
    else grid_relocate_ant(ant);

    ant->model->model.x = ant->pos.x - ant->model->model.w / 2.0;
    ant->model->model.y = -ant->pos.y + ant->model->model.h / 2.0;

    return 0;
}

void ant_update_search(ant_t* ant, float distance)
{

    ant->pos.x += cosf(ant->angle) * distance;
    ant->pos.y += sinf(ant->angle) * distance;

    ant->model->model.x = ant->pos.x - ant->model->model.w / 2.0;
    ant->model->model.y = -ant->pos.y + ant->model->model.h / 2.0;

    block_t* block = grid_getblock(ant->pos);

    if (block == NULL || block->wall == 1)
    {

	ant->pos.x -= cosf(ant->angle) * distance;
	ant->pos.y -= sinf(ant->angle) * distance;
	ant->model->model.x = ant->pos.x - ant->model->model.w / 2.0;
	ant->model->model.y = -ant->pos.y + ant->model->model.h / 2.0;
	ant->angle += M_PI / 4;
    }
    else if (block != ant->block || ant->target_lost == 1)
    {
	ant->target_lost = 0;

	grid_relocate_ant(ant);

	food_t* food = grid_searchfood(ant->pos, ant);

	if (food != NULL)
	{
	    ant_set_state(ant, ST_TOFOOD);
	    if (ant->food_wanted) REL(ant->food_wanted);
	    ant->food_wanted = RET(food);
	}
	else
	{
	    ant_t* enemy = grid_searchenemy(ant->pos, ant);
	    if (enemy != NULL)
	    {
		ant_set_state(ant, ST_TOENEMY);
		if (ant->enemy) REL(ant->enemy);
		ant->enemy = RET(enemy);
	    }
	}
    }
}

void ant_update_search_turn(ant_t* ant)
{

    float step = -0.4 + (float) ((float) rand() / (float) RAND_MAX) * 0.8;
    ant->angle += step;
}

void ant_update_movetofood(ant_t* ant, float distance)
{

    if (ant->food_wanted != NULL &&
	ant->food_wanted->state == FOOD_STATE_NORMAL)
    {
	char reached = ant_followtarget(ant, ant->food_wanted->pos, distance);

	if (reached == 1)
	{
	    food_set_state(ant->food_wanted, FOOD_STATE_TAKEN);

	    if (ant->food_taken) REL(ant->food_taken);
	    ant->food_taken = RET(ant->food_wanted);
	    REL(ant->food_wanted);
	    ant->food_wanted = NULL;

	    // TODO move to scene?

	    ant_t* queen;

	    vec_t* path = grid_searchqueen(ant, (void**) &queen);

	    if (ant->queen) REL(ant->queen);
	    ant->queen = RET(queen);

	    if (path != NULL)
	    {
		// direct walk
		if (path->length == 0)
		{
		    ant_set_state(ant, ST_TOQUEEN);
		}
		else
		{

		    vec_reset(ant->path);
		    vec_add_in_vector(ant->path, path);

		    ant_set_state(ant, ST_WALKPATH);
		}

		path = NULL;
	    }
	    else
	    {

		ant_set_state(ant, ST_TOQUEEN);
	    }
	}
    }
    else
    {
	ant_set_state(ant, ST_SEARCH);

	if (ant->food_wanted) REL(ant->food_wanted);
	ant->food_wanted = NULL;

	ant->target_lost = 1;

	if (ant->path->length > 0)
	{
	    ant_set_state(ant, ST_WALKPATH);
	}
    }
}

void ant_update_movetoenemy(ant_t* ant, float distance)
{

    if (ant->enemy != NULL)
    {
	char reached = ant_followtarget(ant, ant->enemy->pos, distance);

	if (reached == 1)
	{
	    ant->piv = ant->enemy->pos;
	    ant_set_state(ant, ST_FIGHT);
	}
    }
    else
    {
	ant_set_state(ant, ST_SEARCH);

	ant->target_lost = 1;

	if (ant->path->length > 0)
	{
	    ant_set_state(ant, ST_WALKPATH);
	}
    }
}

void ant_update_movetoqueen(ant_t* ant, float distance)
{

    if (ant->queen != NULL &&
	ant->food_taken != NULL)
    {
	ant_t* queen = ant->queen;

	char reached = ant_followtarget(ant, queen->pos, distance);

	if (reached == 1)
	{
	    queen->mass += ant->food_taken->mass;

	    food_set_state(ant->food_taken, FOOD_STATE_EATEN);

	    ant_set_state(ant, ST_SEARCH);

	    if (ant->food_taken) REL(ant->food_taken);
	    ant->food_taken = NULL;
	}
	else
	{
	    food_set_model_pos(ant->food_taken, ant->model->model.x + 5.0, ant->model->model.y + 5.0);
	}
    }
    else
    {

	if (ant->food_taken != NULL)
	{
	    food_set_state(ant->food_taken, FOOD_STATE_EATEN);
	    REL(ant->food_taken);
	    ant->food_taken = NULL;
	}

	ant_set_state(ant, ST_SEARCH);

	ant->target_lost = 1;

	if (ant->path->length > 0)
	{
	    ant_set_state(ant, ST_WALKPATH);
	}
    }
}

void ant_update_walkpath(ant_t* ant, float distance)
{
    if (ant->path->length > 0)
    {
	v2_t* target = ant->path->data[0];

	block_t* oldblock = ant->block;

	char reached = ant_followtarget(ant, *target, distance);

	if (reached == 1)
	{

	    vec_rem_at_index(ant->path, 0);

	    if (ant->path->length == 0)
	    {
		if (ant->food_taken != NULL)
		{
		    ant_set_state(ant, ST_TOQUEEN);
		}
		else
		{
		    ant_set_state(ant, ST_SEARCH);
		}
	    }
	}
	else
	{

	    // TODO should be moved to update

	    if (ant->food_taken != NULL)
	    {
		ant->food_taken->model->model.x = ant->model->model.x + 5.0;
		ant->food_taken->model->model.y = ant->model->model.y + 5.0;
	    }
	    else if (ant->block != oldblock || ant->target_lost == 1)
	    {

		ant->target_lost = 0;

		// check for food

		food_t* food = grid_searchfood(ant->pos, ant);

		if (food != NULL)
		{

		    ant_set_state(ant, ST_TOFOOD);

		    if (ant->food_wanted) REL(ant->food_wanted);
		    ant->food_wanted = RET(food);
		}
	    }
	}
    }
    else
    {

	ant_set_state(ant, ST_SEARCH);
    }
}

void ant_update_fight(ant_t* ant)
{

    if (ant->enemy != NULL &&
	ant->enemy->energy > 0.0 &&
	ant->enemy->color != ant->color)
    {
	ant->angle += 0.1;

	float nx = ant->piv.x + cosf(ant->angle) * defaults.dist_dec * 3.0;
	float ny = ant->piv.y + sinf(ant->angle) * defaults.dist_dec * 3.0;

	ant->model->model.x = nx - ant->model->model.w / 2.0;
	ant->model->model.y = -ny + ant->model->model.h / 2.0;
    }
    else
    {
	ant_set_state(ant, ST_SEARCH);

	if (ant->enemy) REL(ant->enemy);
	ant->enemy = NULL;

	ant->target_lost = 1;
    }
}

void ant_update_fight_turn(ant_t* ant)
{

    ant_takehit(ant->enemy, ant, ((float) (rand() % 100) / 100.0) * defaults.max_hit);
}

void ant_update_wait(ant_t* ant, uint32_t ticks)
{

    // ant->blinking = 1 - ant->blinking;
    if (ticks % 800 < 400)
    {
	memcpy(ant->model->colors, col_yellow, 32);
    }
    else
    {
	uint32_t colors[8] = {
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color};

	memcpy(ant->model->colors, colors, 32);
    }
}

void ant_select(ant_t* ant)
{
    if (ant->state == ST_SEARCH ||
	ant->state == ST_TOENEMY ||
	ant->state == ST_TOFOOD ||
	(ant->state == ST_WALKPATH && ant->food_taken == NULL))
    {
	memcpy(ant->model->colors, col_yellow, 32);

	ant_set_state(ant, ST_WAIT);

	ant->selected = 1;
    }
}

void ant_target(ant_t* ant, float x, float y)
{

    if (ant->selected == 1)
    {

	uint32_t colors[8] = {
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color,
	    ant->color};

	memcpy(ant->model->colors, colors, 32);

	ant->trg.x = x;
	ant->trg.y = y;

	vec_t* path = grid_findpath(ant->pos, ant->trg);

	vec_reset(ant->path);

	vec_add_in_vector(ant->path, path);

	if (ant->path->length > 0)
	{
	    ant_set_state(ant, ST_WALKPATH);
	}
	else
	{
	    ant_set_state(ant, ST_SEARCH);
	}

	ant->selected = 0;
    }
}

void ant_step(ant_t* ant, float distance)
{

    if (ant->state == ST_SEARCH) ant_update_search(ant, distance);
    else if (ant->state == ST_TOFOOD) ant_update_movetofood(ant, distance);
    else if (ant->state == ST_TOENEMY) ant_update_movetoenemy(ant, distance);
    else if (ant->state == ST_TOQUEEN) ant_update_movetoqueen(ant, distance);
    else if (ant->state == ST_WALKPATH) ant_update_walkpath(ant, distance);
    else if (ant->state == ST_FIGHT) ant_update_fight(ant);
}

void ant_turn(ant_t* ant, uint32_t ticks)
{

    if (ant->state == ST_SEARCH) ant_update_search_turn(ant);
    else if (ant->state == ST_FIGHT) ant_update_fight_turn(ant);
    else if (ant->state == ST_WAIT) ant_update_wait(ant, ticks);
    else if (ant->state == ST_QUEEN) ant_update_queen(ant);
}

#endif
