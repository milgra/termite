# Termite - The ultimate real-time strategy

![Termite](termite.png)

Termite is the ultimate psychedelic real-time action strategy experience. You are the leader of a termite hive with three objectives :

- collect food  
- Produce ants  
- Destroy alien colonies  
- Select groups of termits, order them to collect food or attack aliens, use the speed slider to slow down/speed up time to have more control over your ants.  

With Termite you get :

- 31 levels of intense action and mass destruction  
- Random scene generator  
- Tons of particles  
- Voxel system based minimal design

The code is also suitable for learning purposes.

### Files

ant.c - Ant antity  
control.c - Control layer  
defaults.c - Settings and defaults  
food.c - Food entity  
grid.c - World grid  
grid2d.c - Abstract grid for path finding, etc  
levels.c - Level descriptions  
main.c - SDL handling, top level control  
particle.c - Particle entity for effects  
scene.c - Scene handling
ui.c - UI handling
voxelutil.c - Voxel label generator

### Tech info

Cortex is a pure C project written in [headerless C](https://github.com/milgra/headerlessc).  
It uses reference-counted memory management ( zc_memory.c ).  
It uses modular programming with mediator - implementation groups with different functionality that don't include each other directly, they send messages to each other.  
If a module is "static" a global variable is used to store it's state, instancing is meaningless .  

### TODO
