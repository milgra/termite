/* Scene handler */

#ifndef scene_h
#define scene_h

#include "zc_mat4.c"
#include "zc_vector.c"
#include <stdio.h>

enum scene_state
{
    SCENE_STATE_PAUSED,
    SCENE_STATE_NORMAL,
    SCENE_STATE_SHARDS
};

enum game_state
{
    GAME_STATE_IDLE,
    GAME_STATE_LOST,
    GAME_STATE_WON
};

typedef struct _scene_t
{
    enum scene_state state;
    enum game_state  game_state;

    vec_t* voxels;
    vec_t* clouds;
    vec_t* sparks;
    vec_t* shards;
    vec_t* litter;
    vec_t* births;

    uint32_t ticks;

    m4_t mat_proj;
    m4_t mat_pers;

    char rotate;

    float base_scale;
    float base_angle;
    float curr_angle;
    float base_trans;
    float curr_trans;

    float dist_passed;
    float dist_per_sec;
} scene_t;

void scene_init(void);
void scene_free(void);

extern scene_t scn;

#endif /* scene_h */

#if __INCLUDE_LEVEL__ == 0

#include "ant.c"
#include "bus.c"
#include "control.c"
#include "defaults.c"
#include "food.c"
#include "levels.c"
#include "particle.c"
#include "renderer.c"
#include "voxel.c"
#include <float.h>

void scene_reset(void);
void scene_onmessage(const char* name, void* data);
void scene_updateperspective(void);

scene_t scn;

void scene_init()
{
    bus_subscribe(
	"UI",
	scene_onmessage);

    bus_subscribe(
	"CTL",
	scene_onmessage);

    bus_subscribe(
	"RND",
	scene_onmessage);

    scn.sparks = VNEW();
    scn.clouds = VNEW();
    scn.shards = VNEW();
    scn.voxels = VNEW();
    scn.litter = VNEW();
    scn.births = VNEW();

    scene_reset();
    grid_init();
    control_init();
}

void scene_free()
{
    REL(scn.sparks);
    REL(scn.clouds);
    REL(scn.shards);
    REL(scn.voxels);
    REL(scn.litter);
    REL(scn.births);

    grid_free();
    control_free();
}

void scene_reset()
{
    vec_reset(
	scn.sparks);

    vec_reset(
	scn.clouds);

    vec_reset(
	scn.shards);

    vec_reset(
	scn.voxels);

    vec_reset(
	scn.litter);

    vec_reset(
	scn.births);

    scn.ticks        = 0;
    scn.base_angle   = 0.0;
    scn.curr_angle   = scn.base_angle + 0.001;
    scn.base_trans   = 0.0;
    scn.curr_trans   = 0.001;
    scn.dist_passed  = 0.0;
    scn.dist_per_sec = defaults.dist_per_sec;

    scn.rotate = 0;

    if (defaults.depth_on == 1)
    {
	scn.base_angle = -M_PI / 6;
	scn.curr_angle = scn.base_angle + 0.001;
    }
}

void scene_set_state(enum scene_state state)
{
    // printf( "scene set state %i\n" , state );

    scn.state = state;
}

void scene_set_game_state(enum game_state state)
{
    // printf( "scene set game state %i\n" , state );

    scn.game_state = state;
}

void scene_updatescale()
{
    float scale = 1.0;

    float wthratio =
	defaults.display_size.x / grid.width;

    float testheight =
	grid.height * wthratio;

    float hthratio =
	defaults.display_size.y / grid.height;

    float testwidth =
	grid.width * hthratio;

    if (testwidth <= defaults.display_size.x)
    {
	scale = hthratio;
    }
    else if (testheight <= defaults.display_size.y)
    {
	scale = wthratio;
    }

    scn.base_scale = scale * 0.9; // shrink a little to fit in space

    if (defaults.depth_on == 1)
    {
	scn.base_scale *= 0.9;
    }
}

void scene_updateperspective()
{
    // calculate plane distance from focus point with simple trigonometry

    float camera_fov_y = M_PI / 4.0;

    float camera_eye_z =
	(defaults.display_size.y / 2.0) /
	(tanf(camera_fov_y / 2.0));

    float maxlength = grid.width > grid.height ? grid.width : grid.height;

    float min = camera_eye_z - maxlength * scn.base_scale;
    float max = camera_eye_z + maxlength * scn.base_scale;

    if (min < 10.0) min = 10.0;

    m4_t pers = m4_defaultperspective(
	camera_fov_y,
	defaults.display_size.x / defaults.display_size.y,
	min,
	max);

    m4_t trans = m4_defaulttranslation(0, 0, -camera_eye_z);

    scn.mat_pers = m4_multiply(pers, trans);
}

void scene_updateprojection(int32_t delta)
{
    if (scn.rotate == 1 && delta > 0)
    {
	scn.base_angle -= ((float) delta / 6000.0);
    }

    if (scn.base_angle < -M_PI * 2)
    {
	float delta = (scn.base_angle - scn.curr_angle);
	scn.base_angle += M_PI * 2;
	scn.curr_angle = scn.base_angle - delta;
    }

    scn.curr_angle += (scn.base_angle - scn.curr_angle) / 20.0;
    scn.curr_trans += (scn.base_trans - scn.curr_trans) / 20.0;

    m4_t angle_matrix = m4_defaultrotation(scn.curr_angle, 0.0, 0.0);
    m4_t trans_matrix = m4_defaulttranslation(-grid.width / 2.0, grid.height / 2.0 + scn.curr_trans, 0.0);
    m4_t scale_matrix = m4_defaultscale(scn.base_scale, scn.base_scale, scn.base_scale);

    if (defaults.current_level == 32) angle_matrix = m4_defaultrotation(scn.curr_angle, scn.curr_angle, 0.0);

    scn.mat_proj = m4_multiply(scn.mat_pers, angle_matrix);
    scn.mat_proj = m4_multiply(scn.mat_proj, scale_matrix);
    scn.mat_proj = m4_multiply(scn.mat_proj, trans_matrix);
}

// entity handling ( create/update )

void scene_convert_voxels_to_shards(vec_t* voxels)
{
    for (int index = 0;
	 index < voxels->length;
	 index++)
    {

	voxel_t* voxel = voxels->data[index];

	v3_t direction = {-1.0 + (float) (rand() % 200) / 100.0, -1.0 + (float) (rand() % 200) / 100.0, -1.0 + (float) (rand() % 200) / 100.0};

	particle_t* shard = particle_create(voxel, direction);

	VADD(scn.voxels, voxel);
	VADD(scn.shards, shard);

	REL(shard);
    }
}

void scene_createshards()
{
    vec_t* temp = VNEW();
    vec_add_in_vector(temp, scn.voxels);
    vec_reset(scn.voxels);

    scene_convert_voxels_to_shards(temp);
    scene_convert_voxels_to_shards(grid.voxels);
    scene_convert_voxels_to_shards(grid.tilevoxels);
}

void scene_step_shards(int32_t delta)
{
    float ratio = (float) delta / 50.0;

    for (int index = 0;
	 index < scn.shards->length;
	 index++)
    {

	particle_t* shard = scn.shards->data[index];

	shard->model->model.x += shard->dir.x * ratio;
	shard->model->model.y += shard->dir.y * ratio;
	shard->model->model.z += shard->dir.z * ratio;
    }
}

void scene_create_spark(ant_t* ant)
{
    v3_t direction = {-0.5 + (float) (rand() % 100) / 100.0, -0.5 + (float) (rand() % 100) / 100.0, 4.0 + (float) (rand() % 20) / 10.0};

    uint32_t color    = ant->color;
    uint32_t btmcolor = ant->color & 0xFFFFFF44;

    voxel_t* model = HEAP(((voxel_t){
	.model = {ant->pos.x, -ant->pos.y, defaults.grid_size / 2.0 + 1.0, 4.0, 4.0, 4.0},

	.colors = {color, color, color, color, btmcolor, btmcolor, btmcolor, btmcolor}}));

    particle_t* spark = particle_create(model, direction);

    VADD(scn.sparks, spark);
    VADD(scn.voxels, spark->model);

    REL(spark);
    REL(model);
}

void scene_update_sparks(float distance)
{
    for (int index = 0;
	 index < scn.sparks->length;
	 index++)
    {
	particle_t* spark = scn.sparks->data[index];

	spark->model->model.x += spark->dir.x;
	spark->model->model.y -= spark->dir.y;
	spark->model->model.z += spark->dir.z;

	if (spark->model->model.z < defaults.grid_size / 2.0)
	{
	    spark->dir.z *= -0.8;
	    if (spark->dir.z < 0.2) spark->finished = 1;
	}
	else spark->dir.z -= 0.2;

	if (spark->finished == 1) VADD(scn.litter, spark);
    }

    if (scn.litter->length > 0)
    {
	for (int index = 0;
	     index < scn.litter->length;
	     index++)
	{
	    particle_t* spark = scn.litter->data[index];

	    VREM(scn.sparks, spark);
	    VREM(scn.voxels, spark->model);
	}

	vec_reset(scn.litter);
    }
}

void scene_createclouds()
{
    uint32_t color = defaults.cloudcolor;

    for (int index = 0; index < 20; index++)
    {

	voxel_t* model = HEAP(((voxel_t){
	    .model  = {(float) (rand() % 1500), 450.0 - (float) (rand() % 1500), -50.0 - index, 200 + (float) (rand() % 500), 50 + (float) (rand() % 150), 50.0},
	    .colors = {[0 ... 7] = color}}));

	v3_t direction = {1.0 + (float) (rand() % 10) / 10.0, 0.0, 0.0};

	particle_t* cloud = particle_create(model, direction);

	VADD(scn.clouds, cloud);
	VADD(scn.voxels, cloud->model);

	REL(cloud);
	REL(model);
    }
}

void scene_update_clouds(float distance)
{
    for (int index = 0;
	 index < scn.clouds->length;
	 index++)
    {
	particle_t* cloud = scn.clouds->data[index];

	cloud->model->model.x -= cloud->dir.x * distance;

	if (cloud->model->model.x + cloud->model->model.w < -800.0)
	{
	    cloud->model->model.y = 450.0 - (float) (rand() % 1500);
	    cloud->model->model.x = 2000.0;
	}
    }
}

void scene_create_ant(ant_t* ant)
{
    ant_t* newant = ant_create(ant, ant->pos.x, ant->pos.y, ant->color);

    VADD(scn.births, newant);

    REL(newant);
}

void scene_update_ants(float dist_per_iter, char turn_after_step, uint32_t ticks)
{
    char game_won  = 1;
    char game_lost = 1;

    for (int index = 0;
	 index < grid.ants->length;
	 index++)
    {
	ant_t* ant = grid.ants->data[index];

	ant_step(ant, dist_per_iter);

	if (turn_after_step == 1)
	{
	    ant_turn(ant, ticks);
	}

	if (ant->has_child == 1)
	{
	    scene_create_ant(ant);
	    ant->has_child = 0;
	}

	if (ant->sprinkle == 1)
	{
	    scene_create_spark(ant);
	    ant->sprinkle = 0;
	}

	if (ant->state == ST_DEAD)
	{
	    VADD(scn.litter, ant);
	}

	if (ant->color == defaults.playercolor)
	{
	    if (ant->state == ST_QUEEN) game_lost = 0; // not lost til our queen lives
	}
	else game_won = 0; // no winning til alien ants exist
    }

    // add born ants

    if (scn.births->length > 0)
    {
	for (int index = 0;
	     index < scn.births->length;
	     index++)
	{
	    ant_t* newant = scn.births->data[index];
	    grid_addant(newant);
	}

	vec_reset(scn.births);
    }

    // remove dead ants

    if (scn.litter->length > 0)
    {
	for (int index = 0;
	     index < scn.litter->length;
	     index++)
	{
	    ant_t* ant = scn.litter->data[index];
	    grid_removeant(ant);
	}

	vec_reset(scn.litter);
    }

    // check if we won

    if (scn.game_state == GAME_STATE_IDLE &&
	game_won == 1)
    {
	scn.base_trans = 0.0;
	scene_set_game_state(GAME_STATE_WON);
	bus_notify("SCN", "WON", NULL);
	if (defaults.depth_on == 1) scn.rotate = 1;
    }

    // check if we lost

    if (scn.game_state == GAME_STATE_IDLE &&
	game_lost == 1)
    {
	scn.base_trans = 0.0;
	scene_set_game_state(GAME_STATE_LOST);
	scene_set_state(SCENE_STATE_SHARDS);
	scene_createshards();
	bus_notify("SCN", "LOST", NULL);
    }
}

void scene_update_food()
{
    for (int index = 0;
	 index < grid.food->length;
	 index++)
    {
	food_t* food = grid.food->data[index];
	if (food->state == FOOD_STATE_EATEN)
	{
	    VADD(scn.litter, food);
	}
    }

    if (scn.litter->length > 0)
    {
	for (int index = 0;
	     index < scn.litter->length;
	     index++)
	{

	    food_t* food = scn.litter->data[index];
	    grid_removefood(food);
	}

	vec_reset(scn.litter);
    }
}

void scene_step_normal(float dist_to_step)
{
    do
    {
	char turn_after_step = 0;

	float dist_per_iter = dist_to_step;
	float dist_til_dec  = defaults.dist_dec - scn.dist_passed;

	if (dist_per_iter > dist_til_dec) // check if iteration step is superior to remaining distance to decision
	{
	    dist_per_iter   = dist_til_dec; // modify iteration step
	    turn_after_step = 1;            // indicate that this is a decision step
	    scn.dist_passed = 0.0;          // reset dist_passed counter
	}
	else
	{
	    scn.dist_passed += dist_per_iter;
	}

	dist_to_step -= dist_per_iter; // decrease dist_to_step with iteration step

	scene_update_ants(dist_per_iter, turn_after_step, scn.ticks);
	scene_update_food();
	scene_update_sparks(dist_per_iter);
	scene_update_clouds(dist_per_iter);

    } while (dist_to_step > 0.0);
}

void scene_upload(int32_t delta)
{
    if (fabs(scn.curr_angle - scn.base_angle) > 0.0001 ||
	fabs(scn.curr_trans - scn.base_trans) > 0.0001)
    {
	scene_updateprojection(delta);
    }

    renderdata_t data =
	{
	    .voxels     = grid.voxels,
	    .projection = scn.mat_proj,
	    .level      = 1,
	};

    bus_notify("SCN", "UPLOADVOXELS", &data);

    data.voxels = scn.voxels;
    data.level  = 2;

    bus_notify("SCN", "UPLOADVOXELS", &data);

    data.voxels = grid.tilevoxels;
    data.level  = 0;

    bus_notify("SCN", "UPLOADVOXELS", &data);
}

// message commands

void scene_update(void* data)
{
    uint32_t ticks = *(uint32_t*) data;

    // avoid first iteration ( ticks == 0 ) or type overflow

    if (scn.ticks > 0 &&
	scn.ticks < ticks)
    {
	int32_t delta        = ticks - scn.ticks;
	float   dist_to_step = ((float) delta / 1000.0) * scn.dist_per_sec;

	if (scn.state == SCENE_STATE_NORMAL) scene_step_normal(dist_to_step);
	else if (scn.state == SCENE_STATE_SHARDS) scene_step_shards(delta);

	scene_upload(delta);
    }

    scn.ticks = ticks;
}

void scene_touchdown(void* data)
{
    v2_t target = control_touchdown(data, scn.mat_proj);

    if (target.x != FLT_MAX)
    {

	for (int index = 0; index < grid.ants->length; index++)
	{
	    ant_t* ant = grid.ants->data[index];
	    ant_target(ant, target.x, target.y);
	}
    }
}

void scene_touchup(void* data)
{
    v4_t area = control_touchup(data, scn.mat_proj);

    if (area.x != FLT_MAX)
    {

	for (int index = 0;
	     index < grid.ants->length;
	     index++)
	{
	    ant_t* ant = grid.ants->data[index];

	    if (ant->color == defaults.playercolor)
	    {
		if (ant->pos.x > area.x &&
		    ant->pos.x < area.z &&
		    ant->pos.y > area.y &&
		    ant->pos.y < area.w)
		{
		    ant_select(ant);
		}
	    }
	}
    }
}

void scene_load(void* data)
{
    int level = *((int*) data);

    grid_reset();
    scene_reset();

    levels_load(level);
    scene_updatescale();
    scene_updateperspective();
    control_setsize(grid.width, grid.height);

    scene_createclouds();

    scene_set_state(SCENE_STATE_PAUSED);
    scene_set_game_state(GAME_STATE_IDLE);

    if (level == 0 || level == 32)
    {
	scene_set_state(SCENE_STATE_NORMAL);
	scene_set_game_state(GAME_STATE_WON);
	if (defaults.depth_on == 1) scn.rotate = 1;
    }

    VADD(scn.voxels, control.vox_rect);
    // mtvec_add( scn.voxels , control.vox_base ); good for touch debugging
}

void scene_start()
{
    scn.base_trans = 20.0;
    if (defaults.depth_on == 1) scn.base_trans = 120.0;

    scene_set_state(SCENE_STATE_NORMAL);
}

void scene_set_speed(void* data)
{
    float speed      = *(float*) data;
    scn.dist_per_sec = speed * 400.0;
}

void scene_resize()
{
    scene_updatescale();
    scene_updateperspective();
    scene_updateprojection(0);
}

void scene_switchpers()
{
    scn.base_angle = defaults.depth_on == 1 ? -M_PI / 6 : 0;
    scn.rotate     = defaults.depth_on == 1 ? 1 : 0;
}

void scene_reset_time()
{
    scn.ticks = 0;
}

void scene_onmessage(const char* name, void* data)
{
    if (strcmp(name, "UPDATE") == 0) scene_update(data);
    else if (strcmp(name, "TOUCHDOWN") == 0) scene_touchdown(data);
    else if (strcmp(name, "TOUCHMOVE") == 0) control_touchmove(data, scn.mat_proj);
    else if (strcmp(name, "TOUCHUP") == 0) scene_touchup(data);
    else if (strcmp(name, "LOAD") == 0) scene_load(data);
    else if (strcmp(name, "START") == 0) scene_start();
    else if (strcmp(name, "SETSPEED") == 0) scene_set_speed(data);
    else if (strcmp(name, "RESIZE") == 0) scene_resize();
    else if (strcmp(name, "2D/3D") == 0) scene_switchpers();
    else if (strcmp(name, "RESETTIME") == 0) scene_reset_time();
}

#endif
