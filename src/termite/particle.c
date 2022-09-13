/* Particle for effects */

#ifndef particle_h
#define particle_h

#include "voxel.c"
#include <stdio.h>

typedef struct _particle_t
{
    v3_t     dir;
    char     finished;
    voxel_t* model;

} particle_t;

particle_t* particle_create(voxel_t* voxel, v3_t direction);

#endif

#if __INCLUDE_LEVEL__ == 0 // if we are compiled as implementation

void particle_delete(void* pointer)
{
    particle_t* particle = pointer;

    REL(particle->model);
    particle->model = NULL;
}

particle_t* particle_create(voxel_t* voxel, v3_t direction)
{
    particle_t* particle = CAL(sizeof(particle_t), particle_delete, NULL);

    particle->dir   = direction;
    particle->model = RET(voxel);

    return particle;
}

#endif
