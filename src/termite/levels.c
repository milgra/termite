
#ifndef levels_h
#define levels_h

void  levels_load(int level);
char* levels_random(void);

#endif

#if __INCLUDE_LEVEL__ == 0

#include "ant.c"
#include "defaults.c"
#include "food.c"
#include "scene.c"
#include "zc_memory.c"

static char* levels[40];

/* return level descriptor for given index */

char* levels_get(int index)
{
    // main menu
    levels[0] =
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \n"
	"                        \0";

    // one-to-one, 10 player ant, 5 enemy ant, no enemy queen
    levels[1] =
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"     g5   \n"
	"          \0";

    // one-to-one, players queen should produce more ants at the beginning
    levels[2] =
	"          \n"
	"     r5   \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"    g9    \n"
	"          \0";

    // one-to-one, enemys queen should produce more ants at the beginning, player should use the food fields to produce more ants
    levels[3] =
	"          \n"
	"     r5   \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"    g5    \n"
	"          \0";

    // one-to-one, enemys queen should produce less ants at the beginning, player should use the food fields around the enemy
    levels[4] =
	"          \n"
	"     g8   \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"    r4    \n"
	"          \0";

    levels[5] =
	"          \n"
	" g        \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"        r \n"
	"          \0";

    levels[6] =
	"          \n"
	"     g5   \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	" r5     b5\n"
	"          \0";

    levels[7] =
	"          \n"
	"  g2   b5 \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"  r5   y5 \n"
	"          \0";

    levels[8] =
	"          \n"
	"  v5   b5 \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"     g2   \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"  r5   y5 \n"
	"          \0";

    levels[9] =
	"     g     \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"r         v\n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"     b     \0";

    levels[10] =
	"     g1   \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	" r5    v5 \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"     b5   \0";

    levels[11] =
	"g        b\n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"r        y\0";

    levels[12] =
	"g2      f5\n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"          \n"
	"f5      r5\0";

    levels[13] =
	"          \n"
	"          \n"
	"  v    b  \n"
	"          \n"
	"y        r\n"
	"          \n"
	"          \n"
	"     g2   \n"
	"          \n"
	"          \n"
	"b        v\n"
	"          \n"
	"  r    y  \n"
	"          \n"
	"          \0";

    levels[14] =
	"g         v\n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"       v   \n"
	"           \n"
	"   b   r   \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"b         r\0";

    levels[15] =
	"     v5    \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"  g5    g5 \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"     v5    \0";

    levels[16] =
	"           \n"
	"           \n"
	"  y5 v5 b5 \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"  y5 g5 v5 \n"
	"           \n"
	"           \n"
	"           \n"
	"           \n"
	"  g5 r5 g5 \n"
	"           \n"
	"           \0";

    levels[17] =
	"            \n"
	"            \n"
	"            \n"
	"   g1       \n"
	"            \n"
	"        r2  \n"
	"            \n"
	"   v2       \n"
	"            \n"
	"        b2  \n"
	"            \n"
	"   g1       \n"
	"            \n"
	"            \n"
	"            \0";

    levels[18] =
	"           w          \n"
	"           w          \n"
	"           w          \n"
	"    f5           r5   \n"
	"           w          \n"
	"           w          \n"
	"           w          \n"
	"wwwww wwwwwwwwww wwwww\n"
	"           w          \n"
	"           w          \n"
	"           w          \n"
	"     g5         f5    \n"
	"           w          \n"
	"           w          \n"
	"           w          \0";

    levels[19] =
	"           w          \n"
	"           w f f f f f\n"
	"           w  f f f f \n"
	"    g5     w          \n"
	"           w          \n"
	"           w          \n"
	"           w          \n"
	"                      \n"
	"           w          \n"
	"           w          \n"
	"           w          \n"
	"           w    r5    \n"
	"           w          \n"
	" f f f f f w          \n"
	"  f f f f  w          \0";

    levels[20] =
	"           w          \n"
	"           w          \n"
	"           w          \n"
	"    g5     w     b5   \n"
	"           w          \n"
	"           w          \n"
	" f f f f f w f f f f f\n"
	"  f f f f  f  f f f f \n"
	"           w          \n"
	"           w          \n"
	"    y5     w     r5   \n"
	"           w          \n"
	"           w          \n"
	"           w          \n"
	"           w          \0";

    levels[21] =
	"        w       \n"
	"        w       \n"
	"        w       \n"
	" g      w     r \n"
	"        w       \n"
	"  	     w       \n"
	"        w       \n"
	"        w       \n"
	"        w       \n"
	"                \n"
	"    f          f\n"
	"  f         f   \n"
	"    f   f     f \n"
	"     f       f  \0";

    levels[22] =
	"            w         \n"
	"            w         \n"
	"            w         \n"
	"    g3            b9  \n"
	"            w         \n"
	"            w         \n"
	"            w  wwwwwww\n"
	"  wwwwwwwwwww  w      \n"
	"               w      \n"
	"wwwwww  wwwwwwww  w   \n"
	"        w         w   \n"
	"        w         w   \n"
	"   wwwwww  wwwwwwww   \n"
	"           w f f f f  \n"
	"           w  f f f f \0";

    levels[23] =
	"    w     r     w    \n"
	" g  w  f     f  w  g \n"
	"    w     w     w    \n"
	" f  w  f  w  f  w  f \n"
	"    w     w     w    \n"
	" f  w  f  w  f  w  f \n"
	"    w     w     w    \n"
	" f  w  f  w  f  w  f \n"
	"    w     w     w    \n"
	" f  w  f  w  f  w  f \n"
	"    w     w     w    \n"
	" f  w  f  w  f  w  f \n"
	"    w     w     w    \n"
	" f     f  w  f     f \n"
	"          w          \0";

    levels[24] =
	"  f   w   f   w   f  \n"
	" f f  w f   f w  f f \n"
	"b f   w   r   w   f y\n"
	"ww wwww f   f wwww ww\n"
	" f f fw   f   wf f f \n"
	"f f f w f   f w f f f\n"
	" f f  wwww wwww  f f \n"
	"                     \n"
	"                     \n"
	"                     \n"
	" f f f f f f f f f f \n"
	"                     \n"
	"                     \n"
	"           g         \n"
	"                     \0";

    levels[25] =
	"wwgwwwwwwwwwwwwwwwrww\n"
	"ww wwwwwwwwwwwwwww ww\n"
	"ww wwww f f f wwww ww\n"
	"ww  ww  wwwww  ww  ww\n"
	"wwwf ffwwwwwww f  www\n"
	"wwwww wwwwwwwww wwwww\n"
	"wwwww wwwwwwwww wwwww\n"
	"wwwww wwwwwwwww wwwww\n"
	"wwwww wwwwwwwww wwwww\n"
	"wwwww  wwwwwww  wwwww\n"
	"wwwwww  wwww   wwwwww\n"
	"wwwwwww f r fwwwwwwww\n"
	"wwwwwwwwwwwwwwwwwwwww\n"
	"wwwwwwwwwwwwwwwwwwwww\0";

    levels[26] =
	"wwgwwwwwwwwwwwwwwwgww\n"
	"ww wwwwwwwwwwwwwww ww\n"
	"ww wwww f f f wwww ww\n"
	"ww fww f     f wwf ww\n"
	"www  f         f  www\n"
	"wwwww f       f wwwww\n"
	"wwwww f       f wwwww\n"
	"wwwww f       f wwwww\n"
	"wwwww f       f wwwww\n"
	"wwwww f       f wwwww\n"
	"wwwwww f     f wwwwww\n"
	"wwwwwww f f fwwwwwwww\n"
	"wwwwwwwwwwrwwwwwwwwww\n"
	"wwwwwwwwwwwwwwwwwwwww\0";

    levels[27] =
	"      w        w     \n"
	"      w   r6   w     \n"
	" f f  w        w     \n"
	" f f  w        w     \n"
	" f f  w        w     \n"
	" f f  w        w     \n"
	" f f               g8\n"
	" f f  w        w     \n"
	" f f  w        w     \n"
	" f f  w        w     \n"
	" f f  w        w     \n"
	"      w        w     \n"
	"      w   r6   w     \n"
	"      w        w     \0";

    levels[28] =
	"  g   w   y7  w   g  \n"
	"      w       w      \n"
	"  f   w   f   w   f  \n"
	"      w       w      \n"
	"  f   w   f   w   f  \n"
	"      w       w      \n"
	"  f   w   f   w   f  \n"
	"                     \n"
	"  f   w   f   w   f  \n"
	"      w       w      \n"
	"  f   w   f   w   f  \n"
	"      w       w      \n"
	"  f   w   f   w   f  \n"
	"      w       w      \n"
	"  b7  w   g   w   v7 \n"
	"ww wwwwwwwwwwwwwwwwww\n"
	" f f f f f f f f f f \0";

    levels[29] =
	"wwwwwwwwwwgwwwwwwwwww\n"
	"wwwwwwwwww wwwwwwwwww\n"
	"wwwwwwwwww wwwwwwwwww\n"
	"wwwyfffffffffffffywww\n"
	"wwwwwwfffffffffwwwwww\n"
	"wwwwwfffffffffffwwwww\n"
	"wwwwwfffffffffffwwwww\n"
	"wwvfffffffffffffffvww\n"
	"wwwwwfffffffffffwwwww\n"
	"wwwwwfffffffffffwwwww\n"
	"wwwwwfffffffffffwwwww\n"
	"wwwwwwfffffffffwwwwww\n"
	"wwwwrfffffffffffrwwww\n"
	"wwwwwwwwww wwwwwwwwww\n"
	"wwwwwwwwww wwwwwwwwww\n"
	"wwwwwwwwwwgwwwwwwwwww\0";

    levels[30] =
	"   g wwwwwwwwwww     \n"
	"     wwww            \n"
	"     wwww    y7   www\n"
	"     wwww         www\n"
	"wwww f f f wwwwwwwwww\n"
	"wwww f f f wwwwwwwwww\n"
	"wwww f f f       wwww\n"
	"ww   wwww        wwww\n"
	"ww   wwww   v7   wwww\n"
	"ww   wwww        wwww\n"
	"ww   wwwwwwwwww f  ww\n"
	"www f f wwwwwww f  ww\n"
	"www f f wwwwwww f  ww\n"
	"www f f wwwwwww f  ww\n"
	"     wwwwww       www\n"
	"     wwwwww       www\n"
	"  b7 wwwwww  r7   www\n"
	"     wwwwww       www\0";

    levels[31] =
	" f  www   f   f  ww  \n"
	" f r  ww   r  v ww   \n"
	"y ww   ww   f  ww  r \n"
	" ww   f ww  f    f   \n"
	"ww  f b  ww  ww f b  \n"
	" ww    f  wwww    f  \n"
	"  ww  f    ww f   f  \n"
	"f f    v       f     \n"
	"r   ww   ww  f       \n"
	" y   ww f wwww   f   \n"
	"f  f  www  f wwwwf f \n"
	"  g    ww y     w www\n"
	"   f    ww   f     f \n"
	" f   r   ww    g  f  \0";

    levels[32] =
	"                                      \n"
	"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n"
	"w   w   w ww w   w   w   w   w   w w w\n"
	"w www w w  w w www w w w ww ww www w w\n"
	"w www w w    w www w w w ww ww   w w w\n"
	"w www w w w  w w w  ww   ww wwww wwwww\n"
	"w   w   w ww w   w w w w ww ww   w w w\n"
	"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n"
	"www w w   w w ww   w w w www   w w www\n"
	"www w w w w w ww w w w w www www w www\n"
	"www   w w w w ww w w w w www   w w www\n"
	"wwww ww w w w ww  ww w w www wwwwwwwww\n"
	"wwww ww   w   ww w w   w   w   w w www\n"
	"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n"
	"                                      \0";

    char* s_managed = CAL(strlen(levels[index]) + 1, NULL, NULL);
    strcpy(s_managed, levels[index]);

    return s_managed;
}

char* levels_random()
{

    int rows = 8 + rand() % 20;
    int cols = 8 + rand() % 20;

    char* result = CAL(rows * cols * sizeof(char) + sizeof(char), NULL, NULL);
    int   index  = 0;

    for (int row = 0; row < rows; row++)
    {

	for (int col = 0; col < cols - 1; col++)
	{

	    result[index++] = ' ';
	}

	result[index++] = '\n';
    }

    result[index - 1] = '\0';

    // add queen

    int row;
    int col;

    do
    {

	row = rand() % rows;
	col = rand() % cols;

    } while (result[row * cols + col] != ' ');

    result[row * cols + col] = 'g';

    // add aliens

    char* aliens = "rbvy";

    int count = 3 + rand() % 5;
    int iter  = 0;

    for (int index = 0; index < count; index++)
    {
	do
	{

	    row = rand() % rows;
	    col = rand() % cols;

	} while (result[row * cols + col] != ' ' && iter < 100);

	result[row * cols + col] = aliens[rand() % 4];
    }

    // add food

    count = 20 + rand() % (rows + cols);
    iter  = 0;

    for (int index = 0; index < count; index++)
    {
	do
	{

	    row = rand() % rows;
	    col = rand() % cols;

	} while (result[row * cols + col] != ' ' && iter < 100);

	result[row * cols + col] = 'f';
    }

    // add walls

    count = 20 + rand() % (rows + cols);
    iter  = 0;

    for (int index = 0; index < count; index++)
    {
	do
	{

	    row = rand() % rows;
	    col = rand() % cols;

	} while (result[row * cols + col] != ' ' && iter < 100);

	result[row * cols + col] = 'w';
    }

    return result;
}

char* levels_create_fitting()
{

    int rows = 20;
    int cols = (defaults.display_size.x / defaults.display_size.y) * rows;

    if (cols == 0) cols = 1;
    if (cols > 25) cols = 25;

    char* result = CAL(rows * cols * sizeof(char) + sizeof(char), NULL, NULL);
    int   index  = 0;

    for (int row = 0; row < rows; row++)
    {

	for (int col = 0; col < cols - 1; col++)
	{

	    result[index++] = ' ';
	}

	result[index++] = '\n';
    }

    result[index - 1] = '\0';

    return result;
}

void levels_add_random_ants(uint32_t color, uint32_t count)
{

    ant_t* queen = ant_create(NULL, (float) (rand() % (int) (grid.width)), (float) (rand() % (int) (grid.height)), color);

    for (int index = 0;
	 index < count;
	 index++)
    {
	ant_t* newant = ant_create(queen, (float) (rand() % (int) (grid.width)), (float) (rand() % (int) (grid.height)), color);

	grid_addant(newant);

	REL(newant);
    }

    REL(queen);
}

void levels_add_random_food_in_rect(uint32_t count, float x, float y, float width, float height)
{

    for (int index = 0;
	 index < count;
	 index++)
    {

	food_t* food = food_create(
	    x + ((float) (rand() % 100) / 100.0) * width,
	    y + ((float) (rand() % 100) / 100.0) * height,
	    0xFF00FFFF);

	grid_addfood(food);

	REL(food);
    }
}

void levels_add_random_food_in_circle(uint32_t count, float x, float y, float radius)
{

    for (int index = 0;
	 index < count;
	 index++)
    {

	float angle = ((float) (rand() % 100) / 100.0) * 6.28;

	food_t* food = food_create(
	    x + sinf(angle) * radius,
	    y + cosf(angle) * radius,
	    0xFF00FFFF);

	grid_addfood(food);

	REL(food);
    }
}

void levels_setup(int level)
{

    if (level == 0) levels_add_random_ants(defaults.playercolor, 200); // starting level, 200 self ant
    else if (level == 32) levels_add_random_ants(defaults.playercolor, 2000);
    else if (level == 1) levels_add_random_ants(0xFF0000FF, 5); // 5 alien ants
    else if (level == 3 || level == 4)                          // 30 random food on the bottom side
    {
	levels_add_random_food_in_rect(30, 0, grid.height / 2.0, grid.width, grid.height / 2.0);
    }
    else if (level == 5 || level == 11) // 500 random food
    {
	levels_add_random_food_in_rect(500, 0, 0, grid.width, grid.height);
    }
    else if (level == 6) // 30 food in the middle
    {
	levels_add_random_food_in_rect(30, grid.width / 2.0 + -20.0, grid.height / 2.0, 40.0, grid.height / 2.0);
    }
    else if (level == 7 || level == 9) // cross shaped food fields
    {
	levels_add_random_food_in_rect(100, grid.width / 2.0 - defaults.grid_size / 2.0, 0.0, 40.0, grid.height);
	levels_add_random_food_in_rect(100, 0.0, grid.height / 2.0 + -20.0, grid.width, 40.0);
    }
    else if (level == 8 || level == 13) // circular food fields
    {
	int count  = level == 13 ? 150 : 20;
	int radius = level == 13 ? 180 : 90;

	levels_add_random_food_in_circle(count, grid.width / 2.0 + defaults.grid_size / 2.0, grid.height / 2.0, radius);
    }
    else if (level == 10) // food in corners
    {
	levels_add_random_food_in_rect(50, 0.0, 0.0, defaults.grid_size, defaults.grid_size);
	levels_add_random_food_in_rect(50, grid.width - defaults.grid_size, 0.0, defaults.grid_size, defaults.grid_size);
	levels_add_random_food_in_rect(50, grid.width - defaults.grid_size, grid.height - defaults.grid_size, defaults.grid_size, defaults.grid_size);
	levels_add_random_food_in_rect(50, 0.0, grid.height - defaults.grid_size, defaults.grid_size, defaults.grid_size);
    }
    else if (level == 14) // cross shaped and wall-aligned food fields
    {
	levels_add_random_food_in_rect(50, grid.width / 2.0 + -20.0, 0.0, 40.0, grid.height);
	levels_add_random_food_in_rect(50, 0.0, grid.height / 2.0 + -20.0, grid.width, 40.0);
	levels_add_random_food_in_rect(50, 0.0, 0.0, defaults.grid_size, grid.height);
	levels_add_random_food_in_rect(50, grid.width - defaults.grid_size, 0.0, defaults.grid_size, grid.height);
	levels_add_random_food_in_rect(50, defaults.grid_size, 0.0, grid.width - 2 * defaults.grid_size, defaults.grid_size);
	levels_add_random_food_in_rect(50, defaults.grid_size, grid.height - defaults.grid_size, grid.width - 2 * defaults.grid_size, defaults.grid_size);
    }
    else if (level == 15) // two food bars
    {
	levels_add_random_food_in_rect(200, grid.width / 2.0 - 100.0, 0.0, 40.0, grid.height);
	levels_add_random_food_in_rect(200, grid.width / 2.0 + 60.0, 0.0, 40.0, grid.height);
    }
    else if (level == 16) // hash food
    {
	levels_add_random_food_in_rect(200, grid.width / 2.0 - 100.0, 0.0, 40.0, grid.height);
	levels_add_random_food_in_rect(200, grid.width / 2.0 + 60.0, 0.0, 40.0, grid.height);
	levels_add_random_food_in_rect(200, grid.width / 2.0 - 100.0, 0.0, 40.0, grid.height);
	levels_add_random_food_in_rect(200, 0.0, grid.height / 2.0 - 140.0, grid.width, 40.0);
	levels_add_random_food_in_rect(200, 0.0, grid.height / 2.0 + 100.0, grid.width, 40.0);
    }
    else if (level == 17) // 5 circles
    {
	int count  = 100;
	int radius = 140;

	levels_add_random_food_in_circle(count, -defaults.grid_size / 2.0 + defaults.grid_size * 4, -defaults.grid_size / 2.0 + defaults.grid_size * 4, radius);
	levels_add_random_food_in_circle(count, -defaults.grid_size / 2.0 + defaults.grid_size * 4, -defaults.grid_size / 2.0 + defaults.grid_size * 8, radius);
	levels_add_random_food_in_circle(count, -defaults.grid_size / 2.0 + defaults.grid_size * 4, -defaults.grid_size / 2.0 + defaults.grid_size * 12, radius);
	levels_add_random_food_in_circle(count, -defaults.grid_size / 2.0 + defaults.grid_size * 8, -defaults.grid_size / 2.0 + defaults.grid_size * 6, radius);
	levels_add_random_food_in_circle(count, -defaults.grid_size / 2.0 + defaults.grid_size * 8, -defaults.grid_size / 2.0 + defaults.grid_size * 10, radius);
    }
}

typedef struct _gblock_t
{
    char wall;
    char food;
    char queen;

    int      count;
    uint32_t color;
} gblock_t;

vec_t* levels_text_to_blocks(char* leveltext, uint32_t* cols, uint32_t* rows)
{

    vec_t* gblocks = VNEW();

    while (*leveltext != '\0')
    {

	if (*leveltext == '\n')
	{

	    *cols = 0;
	    *rows += 1;
	}
	else
	{
	    gblock_t* block = HEAP((gblock_t){0});

	    if (*leveltext == 'w')
	    {
		block->wall = 1;
	    }

	    if (*leveltext == 'f')
	    {
		block->food  = 1;
		block->count = 5;
		char n       = *(leveltext + 1);
		if (n >= '0' && n <= '9') block->count = atoi(leveltext + 1) * 2;
	    }

	    if (*leveltext == 'r' ||
		*leveltext == 'g' ||
		*leveltext == 'b' ||
		*leveltext == 'y' ||
		*leveltext == 'v')
	    {

		block->queen = 1;
		block->count = 5;

		char c = *leveltext;

		if (c == 'r') block->color = 0xFF0000FF;
		else if (c == 'g') block->color = 0x00FF00FF;
		else if (c == 'b') block->color = 0x0000FFFF;
		else if (c == 'y') block->color = 0xFFFF00FF;
		else if (c == 'v') block->color = 0xFFFFFFFF;

		char n = *(leveltext + 1);
		if (n >= '0' && n <= '9') block->count = atoi(leveltext + 1) * 2 - 1;
	    }

	    VADD(gblocks, block);
	    REL(block);

	    *cols += 1;
	}

	leveltext += 1;
    }

    *rows += 1;

    return gblocks;
}

void levels_blocks_rotate(vec_t* blocks, uint32_t* cols, uint32_t* rows)
{

    uint32_t index = blocks->length;

    for (int col = *cols - 1; col > -1; col--)
    {
	for (int row = 0; row < *rows; row++)
	{

	    gblock_t* block = blocks->data[row * *cols + col];
	    VADD(blocks, block);
	}
    }

    vec_rem_in_range(blocks, 0, index - 1);

    uint32_t newcols = *rows;
    uint32_t newrows = *cols;

    *rows = newrows;
    *cols = newcols;
}

void levels_load(int level)
{
    char* leveltext;

    if (level == -1)
    {
	leveltext = levels_random();
    }
    else if (level == 0)
    {
	leveltext = levels_create_fitting();
    }
    else
    {
	leveltext = levels_get(level);
    }

    uint32_t cols = 0;
    uint32_t rows = 0;

    vec_t* gblocks = levels_text_to_blocks(leveltext, &cols, &rows);

    REL(leveltext);

    // rotate scene if needed

    char rotate = 0;

    if (defaults.display_size.x < defaults.display_size.y)
    {
	if (cols > rows) rotate = 1;
    }
    else
    {
	if (cols < rows) rotate = 1;
    }

    if ((level == -1 || level > 17) && rotate == 1) levels_blocks_rotate(gblocks, &cols, &rows);

    // build up scene

    grid.rows = rows;
    grid.cols = cols;

    for (int row = 0; row < rows; row++)
    {
	for (int col = 0; col < cols; col++)
	{
	    gblock_t* gblock = gblocks->data[row * cols + col];

	    block_t* block = block_alloc();

	    VADD(grid.blocks, block);

	    grid2d_node_t* node = HEAP(((grid2d_node_t){
		.col = col,
		.row = row}));

	    VADD(grid.pathnodes, node);

	    node->wall  = gblock->wall;
	    block->wall = gblock->wall;

	    if (block->wall == 0)
	    {

		uint32_t color    = (col % 2 + row) % 2 ? defaults.gridcolor_a : defaults.gridcolor_b;
		uint32_t btmcolor = (col % 2 + row) % 2 ? defaults.gridcolor_c : defaults.gridcolor_d;

		voxel_t* voxel = HEAP(((voxel_t){
		    .model = {col * defaults.grid_size, -((float) row * defaults.grid_size), defaults.grid_size / 2.0, defaults.grid_size, defaults.grid_size, defaults.grid_size},

		    .colors = {color, color, color, color, btmcolor, btmcolor, btmcolor, btmcolor}}));

		VADD(grid.tilevoxels, voxel);

		REL(voxel);
	    }

	    REL(block);
	    REL(node);

	    if (gblock->food == 1)
	    {

		for (int index = 0;
		     index < gblock->count;
		     index++)
		{

		    food_t* food = food_create(
			col * defaults.grid_size + ((float) (rand() % 100) / 100.0) * defaults.grid_size,
			row * defaults.grid_size + ((float) (rand() % 100) / 100.0) * defaults.grid_size,
			0xFF00FFFF);

		    grid_addfood(food);

		    REL(food);
		}
	    }

	    if (gblock->queen == 1)
	    {

		ant_t* ant = ant_create(
		    NULL,
		    col * defaults.grid_size + defaults.grid_size / 2.0,
		    row * defaults.grid_size + defaults.grid_size / 2.0,
		    gblock->color);

		ant->mass = ant->massperchild * (float) gblock->count;

		grid_addant(ant);

		REL(ant);
	    }
	}
    }

    REL(gblocks);

    grid.width  = grid.cols * defaults.grid_size;
    grid.height = grid.rows * defaults.grid_size;

    levels_setup(level);
}

#endif
