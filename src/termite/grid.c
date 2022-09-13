/* World grid */

#ifndef grid_h
#define grid_h

#include "grid2d.c"
#include "zc_map.c"
#include "zc_vector.c"
#include <stdio.h>

typedef struct _block_t
{
    char   wall;
    vec_t* ants;
    vec_t* food;

} block_t;

typedef struct _grid_t
{
    uint32_t rows;
    uint32_t cols;

    float width;
    float height;

    // TODO scene hasznalja foleg, at kene gondolni

    vec_t* ants;
    vec_t* food;
    vec_t* voxels;
    vec_t* tilevoxels;

    vec_t* blocks;
    vec_t* pathnodes;
    map_t* pathcache;
} grid_t;

block_t* block_alloc(void);

void     grid_init(void);
void     grid_free(void);
void     grid_reset(void);
void     grid_resetnodes(void);
block_t* grid_getblock(v2_t pos);
vec_t*   grid_findpath(v2_t s, v2_t e);
void*    grid_searchqueen(void* antp, void** queen);
void*    grid_searchfood(v2_t pos, void* ant);
void*    grid_searchenemy(v2_t pos, void* ant);
void     grid_showcontent(void);
void     grid_addant(void* data);
void     grid_removeant(void* data);
void     grid_relocate_ant(void* data);
void     grid_addfood(void* data);
void     grid_removefood(void* data);

extern grid_t grid;

#endif

#if __INCLUDE_LEVEL__ == 0

#include "ant.c"
#include "defaults.c"
#include "food.c"

void block_dealloc(void* data)
{
    block_t* block = data;

    vec_reset(block->ants);
    vec_reset(block->food);

    REL(block->ants);
    REL(block->food);

    block->ants = NULL;
    block->food = NULL;
}

block_t* block_alloc()
{
    block_t* block = CAL(sizeof(block_t), block_dealloc, NULL);

    block->ants = VNEW();
    block->food = VNEW();

    return block;
}

grid_t grid;

void grid_init()
{
    grid.rows = 0;
    grid.cols = 0;

    grid.ants       = VNEW();
    grid.food       = VNEW();
    grid.voxels     = VNEW();
    grid.tilevoxels = VNEW();

    grid.blocks    = VNEW();
    grid.pathnodes = VNEW();
    grid.pathcache = MNEW();
}

void grid_free()
{
}

void grid_reset()
{
    for (int index = 0;
	 index < grid.ants->length;
	 index++)
    {

	ant_t* ant = grid.ants->data[index];
	ant_cleanup(ant);
    }

    vec_reset(grid.ants);
    vec_reset(grid.food);
    vec_reset(grid.voxels);
    vec_reset(grid.tilevoxels);

    vec_reset(grid.blocks);
    vec_reset(grid.pathnodes);
    map_reset(grid.pathcache);

    grid.rows = 0;
    grid.cols = 0;

    grid.width  = 0;
    grid.height = 0;
}

void grid_resetnodes()
{
    for (int index = 0;
	 index < grid.pathnodes->length;
	 index++)
    {

	grid2d_node_t* node = grid.pathnodes->data[index];
	node->closed        = node->wall;
	node->opened        = 0;
	node->parent        = NULL;
	node->f             = 0.0;
	node->g             = 0.0;
	node->h             = 0.0;
    }
}

block_t* grid_getblock(v2_t pos)
{
    float col = pos.x / defaults.grid_size;
    float row = pos.y / defaults.grid_size;

    if (col < 0 ||
	row < 0 ||
	col >= grid.cols ||
	row >= grid.rows)
    {
	return NULL;
    }

    int index = (int) row * grid.cols + (int) col;
    return grid.blocks->data[index];
}

void grid_addant(void* data)
{
    ant_t*   ant   = data;
    block_t* block = grid_getblock(ant->pos);

    if (block != NULL && block->wall == 0)
    {
	VADD(block->ants, ant);

	if (vec_index_of_data(grid.ants, ant) == UINT32_MAX)
	{
	    VADD(grid.ants, ant);
	    VADD(grid.voxels, ant->model);
	}
	else printf("duplicate ant in grid.ants %s !!!\n", ant->id);
    }
}

void grid_removeant(void* data)
{
    ant_t*   ant   = data;
    block_t* block = grid_getblock(ant->pos);

    if (block != NULL)
    {
	VREM(block->ants, ant);
    }

    VREM(grid.voxels, ant->model);

    char success = VREM(grid.ants, ant);

    if (success == 0) printf("ant was non existing in grid.ants %s !!!\n", ant->id);
}

void grid_relocate_ant(void* data)
{
    ant_t*   ant   = data;
    block_t* block = grid_getblock(ant->pos);

    if (block != ant->block)
    {

	if (ant->block != NULL) VREM(ant->block->ants, ant);

	VADD(block->ants, ant);

	if (ant->block) REL(ant->block);
	ant->block = RET(block);
    }
}

void grid_addfood(void* data)
{
    food_t*  food  = data;
    block_t* block = grid_getblock(food->pos);

    if (block != NULL && block->wall == 0)
    {
	VADD(block->food, food);

	if (vec_index_of_data(grid.food, food) == UINT32_MAX)
	{

	    VADD(grid.food, food);
	    VADD(grid.voxels, food->model);
	}
	else printf("duplicate food in grid.food %s !!!\n", food->id);
    }
}

void grid_removefood(void* data)
{
    // printf( "food removed %zx\n" , ( size_t ) data );

    food_t*  food  = data;
    block_t* block = grid_getblock(food->pos);

    if (block != NULL && block->wall == 0)
    {

	char success = VREM(block->food, food);

	if (success == 0) printf("food was non existing in block->food %s !!!\n", food->id);

	VREM(grid.voxels, food->model);

	success = VREM(grid.food, food);

	if (success == 0) printf("food was non existing in grid.food %s !!!\n", food->id);
    }
}

vec_t* grid_findpath(v2_t s, v2_t e)
{
    grid_resetnodes();

    int startX = (int) (s.x / defaults.grid_size);
    int startY = (int) (s.y / defaults.grid_size);

    int endX = (int) (e.x / defaults.grid_size);
    int endY = (int) (e.y / defaults.grid_size);

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX >= grid.cols) endX = grid.cols - 1;
    if (endY >= grid.rows) endY = grid.rows - 1;

    int start = startY * grid.cols + startX;
    int end   = endY * grid.cols + endX;

    // check path in cache

    char pathid[10] = {0};

    snprintf(pathid, 10, "%x_%x", start, end);

    vec_t* result = MGET(grid.pathcache, pathid);

    if (result == NULL)
    {
	// create result path

	result = VNEW();

	// grid_showcontent();

	// check direct walk

	char haswall = grid2d_has_wall_inbetween(grid.pathnodes, startX, startY, endX, endY, grid.cols, grid.rows);

	if (haswall == 1)
	{
	    vec_t* path = VNEW();

	    grid2d_find(
		grid.pathnodes,
		path,
		grid.cols,
		grid.rows,
		start,
		end);

	    if (path->length > 0)
	    {
		for (int index = 1;
		     index < path->length - 1;
		     index++)
		{

		    grid2d_node_t* node = path->data[index];

		    v2_t* point = HEAP(((v2_t){

			node->col * defaults.grid_size + defaults.grid_size / 2.0,
			node->row * defaults.grid_size + defaults.grid_size / 2.0

		    }));

		    VADD(result, point);

		    REL(point);
		}

		v2_t* endpoint = HEAP(((v2_t){e.x, e.y}));
		VADD(result, endpoint);

		REL(endpoint);
	    }

	    REL(path);
	}
	else
	{
	    v2_t* endpoint = HEAP(((v2_t){e.x, e.y}));
	    VADD(result, endpoint);

	    REL(endpoint);
	}

	MPUT(grid.pathcache, pathid, result);
    }

    return result;
}

void* grid_searchqueen(void* antp, void** queen)
{
    ant_t* ant = antp;

    vec_t* finalpath  = NULL;
    ant_t* finalqueen = NULL;
    float  dist       = 10000.0;

    for (int index = 0;
	 index < grid.ants->length;
	 index++)
    {
	ant_t* onequeen = grid.ants->data[index];

	if (onequeen->color == ant->color &&
	    onequeen->state == ST_QUEEN)
	{
	    vec_t* path = grid_findpath(ant->pos, onequeen->pos);

	    float newdist = fabs(onequeen->pos.x - ant->pos.x) + fabs(onequeen->pos.y - ant->pos.y);

	    if (finalpath == NULL ||
		path->length < finalpath->length ||
		newdist < dist)
	    {

		dist = newdist;

		finalpath  = path;
		finalqueen = onequeen;
	    }
	}
    }

    *queen = finalqueen;

    return finalpath;
}

void* grid_searchenemy(v2_t pos, void* antp)
{
    int col = (int) (pos.x / defaults.grid_size);
    int row = (int) (pos.y / defaults.grid_size);

    // first own block

    ant_t* ant  = (ant_t*) antp;
    ant_t* pick = NULL;
    float  dist = 1000.0;

    for (int count = 0;
	 count < 8;
	 count++)
    {

	int c = -1;
	int r = -1;

	if (count == 0)
	{
	    r = row;
	    c = col - 1;
	} // W
	if (count == 1)
	{
	    r = row - 1;
	    c = col - 1;
	} // NW
	if (count == 2)
	{
	    r = row - 1;
	    c = col;
	} // N
	if (count == 3)
	{
	    r = row - 1;
	    c = col + 1;
	} // NE
	if (count == 4)
	{
	    r = row;
	    c = col + 1;
	} // E
	if (count == 5)
	{
	    r = row + 1;
	    c = col + 1;
	} // SE
	if (count == 6)
	{
	    r = row + 1;
	    c = col;
	} // S
	if (count == 7)
	{
	    r = row + 1;
	    c = col - 1;
	} // SW

	if (c > -1 &&
	    c < grid.cols &&
	    r > -1 &&
	    r < grid.rows)
	{

	    int index = r * grid.cols + c;

	    block_t* block = grid.blocks->data[index];

	    for (int index = 0;
		 index < block->ants->length;
		 index++)
	    {

		ant_t* other = block->ants->data[index];

		if (other->color != ant->color &&
		    other->energy >= 0.0)
		{

		    float dx = fabs(other->pos.x - ant->pos.x);
		    float dy = fabs(other->pos.y - ant->pos.y);

		    if (dx + dy < dist)
		    {

			pick = other;
			dist = dx + dy;
		    }
		}
	    }
	}
    }

    return pick;
}

void* grid_searchfood(v2_t pos, void* antp)
{
    int col = (int) (pos.x / defaults.grid_size);
    int row = (int) (pos.y / defaults.grid_size);

    ant_t*  ant  = (ant_t*) antp;
    food_t* pick = NULL;
    float   dist = 1000.0;

    for (int count = 0;
	 count < 8;
	 count++)
    {

	int c = -1;
	int r = -1;

	if (count == 0)
	{
	    r = row;
	    c = col - 1;
	} // W
	if (count == 1)
	{
	    r = row - 1;
	    c = col - 1;
	} // NW
	if (count == 2)
	{
	    r = row - 1;
	    c = col;
	} // N
	if (count == 3)
	{
	    r = row - 1;
	    c = col + 1;
	} // NE
	if (count == 4)
	{
	    r = row;
	    c = col + 1;
	} // E
	if (count == 5)
	{
	    r = row + 1;
	    c = col + 1;
	} // SE
	if (count == 6)
	{
	    r = row + 1;
	    c = col;
	} // S
	if (count == 7)
	{
	    r = row + 1;
	    c = col - 1;
	} // SW

	if (c > -1 &&
	    c < grid.cols &&
	    r > -1 &&
	    r < grid.rows)
	{

	    int index = r * grid.cols + c;

	    block_t* block = grid.blocks->data[index];

	    for (int index = 0;
		 index < block->food->length;
		 index++)
	    {

		food_t* food = block->food->data[index];

		if (food->state == FOOD_STATE_NORMAL)
		{

		    float dx = fabs(food->pos.x - ant->pos.x);
		    float dy = fabs(food->pos.y - ant->pos.y);

		    if (dx + dy < dist)
		    {

			pick = food;
			dist = dx + dy;
		    }
		}
	    }
	}
    }

    return pick;
}

void grid_showcontent()
{
    for (int r = 0;
	 r < grid.rows;
	 r++)
    {

	for (int c = 0;
	     c < grid.cols;
	     c++)
	{

	    int index = r * grid.cols + c;

	    block_t* block = grid.blocks->data[index];

	    if (block->wall == 1) printf("w");
	    else if (block->food->length > 0) printf("f");
	    else if (block->ants->length > 0) printf("a");
	    else printf(" ");

	    // printf( "%i-%i f:%i a:%i " , r , c , block->food->length , block->ants->length );
	}

	printf("\n");
    }
}

#endif
