//
//  control.c
//  Swarm
//
//  Created by Milan Toth on 2018. 09. 08..
//  Copyright © 2018. milgra. All rights reserved.
//

#ifndef control_h
    #define control_h

    #include "voxel.c"
    #include <sys/time.h>

typedef struct _control_t
{

    voxel_t* vox_base;
    voxel_t* vox_rect;

    v3_t lasttouch;
    v3_t starttouch;

    struct timeval lasttouchtime;

} control_t;

void control_init(void);
void control_free(void);
void control_setsize(float x, float y);
void control_updateselection(v3_t pointA, v3_t pointB);
v2_t control_touchdown(void* data, m4_t matrix);
void control_touchmove(void* data, m4_t matrix);
v4_t control_touchup(void* data, m4_t matrix);

extern control_t control;

#endif

#if __INCLUDE_LEVEL__ == 0

    #include "defaults.c"
    #include <float.h>

control_t control;

void control_free()
{
}

void control_createselection()
{

    uint32_t color = defaults.selectcolor;

    voxel_t* selection = HEAP(((voxel_t){
	.model = {0.0, 0.0, 30.0, 0.0, 0.0, 5.0},

	.colors = {color, color, color, color, color, color, color, color}}));

    control.vox_rect = selection;
}

void control_updateselection(v3_t pointA, v3_t pointB)
{

    v3_t ul = pointA;
    v3_t lr = pointB;

    if (lr.x < ul.x)
    {
	lr.x = pointA.x;
	ul.x = pointB.x;
    }

    if (lr.y > ul.y)
    {
	lr.y = pointA.y;
	ul.y = pointB.y;
    }

    control.vox_rect->model.x = ul.x;
    control.vox_rect->model.y = ul.y;
    control.vox_rect->model.z = 30.0;
    control.vox_rect->model.w = lr.x - ul.x;
    control.vox_rect->model.h = -(lr.y - ul.y);
}

void control_createcontrol()
{

    voxel_t* voxel = HEAP(((voxel_t){
	.model = {0.0, 0.0, defaults.grid_size / 2.0, 1000.0, 1000.0, defaults.grid_size},

	.colors = {0x00FF0055, 0x00FF0055, 0x00FF0055, 0x00FF0055, 0x00FF0055, 0x00FF0055, 0x00FF0055, 0x00FF0055}}));

    control.vox_base = voxel;

    voxel_update(control.vox_base);
}

void control_setsize(float x, float y)
{

    control.vox_base->model.x = -100.0;
    control.vox_base->model.y = 100.0;
    control.vox_base->model.w = x + 200.0;
    control.vox_base->model.h = y + 150.0;
}

void control_init()
{

    control_createcontrol();
    control_createselection();
}

v3_t control_getattack(v2_t scrnpt, m4_t matrix)
{

    v4_t front = v4_init(scrnpt.x, defaults.display_size.y - scrnpt.y, -1.0, 0.0);
    v4_t back  = v4_init(scrnpt.x, defaults.display_size.y - scrnpt.y, 0.0, 0.0);

    v3_t frontprj = m4_screen_to_world_coords(matrix, front, defaults.display_size.x, defaults.display_size.y);
    v3_t backprj  = m4_screen_to_world_coords(matrix, back, defaults.display_size.x, defaults.display_size.y);

    v3_t is = voxel_intersect(control.vox_base, frontprj, backprj, "03");

    return is;
}

v2_t control_touchdown(void* data, m4_t matrix)
{
    v2_t  target   = {FLT_MAX, FLT_MAX};
    v2_t* position = data;
    v3_t  local    = control_getattack(*position, matrix);

    if (local.x != FLT_MAX)
    {

	control.starttouch = local;

	control_updateselection(control.starttouch, control.starttouch);

	target.x = local.x;
	target.y = -local.y;
    }

    return target;
}

void control_touchmove(void* data, m4_t matrix)
{

    v2_t* position = data;
    v3_t  local    = control_getattack(*position, matrix);
    if (local.x != FLT_MAX)
    {

	control.lasttouch = local;
	control_updateselection(control.starttouch, local);
    }
}

v4_t control_touchup(void* data, m4_t matrix)
{
    v4_t result = v4_init(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    struct timeval touchtime;
    gettimeofday(&touchtime, NULL);

    uint64_t diff = (touchtime.tv_sec - control.lasttouchtime.tv_sec) * 1000000 + (touchtime.tv_usec - control.lasttouchtime.tv_usec);

    if (diff < 200000)
    {
	result = v4_init(0.0, 0.0, control.vox_base->model.w, control.vox_base->model.h);
    }
    else if (control.lasttouch.x != 0 && control.lasttouch.y != 0)
    {
	result = v4_init(control.vox_rect->model.x, -control.vox_rect->model.y, control.vox_rect->model.w + control.vox_rect->model.x, control.vox_rect->model.h - control.vox_rect->model.y);
    }

    control_updateselection(control.starttouch, control.starttouch);

    control.lasttouch.x = 0;
    control.lasttouch.y = 0;

    gettimeofday(&control.lasttouchtime, NULL);

    return result;
}

#endif

// control voxel
// selection voxel
// touch handling
