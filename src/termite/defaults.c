
#ifndef defaults_h
#define defaults_h

#include "zc_mat3.c"
#include "zc_util2.c"
#include <stdint.h>
#include <stdlib.h>

typedef struct _defaults_t
{
    char dark_on;
    char depth_on;

    int effects_level;
    int highest_level;
    int current_level;

    v3_t  backcolor;
    float ant_max_energy;

    uint32_t playercolor;
    uint32_t gridcolor_a;
    uint32_t gridcolor_b;
    uint32_t gridcolor_c;
    uint32_t gridcolor_d;
    uint32_t textcolor_a;
    uint32_t textcolor_b;
    uint32_t whitecolor;
    uint32_t cloudcolor;
    uint32_t selectcolor;
    uint32_t buttoncolor_a;
    uint32_t buttoncolor_b;

    float dist_dec;     // step distance between direction decisions for ants
    float dist_per_sec; // step distance per second

    float food_mass;
    float queen_mass;
    float queen_massperchild;

    float ant_energy;
    float queen_energy;

    float ant_size;
    float queen_size;
    float grid_size;

    float max_hit;

    // runtime

    v2_t  display_size;
    float text_scale;

    char donation_arrived; // purchased items arrived from stores
    char prices_arrived;   // prices arrived from stores
    char prices[3][100];

} defaults_t;

extern defaults_t defaults;

void defaults_init(void);
void defaults_save(void);
void defaults_setdarktheme(void);
void defaults_setbrighttheme(void);

#endif

#if __INCLUDE_LEVEL__ == 0

#include "settings.c"

defaults_t defaults = {0};

void defaults_setdarktheme()
{
    defaults.gridcolor_a   = 0x2F2F2FFF;
    defaults.gridcolor_b   = 0x1F1F1FFF;
    defaults.gridcolor_c   = 0x1F1F1FFF;
    defaults.gridcolor_d   = 0x0F0F0FFF;
    defaults.textcolor_a   = 0xFFFFFFFF;
    defaults.textcolor_b   = 0xFFFFFFFF;
    defaults.cloudcolor    = 0x0000FF0B;
    defaults.selectcolor   = 0x00FF0055;
    defaults.buttoncolor_a = 0xFFFFFF22;
    defaults.buttoncolor_b = 0xFFFFFF55;

    defaults.backcolor = (v3_t){0.0, 0.0, 0.0};
    defaults.dark_on   = 1;
}

void defaults_setbrighttheme()
{
    defaults.gridcolor_a   = 0xFFFFFFFF;
    defaults.gridcolor_b   = 0xECECECFF;
    defaults.gridcolor_c   = 0xFDFDFDFF;
    defaults.gridcolor_d   = 0xEBEBEBFF;
    defaults.textcolor_a   = 0x000000FF;
    defaults.textcolor_b   = 0xFFFFFFFF;
    defaults.cloudcolor    = 0xFFFFFF0A;
    defaults.selectcolor   = 0x00FF0022;
    defaults.buttoncolor_a = 0x00000022;
    defaults.buttoncolor_b = 0x00000055;

    defaults.backcolor = (v3_t){0.4, 0.65, 0.7};
    defaults.dark_on   = 0;
}

void defaults_init()
{

    int inited = settings_getint("initialized");

    if (inited == 0)
    {
	settings_setint("initialized", 1);
	settings_setint("donation_arrived", 0);

	defaults.highest_level    = 1;
	defaults.effects_level    = 2;
	defaults.donation_arrived = 0;

#ifdef RASPBERRY
	defaults.effects_level = 0;
#endif
    }
    else
    {

	defaults.highest_level    = settings_getint("highest_level");
	defaults.effects_level    = settings_getint("effects_level");
	defaults.donation_arrived = settings_getint("donation_arrived");
    }

    defaults.dark_on  = 1 - settings_getint("dark_on");
    defaults.depth_on = 1 - settings_getint("depth_on");

    defaults.dist_dec     = 2.0;
    defaults.dist_per_sec = 50.0; // duplicated in scene.c

    defaults.ant_size   = 8.0;
    defaults.queen_size = 11.0;
    defaults.grid_size  = 50.0;

    defaults.playercolor = 0x00FF00FF;
    defaults.whitecolor  = 0xFFFFFFFF;

    defaults.food_mass          = 50.0;
    defaults.queen_mass         = 100.0;
    defaults.queen_massperchild = 50.0;

    defaults.ant_energy   = 100.0;
    defaults.queen_energy = 2000.0;

    defaults.max_hit    = 10.0;
    defaults.text_scale = 1.0;

    if (defaults.dark_on == 1) defaults_setdarktheme();
    else defaults_setbrighttheme();

    defaults.highest_level = 22;
}

void defaults_save()
{

    settings_setint("dark_on", 1 - defaults.dark_on);
    settings_setint("depth_on", 1 - defaults.depth_on);

    settings_setint("effects_level", defaults.effects_level);
    settings_setint("highest_level", defaults.highest_level);

    settings_setint("donation_arrived", defaults.donation_arrived);
}

#endif
