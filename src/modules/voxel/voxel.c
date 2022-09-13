#ifndef voxel_h
#define voxel_h

#include "floatbuffer.c"
#include "ogl.c"
#include "zc_mat3.c"
#include "zc_mat4.c"
#include "zc_util2.c"

typedef struct _vox_t vox_t;
struct _vox_t
{
    float x;
    float y;
    float z;

    float w;
    float h;
    float d;
};

typedef struct _voxel_t voxel_t;
struct _voxel_t
{
    vox_t    model;
    v3_t     points[8];
    uint32_t colors[8];
};

void voxel_update(voxel_t* voxel);
v3_t voxel_intersect(voxel_t* voxel, v3_t line_a, v3_t line_b, char* side);
void voxel_collect_points(voxel_t* voxel, floatbuffer_t* buffer);

#endif

#if __INCLUDE_LEVEL__ == 0

#include <string.h>
//    y
//    ^
//    |
//      4---5   z
//     /|  /|  ^
//    0---1 | /
//    | | | |
//    | 6-|-7
//    |/  |/
//    2---3  -> x
//
// triangle strip 2-3-0-1-7-5-6-4-2-0

void voxel_update(voxel_t* voxel)
{
    v3_t a = {voxel->model.x, voxel->model.y, voxel->model.z};
    v3_t b = {voxel->model.x + voxel->model.w, voxel->model.y, voxel->model.z};
    v3_t c = {voxel->model.x, voxel->model.y - voxel->model.h, voxel->model.z};
    v3_t d = {voxel->model.x + voxel->model.w, voxel->model.y - voxel->model.h, voxel->model.z};

    v3_t e = {voxel->model.x, voxel->model.y, voxel->model.z - voxel->model.d};
    v3_t f = {voxel->model.x + voxel->model.w, voxel->model.y, voxel->model.z - voxel->model.d};
    v3_t g = {voxel->model.x, voxel->model.y - voxel->model.h, voxel->model.z - voxel->model.d};
    v3_t h = {voxel->model.x + voxel->model.w, voxel->model.y - voxel->model.h, voxel->model.z - voxel->model.d};

    voxel->points[0] = a;
    voxel->points[1] = b;
    voxel->points[2] = c;
    voxel->points[3] = d;

    voxel->points[4] = e;
    voxel->points[5] = f;
    voxel->points[6] = g;
    voxel->points[7] = h;
}

v3_t voxel_intersect(voxel_t* voxel, v3_t line_a, v3_t line_b, char* side)
{
    voxel_update(voxel);

    v3_t result = {0};

    if (strcmp(side, "03") == 0)
    {
	result = v4_quadlineintersection(
	    *((v4_t*) &voxel->points[0]),
	    *((v4_t*) &voxel->points[1]),
	    *((v4_t*) &voxel->points[2]),
	    line_a,
	    line_b);
    }
    else if (strcmp(side, "27") == 0)
    {
	result = v4_quadlineintersection(
	    *((v4_t*) &voxel->points[2]),
	    *((v4_t*) &voxel->points[3]),
	    *((v4_t*) &voxel->points[6]),
	    line_a,
	    line_b);
    }

    return result;
}

//    y
//    ^
//    |
//      4---5   z
//     /|  /|  ^
//    0---1 | /
//    | | | |
//    | 6-|-7
//    |/  |/
//    2---3  -> x
//
// triangle strip 0-0-0-2-1-3-3-3  5-1-7-3  6-2-4-0-5-1-1-1  6-6-6-4-7-5-5-5

void voxel_collect_points(voxel_t* voxel, floatbuffer_t* buffer)
{

    // degenerate first

    floatbuffer_addvector3(buffer, voxel->points[0]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[0]));

    floatbuffer_addvector3(buffer, voxel->points[0]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[0]));

    floatbuffer_addvector3(buffer, voxel->points[0]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[0]));

    floatbuffer_addvector3(buffer, voxel->points[2]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[2]));

    floatbuffer_addvector3(buffer, voxel->points[1]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[1]));

    // degenerate

    floatbuffer_addvector3(buffer, voxel->points[3]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[3]));

    floatbuffer_addvector3(buffer, voxel->points[3]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[3]));

    floatbuffer_addvector3(buffer, voxel->points[3]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[3]));

    floatbuffer_addvector3(buffer, voxel->points[5]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[5]));

    floatbuffer_addvector3(buffer, voxel->points[1]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[1]));

    floatbuffer_addvector3(buffer, voxel->points[7]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[7]));

    floatbuffer_addvector3(buffer, voxel->points[3]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[3]));

    floatbuffer_addvector3(buffer, voxel->points[6]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[6]));

    floatbuffer_addvector3(buffer, voxel->points[2]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[2]));

    floatbuffer_addvector3(buffer, voxel->points[4]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[4]));

    floatbuffer_addvector3(buffer, voxel->points[0]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[0]));

    floatbuffer_addvector3(buffer, voxel->points[5]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[5]));

    // degenerate last

    floatbuffer_addvector3(buffer, voxel->points[1]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[1]));

    floatbuffer_addvector3(buffer, voxel->points[1]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[1]));

    //        floatbuffer_addvector3( buffer , voxel->points[1] );
    //        floatbuffer_add( buffer , ogl_color_float_from_rgbauint32( voxel->colors[1] ) );

    // degenerate first

    //        floatbuffer_addvector3( buffer , voxel->points[6] );
    //        floatbuffer_add( buffer , ogl_color_float_from_rgbauint32( voxel->colors[6] ) );

    floatbuffer_addvector3(buffer, voxel->points[6]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[6]));

    floatbuffer_addvector3(buffer, voxel->points[6]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[6]));

    floatbuffer_addvector3(buffer, voxel->points[4]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[4]));

    floatbuffer_addvector3(buffer, voxel->points[7]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[7]));

    // degenerate last

    floatbuffer_addvector3(buffer, voxel->points[5]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[5]));

    floatbuffer_addvector3(buffer, voxel->points[5]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[5]));

    floatbuffer_addvector3(buffer, voxel->points[5]);
    floatbuffer_add(buffer, ogl_color_float_from_rgbauint32(voxel->colors[5]));
}

#endif
