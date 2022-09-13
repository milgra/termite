/* Voxel generator */

#ifndef voxeltext_h
#define voxeltext_h

#include "defaults.c"
#include "pixeltext.c"
#include "voxel.c"
#include "zc_vector.c"
#include <stdio.h>

void voxelutil_move(vec_t* voxels, float x, float y);

vec_t* voxelutil_button(char* label, float x, float y, float width, float height, float pixelsize, uint32_t textcolor, uint32_t backcolor);

#endif

#if __INCLUDE_LEVEL__ == 0

void voxelutil_move(vec_t* voxels, float x, float y)
{
    // moves all voxels in vector with given x and y

    voxel_t* voxel;

    for (int index = 0;
	 index < voxels->length;
	 index++)
    {

	voxel = voxels->data[index];
	voxel->model.x += x;
	voxel->model.y += y;
    }
}

void voxelutil_label(char* label, float size, uint32_t color, vec_t* voxels)
{
    // asks for pixel coordinates for label
    // generates voxels based on coordinates

    v2_t*    point;
    voxel_t* voxel;
    vec_t*   points;

    points = pixeltext_generate(label, size);

    for (int index = 0;
	 index < points->length;
	 index++)
    {

	point = points->data[index];

	voxel = HEAP(((voxel_t){
	    .model = {point->x, point->y, 15.0, size, size, 5.0},

	    .colors = {color, color, color, color, color, color, color, color}}));

	vec_add(voxels, voxel);

	REL(voxel);
    }

    REL(points);
}

void voxelutil_buttonback(float x, float y, float width, float height, uint32_t color, vec_t* voxels)
{
    // asks for size of label
    // creates a slightly bigger voxel for button surface

    uint32_t color_a;
    uint32_t color_b;

    color_a = color;
    color_b = color_a & 0xFFFFFF11;

    voxel_t* voxel = HEAP(((voxel_t){

	.model = {x, y, 10.0, width, height, 5.0},

	.colors = {color_a, color_a, color_a, color_a, color_b, color_b, color_b, color_b}

    }));

    vec_add(voxels, voxel);

    REL(voxel);
}

vec_t* voxelutil_button(char* label, float x, float y, float width, float height, float pixelsize, uint32_t textcolor, uint32_t backcolor)
{
    // generate voxels for label
    // generate voxel for background rectangle
    // move voxels to position

    vec_t* voxels;

    v2_t textsize;

    voxels = VNEW();

    // background

    textsize = pixeltext_calcsize(label, pixelsize);

    voxelutil_label(label, pixelsize, textcolor, voxels);

    voxelutil_move(voxels, width / 2.0 - textsize.x / 2.0, -(height / 2.0 - textsize.y / 2.0));

    voxelutil_buttonback(0.0, 0.0, width, height, backcolor, voxels);

    voxelutil_move(voxels, x, y);

    return voxels;
}

#endif
